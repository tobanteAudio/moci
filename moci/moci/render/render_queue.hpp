#pragma once

#include "moci_render_base/moci_render_base.hpp"

#include "moci/render/color.hpp"
#include "moci/render/render_command.hpp"
#include "moci_core/core/memory.hpp"
#include "moci_math/geometry/rectangle.hpp"

#include <glm/glm.hpp>

#include <cstddef>
#include <cstdint>

#include "moci_core/core/vector.hpp"
#include <map>

namespace moci
{

class RenderQueue
{
public:
    struct Stats
    {
        std::uint32_t drawCount {0};
        std::uint32_t textureCount {0};
        std::uint32_t vertexCount {0};
        std::uint32_t quadCount {0};
        std::uint32_t circleCount {0};
    };

    RenderQueue();
    ~RenderQueue();

    // RenderQueue is not copyable.
    RenderQueue(const RenderQueue&) = delete;
    auto operator=(const RenderQueue&) -> RenderQueue& = delete;

    auto StartFrame(float width, float height) -> void;
    auto EndFrame() -> void;

    auto DrawText(std::string text, glm::vec2 position, float scale, Color color) -> void;
    auto DrawQuad(Rectangle<float> rect, Color color, Texture2D::Optional texture = std::nullopt) -> void;
    auto DrawCircle(float x, float y, float radius, int numSides, Color color) -> void;

    [[nodiscard]] auto GetStats() const -> Stats;

private:
    static constexpr std::size_t MaxQuadCount    = 10000;
    static constexpr std::size_t MaxVertexCount  = MaxQuadCount * 4;
    static constexpr std::size_t MaxIndexCount   = MaxQuadCount * 6;
    static constexpr std::size_t MaxTextureUnits = 16;

    struct Vertex
    {
        glm::vec3 position;
        Color color;
        glm::vec2 texture;
        float textureIndex;
        float textureIsMonochrome {0.0f};
    };

    struct RenderData
    {
        Ref<VertexBuffer> vbo;
        Ref<IndexBuffer> ibo;
        Ref<VertexArray> vao;
        Ref<Shader> shader;

        Vector<Vertex> vertices;
        Vector<unsigned short> indices;
        std::uint32_t indexOffset;

        Texture2D::Ptr defaultTexture;
        Vector<Texture2D::Ptr> textures;

        RenderQueue::Stats renderStats;
    };

    struct Character
    {
        Texture2D::Ptr TextureID {};  // ID handle of the glyph texture
        glm::ivec2 Size {};           // Size of glyph
        glm::ivec2 Bearing {};        // Offset from baseline to left/top of glyph
        std::uint32_t Advance {};     // Offset to advance to next glyph
    };

    auto FontInit(std::string const& fontPath) -> void;
    auto BeginBatch() -> void;
    auto Flush() -> void;
    auto FlushIf(bool shouldFlush) -> void;
    auto EndBatch() -> void;
    auto ResetStats() -> void;

    RenderData data_ {};
    std::map<char, Character> characters_;
};

}  // namespace moci