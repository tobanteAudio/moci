#include "render_queue.hpp"

#include "moci/benchmark/profile.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace moci
{

RenderQueue::RenderQueue()
{
#if defined(MOCI_API_OPENGL_LEGACY)
    data_.shader = moci::Shader::Create("moci/moci/assets/es2_batch_render.glsl");
#else
    data_.shader = moci::Shader::Create("moci/moci/assets/gl4_batch_render.glsl");
#endif
    data_.shader->Bind();

    data_.vertices.reserve(MaxVertexCount);
    data_.indices.reserve(MaxIndexCount);
    data_.textures.reserve(MaxTextureUnits);

    data_.defaultTexture = moci::Texture2D::Create("moci/moci/assets/white_10x10.png");

    moci::BufferLayout layout = {
        {moci::ShaderDataType::Float3, "position"},            //
        {moci::ShaderDataType::Float4, "color"},               //
        {moci::ShaderDataType::Float2, "texture"},             //
        {moci::ShaderDataType::Float, "textureIndex"},         //
        {moci::ShaderDataType::Float, "textureIsMonochrome"},  //
    };
    data_.vbo.reset(moci::VertexBuffer::Create(nullptr, sizeof(Vertex) * MaxVertexCount, true));
    data_.vbo->SetLayout(layout);
    data_.vbo->Unbind();
    data_.ibo.reset(moci::IndexBuffer::Create(nullptr, MaxIndexCount, true));
    data_.ibo->Unbind();
    data_.vao = moci::VertexArray::Create();
    data_.vao->AddVertexBuffer(data_.vbo);
    data_.vao->SetIndexBuffer(data_.ibo);
    data_.vao->Unbind();

    FontInit("moci/moci/assets/fonts/open-sans/OpenSans-Bold.ttf");
}

RenderQueue::~RenderQueue()
{
    data_.vbo.reset();
    data_.ibo.reset();
    data_.vao.reset();
    data_.vertices.clear();
    data_.indices.clear();
    data_.textures.clear();
}

auto RenderQueue::StartFrame(float width, float height) -> void
{
    data_.shader->Bind();
    glm::mat4 proj = glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
    data_.shader->SetMat4("u_MVP", proj);
    int samplers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    data_.shader->SetInts("u_Textures", 16, samplers);

    ResetStats();
    BeginBatch();
}

auto RenderQueue::EndFrame() -> void
{
    EndBatch();
    Flush();
    data_.shader->Unbind();
}

auto RenderQueue::DrawText(std::string text, glm::vec2 position, float scale, Color color) -> void
{
    MOCI_PROFILE_FUNCTION();

    for (auto const c : text)
    {
        auto const& ch = characters_[c];

        float const xpos = position.x + ch.Bearing.x * scale;
        float const ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;
        float const w    = ch.Size.x * scale;
        float const h    = ch.Size.y * scale;

        // TODO: Check if size calculation is correct. See negative height in draw call.
        DrawQuad({xpos, ypos + h, w, -h}, color, ch.TextureID);

        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        position.x += (ch.Advance >> 6) * scale;  // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
}

auto RenderQueue::FontInit(std::string const& fontPath) -> void
{
    FT_Library ft = nullptr;
    if (FT_Init_FreeType(&ft) != 0)
    {
        MOCI_CORE_ERROR("freetype: Could not init FreeType Library");
    }

    FT_Face face = nullptr;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face) != 0)
    {
        MOCI_CORE_ERROR("freetype: Failed to load font");
    }

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
        auto texture = Texture2D::Create(  //
            Texture::Format::Luminance,    //
            face->glyph->bitmap.width,     //
            face->glyph->bitmap.rows,      //
            face->glyph->bitmap.buffer     //
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

auto RenderQueue::BeginBatch() -> void
{
    data_.vertices.clear();

    data_.indices.clear();
    data_.indexOffset = 0;

    data_.textures.clear();
    MOCI_CORE_ASSERT(data_.textures.empty(), "");

    data_.textures.push_back(data_.defaultTexture);
    data_.renderStats.textureCount++;
}

auto RenderQueue::EndBatch() -> void
{
    MOCI_PROFILE_FUNCTION();
    data_.vao->Bind();
    auto const verticesSize = static_cast<std::uint32_t>(data_.vertices.size() * sizeof(Vertex));
    data_.vbo->UploadData(0, verticesSize, data_.vertices.data());
    auto const indicesSize = static_cast<std::uint32_t>(data_.indices.size() * sizeof(unsigned short));
    data_.ibo->UploadData(0, indicesSize, data_.indices.data());
    data_.vao->Unbind();
}

auto RenderQueue::Flush() -> void
{
    MOCI_PROFILE_FUNCTION();
    for (size_t i = 0; i < data_.textures.size(); i++)
    {
        data_.textures[i]->Bind(static_cast<std::uint32_t>(i));
    }

    data_.vao->Bind();
    {
        MOCI_PROFILE_SCOPE("RenderQueue::Flush::Draw");
        auto const mode       = RendererAPI::DrawMode::Triangles;
        auto const type       = RendererAPI::ElementType::UnsignedShort;
        auto const numIndices = static_cast<std::uint32_t>(data_.indices.size());
        RenderCommand::DrawElements(mode, numIndices, type, nullptr);
    }
    data_.vao->Unbind();
    for (auto const& tex : data_.textures)
    {
        tex->Unbind();
    }

    data_.renderStats.drawCount++;
}

auto RenderQueue::FlushIf(bool shouldFlush) -> void
{
    if (shouldFlush)
    {
        EndBatch();
        Flush();
        BeginBatch();
    }
}
auto RenderQueue::DrawQuad(Rectangle<float> rect, Color color, Texture2D::Optional texture) -> void
{
    FlushIf(data_.indices.size() + 6 >= MaxIndexCount);

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
            FlushIf(data_.textures.size() == MaxTextureUnits);

            texID = static_cast<uint32_t>(data_.textures.size());
            data_.textures.push_back(texture.value());
            data_.renderStats.textureCount++;
        }
    }
    else
    {
        texID = 0;
    }

    MOCI_CORE_ASSERT(texID >= 0, "");

    auto const x      = rect.GetX();
    auto const y      = rect.GetY();
    auto const width  = rect.GetWidth();
    auto const height = rect.GetHeight();
    data_.vertices.push_back(Vertex {{x, y, 0.0f}, color, {0.0f, 0.0f}, static_cast<float>(texID), 1.0f});
    data_.vertices.push_back(Vertex {{x + width, y, 0.0f}, color, {1.0f, 0.0f}, static_cast<float>(texID), 1.0f});
    data_.vertices.push_back(
        Vertex {{x + width, y + height, 0.0f}, color, {1.0f, 1.0f}, static_cast<float>(texID), 1.0f});
    data_.vertices.push_back(Vertex {{x, y + height, 0.0f}, color, {0.0f, 1.0f}, static_cast<float>(texID), 1.0f});

    for (auto const i : {0, 1, 2, 2, 3, 0})
    {
        data_.indices.push_back(static_cast<unsigned short>(data_.indexOffset + i));
    }
    data_.indexOffset += 4;

    data_.renderStats.vertexCount += 4;
    data_.renderStats.quadCount += 1;
}

