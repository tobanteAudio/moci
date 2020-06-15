#include "moci_render_opengles/es2/renderer_api.hpp"

#include "moci_render_opengles/es2/es2.hpp"

namespace moci
{
namespace
{
auto MociToOpenGLDrawMode(RendererAPI::DrawMode const mode) noexcept -> int
{

    switch (mode)
    {
        case RendererAPI::DrawMode::None:
        {
            MOCI_CORE_ERROR("draw mode 'None' not supported");
            break;
        }
        case RendererAPI::DrawMode::Points:
        {
            return GL_POINTS;
        }
        case RendererAPI::DrawMode::LineStrip:
        {
            return GL_LINE_STRIP;
        }
        case RendererAPI::DrawMode::LineLoop:
        {
            return GL_LINE_LOOP;
        }
        case RendererAPI::DrawMode::Lines:
        {
            return GL_LINES;
        }
        case RendererAPI::DrawMode::TriangleStrips:
        {
            return GL_TRIANGLE_STRIP;
        }
        case RendererAPI::DrawMode::TriangleFan:
        {
            return GL_TRIANGLE_FAN;
        }
        case RendererAPI::DrawMode::Triangles:
        {
            return GL_TRIANGLES;
        }
        default:
        {
            break;
        }
    }
    return 0;
}

}  // namespace

void OpenGLESRendererAPI::Init()
{
    glEnable(GL_BLEND);
    GLCall(glEnable(GL_DEPTH_TEST));
    // Accept fragment if it closer to the camera than the former one
    GLCall(glDepthFunc(GL_LESS));

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLESRendererAPI::SetViewport(Rectangle<uint32_t> viewport)
{
    glViewport(viewport.GetX(), viewport.GetY(), viewport.GetWidth(), viewport.GetHeight());
}

void OpenGLESRendererAPI::SetClearColor(Color color)
{
    auto const red   = color.GetRed();
    auto const green = color.GetGreen();
    auto const blue  = color.GetBlue();
    auto const alpha = color.GetAlpha();
    glClearColor(red, green, blue, alpha);
}

void OpenGLESRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenGLESRendererAPI::DrawArrays(RendererAPI::DrawMode const mode, uint32_t first, uint32_t count)
{
    auto const openGLMode = MociToOpenGLDrawMode(mode);
    GLCall(glDrawArrays(openGLMode, first, count));
}

void OpenGLESRendererAPI::DrawIndexed(RendererAPI::DrawMode const mode, uint32_t count, RendererAPI::ElementType type,
                                      void* indices)
{
    auto const openGLMode = MociToOpenGLDrawMode(mode);
    IgnoreUnused(type);
    GLCall(glDrawElements(openGLMode, count, GL_UNSIGNED_SHORT, indices));
}

void OpenGLESRendererAPI::DrawIndexed(RendererAPI::DrawMode const mode, Ref<VertexArray> const& vertexArray)
{
    auto const openGLMode = MociToOpenGLDrawMode(mode);
    GLCall(glDrawElements(openGLMode, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_SHORT, nullptr));
}

auto OpenGLESRendererAPI::MaxTextureSize() -> std::uint32_t
{
    GLint maxTextureSize = 0;
    GLCall(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize));
    return static_cast<std::uint32_t>(maxTextureSize);
}
auto OpenGLESRendererAPI::MaxTextureUnits() -> std::uint32_t
{
    GLint maxTextureUnits = 0;
    GLCall(glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxTextureUnits));
    return static_cast<std::uint32_t>(maxTextureUnits);
}
auto OpenGLESRendererAPI::MaxVertexAttributes() -> std::uint32_t
{
    GLint maxAttributes = 0;
    GLCall(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes));
    return static_cast<std::uint32_t>(maxAttributes);
}

auto OpenGLESRendererAPI::MaxUniformVectors() -> std::uint32_t
{
    GLint maxUniformVectors = 0;
    // GLCall(glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxUniformVectors));
    return static_cast<std::uint32_t>(maxUniformVectors);
}

}  // namespace moci