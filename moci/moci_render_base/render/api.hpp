#pragma once

#include "moci_graphics_base/moci_graphics_base.hpp"
#include "moci_math/moci_math.hpp"
#include "moci_render_base/render/vertex_array.hpp"

namespace moci
{

enum class RenderDrawMode
{
    None = 0,
    Points,
    LineStrip,
    LineLoop,
    Lines,
    TriangleStrips,
    TriangleFan,
    Triangles,
};

class RendererAPI
{
public:
    enum class API
    {
        None     = 0,
        OpenGL   = 1,
        OpenGLES = 2
    };

    virtual ~RendererAPI()                                                             = default;
    virtual void Init()                                                                = 0;
    virtual void SetViewport(Rectangle<uint32_t> viewport)                             = 0;
    virtual void SetClearColor(Color color)                                            = 0;
    virtual void Clear()                                                               = 0;
    virtual void DrawArrays(RenderDrawMode mode, uint32_t first, uint32_t count)       = 0;
    virtual void DrawIndexed(RenderDrawMode mode, uint32_t count, void* indices)       = 0;
    virtual void DrawIndexed(RenderDrawMode mode, Ref<VertexArray> const& vertexArray) = 0;
    virtual auto MaxTextureSize() -> std::uint32_t                                     = 0;
    virtual auto MaxTextureUnits() -> std::uint32_t                                    = 0;
    virtual auto MaxVertexAttributes() -> std::uint32_t                                = 0;
    virtual auto MaxUniformVectors() -> std::uint32_t                                  = 0;
    inline static auto GetAPI() -> API { return s_API; }

private:
    static API s_API;
};

}  // namespace moci