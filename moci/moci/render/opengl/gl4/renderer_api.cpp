#include "moci/render/opengl/gl4/renderer_api.hpp"

#include "moci/debug/instrumentor.hpp"
#include "moci/render/opengl/gl4/gl4.hpp"

namespace moci
{

void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length,
                           const char* message, const void* userParam)
{
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

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) { glClearColor(color.r, color.g, color.b, color.a); }

void OpenGLRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

// void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
// {
//     glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
//     glBindTexture(GL_TEXTURE_2D, 0);
// }

}  // namespace moci
