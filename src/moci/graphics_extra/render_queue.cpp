#include "render_queue.hpp"

#include "moci/benchmark/profile.hpp"
#include "moci/render/buffer.hpp"
#include "moci/render/render_factory.hpp"
#include "moci/render/shader.hpp"
#include "moci/render/texture.hpp"
#include "moci/render/vertex_array.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/gtc/matrix_transform.hpp>

#include <array>

namespace moci
{

BatchRender2D::BatchRender2D()
{
#if defined(MOCI_API_OPENGL_LEGACY)
    data_.shader = moci::RenderFactory::makeShader("assets/es2_batch_render.glsl");
#else
    data_.shader = moci::RenderFactory::makeShader("assets/gl4_batch_render.glsl");
#endif
    data_.shader->bind();

    data_.vertices.reserve(maxVertexCount);
    data_.indices.reserve(maxIndexCount);
    data_.textures.reserve(maxTextureUnits);

    data_.defaultTexture = moci::RenderFactory::makeTexture2D("assets/white_10x10.png");

    moci::BufferLayout layout = {
        {moci::ShaderDataType::Float3, "position"},            //
        {moci::ShaderDataType::Float4, "color"},               //
        {moci::ShaderDataType::Float2, "texture"},             //
        {moci::ShaderDataType::Float, "textureIndex"},         //
        {moci::ShaderDataType::Float, "textureIsMonochrome"},  //
    };
    data_.vbo.reset(moci::RenderFactory::makeVertexBuffer(nullptr, sizeof(Vertex) * maxVertexCount, true));
    data_.vbo->setLayout(layout);
    data_.vbo->unbind();
    data_.ibo.reset(moci::RenderFactory::makeIndexBuffer({{}, maxIndexCount, true}));
    data_.ibo->unbind();
    data_.vao = moci::RenderFactory::makeVertexArray();
    data_.vao->addVertexBuffer(data_.vbo);
    data_.vao->setIndexBuffer(data_.ibo);
    data_.vao->unbind();

    fontInit("assets/fonts/open-sans/OpenSans-Bold.ttf");
}

BatchRender2D::~BatchRender2D()
{
    data_.vbo.reset();
    data_.ibo.reset();
    data_.vao.reset();
    data_.vertices.clear();
    data_.indices.clear();
    data_.textures.clear();
}

auto BatchRender2D::startFrame(float width, float height) -> void
{
    MOCI_PROFILE_FUNCTION();
    data_.shader->bind();
    auto const proj = glm::ortho(0.0F, width, height, 0.0F, -1.0F, 1.0F);
    data_.shader->setMat4("u_MVP", proj);
    auto samplers = std::array<int, 16> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    data_.shader->setInts("u_Textures", (int)samplers.size(), samplers.data());

    resetFrameStats();
    beginBatch();
}

auto BatchRender2D::endFrame() -> void
{
    MOCI_PROFILE_FUNCTION();
    endBatch();
    flush();
    data_.shader->unbind();
}

auto BatchRender2D::drawText(const std::string& text, glm::vec2 position, float scale, Color color) -> void
{
    MOCI_PROFILE_FUNCTION();

    for (auto const c : text)
    {
        auto const& ch = characters_[c];

        float const xpos = position.x + ch.Bearing.x * scale;
        float const ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;
        float const w    = ch.Size.x * scale;
        float const h    = ch.Size.y * scale;

        drawQuad({xpos, ypos - h, w, h}, color, ch.TextureID);

        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        position.x += (ch.Advance >> 6) * scale;  // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
}

auto BatchRender2D::fontInit(std::string const& fontPath) -> void
{
    FT_Library ft = nullptr;
    if (FT_Init_FreeType(&ft) != 0) { MOCI_CORE_ERROR("freetype: Could not init FreeType Library"); }

    FT_Face face = nullptr;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face) != 0) { MOCI_CORE_ERROR("freetype: Failed to load font"); }

    FT_Set_Pixel_Sizes(face, 0, 48);

    for (std::uint8_t c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
        {
            MOCI_CORE_ERROR("freetype: Failed to load Glyph");
            continue;
        }
        // Generate texture
        auto texture = RenderFactory::makeTexture2D(  //
            Texture::Format::Luminance,               //
            face->glyph->bitmap.width,                //
            face->glyph->bitmap.rows,                 //
            face->glyph->bitmap.buffer                //
        );

        Character character = {
            texture,                                                          //
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),  //
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),    //
            static_cast<std::uint32_t>(face->glyph->advance.x)                //
        };

        characters_.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

auto BatchRender2D::beginBatch() -> void
{
    data_.vertices.clear();

    data_.indices.clear();
    data_.indexOffset = 0;

    data_.textures.clear();
    MOCI_CORE_ASSERT(data_.textures.empty(), "");

    data_.textures.push_back(data_.defaultTexture);
    data_.renderFrameStats.textureCount++;
}

