#pragma once

#include "moci/render/renderer_api.hpp"

namespace moci
{

class OpenGLRendererAPI : public RendererAPI
{
public:
    void Init() override;
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    void SetClearColor(const glm::vec4& color) override;
    void Clear() override;

    void DrawArrays(DrawMode mode, uint32_t first, uint32_t count) override {}
    void DrawElements(DrawMode mode, uint32_t count, ElementType type, void* indices) override {}
    auto MaxTextureSize() -> std::uint32_t override {}
    auto MaxTextureUnits() -> std::uint32_t override {}
    auto MaxVertexAttributes() -> std::uint32_t override {}
    auto MaxUniformVectors() -> std::uint32_t override {}

    // void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
};

}  // namespace moci