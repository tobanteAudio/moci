#pragma once

#include "moci/benchmark/profile.hpp"
#include "moci/core/logging.hpp"

#include "moci_graphics_base/moci_graphics_base.hpp"
#include "moci_render_base/moci_render_base.hpp"

namespace moci
{

class RenderCommand
{
public:
    inline static void Init()
    {
        MOCI_PROFILE_FUNCTION();
        s_RendererAPI->Init();
    }

    inline static void SetViewport(Rectangle<uint32_t> viewport)
    {
        MOCI_PROFILE_FUNCTION();
        s_RendererAPI->SetViewport(viewport);
    }

    inline static void SetClearColor(Color const& color)
    {
        MOCI_PROFILE_FUNCTION();
        s_RendererAPI->SetClearColor(color);
    }

    inline static void Clear()
    {
        MOCI_PROFILE_FUNCTION();
        s_RendererAPI->Clear();
    }

    inline static void DrawArrays(RenderDrawMode mode, uint32_t first, uint32_t count)
    {
        MOCI_PROFILE_FUNCTION();
        s_RendererAPI->DrawArrays(mode, first, count);
    }

    inline static void DrawIndexed(RenderDrawMode mode, uint32_t count, void* indices)
    {
        MOCI_PROFILE_FUNCTION();
        s_RendererAPI->DrawIndexed(mode, count, indices);
    }

    inline static void DrawIndexed(RenderDrawMode mode, Ref<VertexArray> const& vertexArray)
    {
        MOCI_PROFILE_FUNCTION();
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
