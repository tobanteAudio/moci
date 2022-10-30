#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

#include "moci/render/api.hpp"

namespace moci
{

class OpenGLRendererAPI : public RendererAPI
{
public:
    void init() override;
    void setViewport(Rectangle<uint32_t> viewport) override;

    void setClearColor(Color color) override;
    void clear() override;

    void drawArrays(RenderDrawMode mode, uint32_t first, uint32_t count) override;
    void drawIndexed(RenderDrawMode mode, uint32_t count, void* indices) override;
    void drawIndexed(RenderDrawMode mode, Ref<VertexArray> const& vertexArray) override;
    auto maxTextureSize() -> std::uint32_t override;
    auto maxTextureUnits() -> std::uint32_t override;
    auto maxVertexAttributes() -> std::uint32_t override;
    auto maxUniformVectors() -> std::uint32_t override;

    auto getCurrentDevice() -> std::any override { return {}; }
};

}  // namespace moci

#endif