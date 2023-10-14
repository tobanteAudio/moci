#include "renderer_api.hpp"

#if defined(MOCI_API_OPENGL_MODERN)

#include "gl4.hpp"

#include <moci/core/benchmark/profile.hpp>

namespace moci
{

namespace
{
auto MociToOpenGLDrawMode(RenderDrawMode const mode) noexcept -> int
{

    switch (mode)
    {
        case RenderDrawMode::None:
        {
            MOCI_CORE_ERROR("draw mode 'None' not supported");
            break;
        }
        case RenderDrawMode::Points:
        {
            return GL_POINTS;
        }
        case RenderDrawMode::LineStrip:
        {
            return GL_LINE_STRIP;
        }
        case RenderDrawMode::LineLoop:
        {
            return GL_LINE_LOOP;
        }
        case RenderDrawMode::Lines:
        {
            return GL_LINES;
        }
        case RenderDrawMode::TriangleStrips:
        {
            return GL_TRIANGLE_STRIP;
        }
        case RenderDrawMode::TriangleFan:
        {
            return GL_TRIANGLE_FAN;
        }
        case RenderDrawMode::Triangles:
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

void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length,
                           const char* message, const void* userParam)
{
    ignoreUnused(source);
    ignoreUnused(type);
    ignoreUnused(id);
    ignoreUnused(length);
    ignoreUnused(userParam);

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH: MOCI_CORE_CRITICAL(message); return;
        case GL_DEBUG_SEVERITY_MEDIUM: MOCI_CORE_ERROR(message); return;
        case GL_DEBUG_SEVERITY_LOW: MOCI_CORE_WARN(message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION: MOCI_CORE_TRACE(message); return;
    }

    MOCI_CORE_ASSERT(false, "Unknown severity level!");
}

void OpenGLRendererAPI::init()
{
    MOCI_PROFILE_FUNCTION();

#ifdef MOCI_DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::setViewport(Rectangle<uint32_t> viewport)
{
    glViewport(viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight());
}

void OpenGLRendererAPI::setClearColor(ColorRGBA32 color)
{
    auto const red   = color.red();
    auto const green = color.green();
    auto const blue  = color.blue();
    auto const alpha = color.alpha();
    glClearColor(red, green, blue, alpha);
}

void OpenGLRendererAPI::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenGLRendererAPI::drawArrays(RenderDrawMode const mode, uint32_t first, uint32_t count)
{
    auto const glDrawMode = MociToOpenGLDrawMode(mode);
    glDrawArrays(glDrawMode, first, count);
}

void OpenGLRendererAPI::drawIndexed(RenderDrawMode const mode, uint32_t count, void* indices)
{
    auto const glDrawMode = MociToOpenGLDrawMode(mode);
    glDrawElements(glDrawMode, count, GL_UNSIGNED_INT, indices);
}

void OpenGLRendererAPI::drawIndexed(RenderDrawMode const mode, std::shared_ptr<VertexArray> const& vertexArray)
{
    auto const glDrawMode = MociToOpenGLDrawMode(mode);
    glDrawElements(glDrawMode, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}

auto OpenGLRendererAPI::maxTextureSize() -> std::uint32_t
{
    GLint maxTextureSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    return static_cast<std::uint32_t>(maxTextureSize);
}
auto OpenGLRendererAPI::maxTextureUnits() -> std::uint32_t
{
    GLint maxTextureUnits = 0;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    return static_cast<std::uint32_t>(maxTextureUnits);
}
auto OpenGLRendererAPI::maxVertexAttributes() -> std::uint32_t
{
    GLint maxAttributes = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    return static_cast<std::uint32_t>(maxAttributes);
}

auto OpenGLRendererAPI::maxUniformVectors() -> std::uint32_t
{
    GLint maxUniformVectors = 0;
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxUniformVectors);
    return static_cast<std::uint32_t>(maxUniformVectors);
}

}  // namespace moci

#endif