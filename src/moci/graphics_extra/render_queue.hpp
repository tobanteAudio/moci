#pragma once

#include <moci/core/geometry/rectangle.hpp>
#include <moci/core/memory.hpp>
#include <moci/core/vector.hpp>
#include <moci/graphics/color.hpp>
#include <moci/render/api.hpp>
#include <moci/render/renderer.hpp>
#include <moci/render/shader.hpp>
#include <moci/render/texture.hpp>

#include <glm/glm.hpp>

#include <cstddef>
#include <cstdint>
#include <map>

namespace moci
{

class BatchRender2D
{
public:
    struct FrameStats
    {
        std::uint32_t drawCount    = 0;
        std::uint32_t textureCount = 0;
        std::uint32_t vertexCount  = 0;
        std::uint32_t quadCount    = 0;
        std::uint32_t circleCount  = 0;
    };

    BatchRender2D();
    ~BatchRender2D();

    // BatchRender2D is not copyable.
    BatchRender2D(const BatchRender2D&)                    = delete;
    auto operator=(const BatchRender2D&) -> BatchRender2D& = delete;

    auto startFrame(float width, float height) -> void;
    auto endFrame() -> void;

    auto drawText(const std::string& text, glm::vec2 position, float scale, ColorRGBA32 color) -> void;
    auto drawQuad(Rectangle<float> rect, ColorRGBA32 color, Texture2D::Optional texture = std::nullopt) -> void;
    auto drawCircle(float x, float y, float radius, int numSides, ColorRGBA32 color) -> void;

    [[nodiscard]] auto getFrameStats() const -> FrameStats;

private:
    static constexpr std::size_t maxQuadCount    = 10000;
    static constexpr std::size_t maxVertexCount  = maxQuadCount * 4;
    static constexpr std::size_t maxIndexCount   = maxQuadCount * 6;
    static constexpr std::size_t maxTextureUnits = 16;

    struct Vertex
    {
        glm::vec3 position        = {};
        ColorRGBA32 color         = {};
        glm::vec2 texture         = {};
        float textureIndex        = 0.0F;
        float textureIsMonochrome = 0.0F;
    };

    struct RenderData
    {
        Ref<VertexBuffer> vbo                      = nullptr;
        Ref<IndexBuffer> ibo                       = nullptr;
        Ref<VertexArray> vao                       = nullptr;
        Ref<Shader> shader                         = nullptr;
        Vector<Vertex> vertices                    = {};
        Vector<uint32_t> indices                   = {};
        std::uint32_t indexOffset                  = 0;
        Texture2D::Ptr defaultTexture              = nullptr;
        Vector<Texture2D::Ptr> textures            = {};
        BatchRender2D::FrameStats renderFrameStats = {};
    };

    struct Character
    {
        Texture2D::Ptr TextureID = {};  // ID handle of the glyph texture
        glm::ivec2 Size          = {};  // Size of glyph
        glm::ivec2 Bearing       = {};  // Offset from baseline to left/top of glyph
        std::uint32_t Advance    = {};  // Offset to advance to next glyph
    };

    auto fontInit(std::string const& fontPath) -> void;
    auto beginBatch() -> void;
    auto flush() -> void;
    auto flushIf(bool shouldFlush) -> void;
    auto endBatch() -> void;
    auto resetFrameStats() -> void;

    RenderData _data {};
    std::map<char, Character> _characters;
};

}  // namespace moci