auto RenderQueue::DrawCircle(float x, float y, float radius, int numSides, Color color) -> void
{
    FlushIf(data_.indices.size() + (static_cast<size_t>(3) * static_cast<size_t>(numSides)) >= MaxIndexCount);

    auto const numVertices = numSides + 2;
    auto const doublePI    = 3.141f * 2.0f;
    data_.vertices.push_back(Vertex {{x, y, 0.0f}, color, {0.0f, 0.0f}, 0.0f});
    for (auto i = 1; i < numVertices; i++)
    {
        auto const newX = x + (radius * glm::cos(i * doublePI / static_cast<float>(numSides)));
        auto const newY = y + (radius * glm::sin(i * doublePI / static_cast<float>(numSides)));
        data_.vertices.push_back(Vertex {{newX, newY, 0.0f}, color, {0.0f, 0.0f}, 1.0f});
    }

    auto const origin = data_.indexOffset;
    for (auto i = 0; i < numSides; i++)
    {
        data_.indices.push_back(static_cast<unsigned short>(origin));
        data_.indices.push_back(static_cast<unsigned short>(origin + (1 + i)));
        data_.indices.push_back(static_cast<unsigned short>(origin + (2 + i)));
    }
    data_.indexOffset += numVertices;

    data_.renderStats.vertexCount += numVertices;
    data_.renderStats.circleCount += 1;
}
auto RenderQueue::GetStats() const -> RenderQueue::Stats { return data_.renderStats; }

auto RenderQueue::ResetStats() -> void { data_.renderStats = {}; }

}  // namespace moci