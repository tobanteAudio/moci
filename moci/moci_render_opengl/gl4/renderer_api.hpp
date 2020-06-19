#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

#include "moci_core/moci_core.hpp"
#include "moci_render_base/moci_render_base.hpp"

namespace moci
{

class OpenGLRendererAPI : public RendererAPI
{
public:
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

    auto GetCurrentDevice() -> std::any override { return {}; }
};

}  // namespace moci

#endif