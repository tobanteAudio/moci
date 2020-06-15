#include "renderer_api.hpp"

#if defined(MOCI_API_OPENGL_MODERN)

#include "gl4.hpp"
#include "moci_core/moci_core.hpp"

namespace moci
{

void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length,
                           const char* message, const void* userParam)
{
    IgnoreUnused(source);
    IgnoreUnused(type);
    IgnoreUnused(id);
    IgnoreUnused(length);
    IgnoreUnused(userParam);

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH: MOCI_CORE_CRITICAL(message); return;
        case GL_DEBUG_SEVERITY_MEDIUM: MOCI_CORE_ERROR(message); return;
        case GL_DEBUG_SEVERITY_LOW: MOCI_CORE_WARN(message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION: MOCI_CORE_TRACE(message); return;
    }

    MOCI_CORE_ASSERT(false, "Unknown severity level!");
}

void OpenGLRendererAPI::Init()
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

void OpenGLRendererAPI::SetViewport(Rectangle<uint32_t> viewport)
{
    glViewport(viewport.GetX(), viewport.GetY(), viewport.GetWidth(), viewport.GetHeight());
}

void OpenGLRendererAPI::SetClearColor(Color color)
{
    auto const red   = color.GetRed();
    auto const green = color.GetGreen();
    auto const blue  = color.GetBlue();
    auto const alpha = color.GetAlpha();
    glClearColor(red, green, blue, alpha);
}

void OpenGLRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenGLRendererAPI::DrawArrays(RendererAPI::DrawMode mode, uint32_t first, uint32_t count)
{
    IgnoreUnused(mode);
    glDrawArrays(GL_TRIANGLES, first, count);
}

void OpenGLRendererAPI::DrawElements(RendererAPI::DrawMode mode, uint32_t count, RendererAPI::ElementType type,
                                     void* indices)
{
    IgnoreUnused(mode);
    IgnoreUnused(type);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, indices);
}

void OpenGLRendererAPI::DrawIndexed(Ref<VertexArray> const& vertexArray)
{
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

auto OpenGLRendererAPI::MaxTextureSize() -> std::uint32_t
{
    GLint maxTextureSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    return static_cast<std::uint32_t>(maxTextureSize);
}
auto OpenGLRendererAPI::MaxTextureUnits() -> std::uint32_t
{
    GLint maxTextureUnits = 0;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    return static_cast<std::uint32_t>(maxTextureUnits);
}
auto OpenGLRendererAPI::MaxVertexAttributes() -> std::uint32_t
{
    GLint maxAttributes = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);
    return static_cast<std::uint32_t>(maxAttributes);
}

auto OpenGLRendererAPI::MaxUniformVectors() -> std::uint32_t
{
    GLint maxUniformVectors = 0;
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &maxUniformVectors);
    return static_cast<std::uint32_t>(maxUniformVectors);
}

// void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
// {
//     glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
//     glBindTexture(GL_TEXTURE_2D, 0);
// }

}  // namespace moci

#endif