auto BatchRender2D::endBatch() -> void
{
    MOCI_PROFILE_FUNCTION();
    data_.vao->bind();
    auto const verticesSize = static_cast<std::uint32_t>(data_.vertices.size() * sizeof(Vertex));
    data_.vbo->uploadData(0, verticesSize, data_.vertices.data());
    data_.ibo->uploadData(0, data_.indices);
    data_.vao->unbind();
}

auto BatchRender2D::flush() -> void
{
    MOCI_PROFILE_FUNCTION();
    for (size_t i = 0; i < data_.textures.size(); i++) { data_.textures[i]->bind(static_cast<std::uint32_t>(i)); }

    data_.vao->bind();
    {
        MOCI_PROFILE_SCOPE("BatchRender2D::Flush::Draw");
        auto const mode       = RenderDrawMode::Triangles;
        auto const numIndices = static_cast<std::uint32_t>(data_.indices.size());
        RenderCommand::drawIndexed(mode, numIndices, nullptr);
    }
    data_.vao->unbind();
    for (auto const& tex : data_.textures) { tex->unbind(); }

    data_.renderFrameStats.drawCount++;
}

auto BatchRender2D::flushIf(bool shouldFlush) -> void
{
    if (shouldFlush)
    {
        endBatch();
        flush();
        beginBatch();
    }
}
auto BatchRender2D::drawQuad(Rectangle<float> rect, Color color, Texture2D::Optional texture) -> void
{
    flushIf(data_.indices.size() + 6 >= maxIndexCount);

    int texID {-1};
    if (texture)
    {
        std::uint32_t counter = 0;
        for (auto& tex : data_.textures)
        {
            if (tex.get() == texture->get())
            {
                texID = counter;
                break;
            }
            counter++;
        }

        if (texID == -1)
        {
            flushIf(data_.textures.size() == maxTextureUnits);

            texID = static_cast<uint32_t>(data_.textures.size());
            data_.textures.push_back(texture.value());
            data_.renderFrameStats.textureCount++;
        }
    }
    else { texID = 0; }

    MOCI_CORE_ASSERT(texID >= 0, "");

    auto const x      = rect.getX();
    auto const y      = rect.getY();
    auto const width  = rect.getWidth();
    auto const height = rect.getHeight();
    data_.vertices.push_back(Vertex {{x, y, 0.0F}, color, {0.0F, 0.0F}, static_cast<float>(texID), 1.0F});
    data_.vertices.push_back(Vertex {{x + width, y, 0.0F}, color, {1.0F, 0.0F}, static_cast<float>(texID), 1.0F});
    data_.vertices.push_back(
        Vertex {{x + width, y + height, 0.0F}, color, {1.0F, 1.0F}, static_cast<float>(texID), 1.0F});
    data_.vertices.push_back(Vertex {{x, y + height, 0.0F}, color, {0.0F, 1.0F}, static_cast<float>(texID), 1.0F});

    for (auto const i : {0, 1, 2, 2, 3, 0}) { data_.indices.push_back(static_cast<uint32_t>(data_.indexOffset + i)); }
    data_.indexOffset += 4;

    data_.renderFrameStats.vertexCount += 4;
    data_.renderFrameStats.quadCount += 1;
}

auto BatchRender2D::drawCircle(float x, float y, float radius, int numSides, Color color) -> void
{
    flushIf(data_.indices.size() + (static_cast<size_t>(3) * static_cast<size_t>(numSides)) >= maxIndexCount);

    auto const numVertices = numSides + 2;
    auto const doublePI    = 3.141F * 2.0F;
    data_.vertices.push_back(Vertex {{x, y, 0.0F}, color, {0.0F, 0.0F}, 0.0F});
    for (auto i = 1; i < numVertices; i++)
    {
        auto const newX = x + (radius * glm::cos(i * doublePI / static_cast<float>(numSides)));
        auto const newY = y + (radius * glm::sin(i * doublePI / static_cast<float>(numSides)));
        data_.vertices.push_back(Vertex {{newX, newY, 0.0F}, color, {0.0F, 0.0F}, 1.0F});
    }

    auto const origin = data_.indexOffset;
    for (auto i = 0; i < numSides; i++)
    {
        data_.indices.push_back(static_cast<uint32_t>(origin));
        data_.indices.push_back(static_cast<uint32_t>(origin + (1 + i)));
        data_.indices.push_back(static_cast<uint32_t>(origin + (2 + i)));
    }
    data_.indexOffset += numVertices;

    data_.renderFrameStats.vertexCount += numVertices;
    data_.renderFrameStats.circleCount += 1;
}
auto BatchRender2D::getFrameStats() const -> BatchRender2D::FrameStats { return data_.renderFrameStats; }

auto BatchRender2D::resetFrameStats() -> void { data_.renderFrameStats = {}; }

}  // namespace moci