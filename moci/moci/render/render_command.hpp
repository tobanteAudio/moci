#pragma once

#include "renderer_api.hpp"

#include <memory>

namespace moci
{

class RenderCommand
{
public:
    inline static void Init() { s_RendererAPI->Init(); }

    inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        s_RendererAPI->SetViewport(x, y, width, height);
    }

    inline static void SetClearColor(const glm::vec4& color) { s_RendererAPI->SetClearColor(color); }

    inline static void Clear() { s_RendererAPI->Clear(); }

    inline static void DrawArrays(RendererAPI::DrawMode mode, uint32_t first, uint32_t count)
    {
        s_RendererAPI->DrawArrays(mode, first, count);
    }

    inline static void DrawElements(RendererAPI::DrawMode mode, uint32_t count, RendererAPI::ElementType type,
                                    void* indices)
    {
        s_RendererAPI->DrawElements(mode, count, type, indices);
    }

    inline static auto MaxTextureSize() -> std::uint32_t { return s_RendererAPI->MaxTextureSize(); }
    inline static auto MaxTextureUnits() -> std::uint32_t { return s_RendererAPI->MaxTextureUnits(); }
    inline static auto MaxVertexAttributes() -> std::uint32_t { return s_RendererAPI->MaxVertexAttributes(); }

private:
    static std::unique_ptr<RendererAPI> s_RendererAPI;
};

}  // namespace moci
