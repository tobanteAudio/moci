#pragma once

#include <glm/glm.hpp>

namespace moci
{

class RendererAPI
{
public:
    enum class API
    {
        None     = 0,
        OpenGL   = 1,
        OpenGLES = 2
    };

    enum class DrawMode
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

    enum class ElementType
    {
        None          = 0,
        UnsignedByte  = 1,
        UnsignedShort = 2,
    };

public:
    virtual ~RendererAPI()                                                                    = default;
    virtual void Init()                                                                       = 0;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)         = 0;
    virtual void SetClearColor(const glm::vec4& color)                                        = 0;
    virtual void Clear()                                                                      = 0;
    virtual void DrawArrays(DrawMode mode, uint32_t first, uint32_t count)                    = 0;
    virtual void DrawElements(DrawMode mode, uint32_t count, ElementType type, void* indices) = 0;
    virtual auto MaxTextureSize() -> std::uint32_t                                            = 0;
    virtual auto MaxTextureUnits() -> std::uint32_t                                           = 0;
    virtual auto MaxVertexAttributes() -> std::uint32_t                                       = 0;
    virtual auto MaxUniformVectors() -> std::uint32_t                                         = 0;
    inline static auto GetAPI() -> API { return s_API; }

private:
    static API s_API;
};

}  // namespace moci