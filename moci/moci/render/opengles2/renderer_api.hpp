#pragma once

#include "moci/render/renderer_api.hpp"

#include <glm/glm.hpp>

namespace moci
{
class OpenGLESRendererAPI : public RendererAPI
{
public:
    ~OpenGLESRendererAPI() override = default;
    void Init() override;
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    void SetClearColor(const glm::vec4& color) override;
    void Clear() override;

    void DrawArrays(RendererAPI::DrawMode mode, uint32_t first, uint32_t count) override;
    void DrawElements(RendererAPI::DrawMode mode, uint32_t count, RendererAPI::ElementType type,
                      void* indices) override;

    auto MaxTextureSize() -> std::uint32_t override;
    auto MaxTextureUnits() -> std::uint32_t override;
    auto MaxVertexAttributes() -> std::uint32_t override;
};

}  // namespace moci
