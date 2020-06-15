#pragma once

#include "moci_math/moci_math.hpp"
#include "moci_render_base/moci_render_base.hpp"

#include <glm/glm.hpp>

namespace moci
{
class OpenGLESRendererAPI : public RendererAPI
{
public:
    ~OpenGLESRendererAPI() override = default;
    void Init() override;

    void SetViewport(Rectangle<uint32_t> viewport) override;
    void SetClearColor(Color color) override;
    void Clear() override;

    void DrawArrays(RenderDrawMode mode, uint32_t first, uint32_t count) override;
    void DrawIndexed(RenderDrawMode mode, uint32_t count, void* indices) override;
    void DrawIndexed(RenderDrawMode mode, Ref<VertexArray> const& vertexArray) override;

    auto MaxTextureSize() -> std::uint32_t override;
    auto MaxTextureUnits() -> std::uint32_t override;
    auto MaxVertexAttributes() -> std::uint32_t override;
    auto MaxUniformVectors() -> std::uint32_t override;
};

}  // namespace moci
