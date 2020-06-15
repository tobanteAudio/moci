#pragma once

#include "moci_core/moci_core.hpp"
#include "moci_graphics_base/moci_graphics_base.hpp"
#include "moci_render_base/moci_render_base.hpp"

namespace moci
{

class RenderCommand
{
public:
    inline static void Init() { s_RendererAPI->Init(); }

    inline static void SetViewport(Rectangle<uint32_t> viewport) { s_RendererAPI->SetViewport(viewport); }

    inline static void SetClearColor(Color const& color) { s_RendererAPI->SetClearColor(color); }

    inline static void Clear() { s_RendererAPI->Clear(); }

    inline static void DrawArrays(RenderDrawMode mode, uint32_t first, uint32_t count)
    {
        s_RendererAPI->DrawArrays(mode, first, count);
    }

    inline static void DrawIndexed(RenderDrawMode mode, uint32_t count, void* indices)
    {
        s_RendererAPI->DrawIndexed(mode, count, indices);
    }

    inline static void DrawIndexed(RenderDrawMode mode, Ref<VertexArray> const& vertexArray)
    {
        s_RendererAPI->DrawIndexed(mode, vertexArray);
    }

    inline static auto MaxTextureSize() -> std::uint32_t { return s_RendererAPI->MaxTextureSize(); }
    inline static auto MaxTextureUnits() -> std::uint32_t { return s_RendererAPI->MaxTextureUnits(); }
    inline static auto MaxVertexAttributes() -> std::uint32_t { return s_RendererAPI->MaxVertexAttributes(); }
    inline static auto MaxUniformVectors() -> std::uint32_t { return s_RendererAPI->MaxUniformVectors(); }

private:
    static Scope<RendererAPI> s_RendererAPI;
};

}  // namespace moci
