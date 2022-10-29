#pragma once

#include "moci/core/geometry/rectangle.hpp"
#include "moci/graphics/color.hpp"
#include "moci/render/vertex_array.hpp"

#include <any>

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

    virtual ~RendererAPI() = default;
    virtual void init()    = 0;

    virtual void setViewport(Rectangle<uint32_t> viewport) = 0;
    virtual void setClearColor(Color color)                = 0;
    virtual void clear()                                   = 0;

    virtual void drawArrays(RenderDrawMode mode, uint32_t first, uint32_t count)       = 0;
    virtual void drawIndexed(RenderDrawMode mode, uint32_t count, void* indices)       = 0;
    virtual void drawIndexed(RenderDrawMode mode, Ref<VertexArray> const& vertexArray) = 0;

    virtual auto maxTextureSize() -> std::uint32_t      = 0;
    virtual auto maxTextureUnits() -> std::uint32_t     = 0;
    virtual auto maxVertexAttributes() -> std::uint32_t = 0;
    virtual auto maxUniformVectors() -> std::uint32_t   = 0;

    virtual auto getCurrentDevice() -> std::any = 0;

    inline static auto getApi() -> API { return sApi; }

private:
    static API sApi;
};

}  // namespace moci