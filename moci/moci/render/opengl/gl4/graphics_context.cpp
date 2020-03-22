#include "moci/render/opengl/gl4/graphics_context.hpp"

#include "moci/debug/instrumentor.hpp"
#include "moci/render/opengl/gl4/gl4.hpp"

namespace moci
{

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
{
    MOCI_CORE_ASSERT(windowHandle, "Window handle is null!")
}

void OpenGLContext::Init()
{
    MOCI_PROFILE_FUNCTION();

    glfwMakeContextCurrent(m_WindowHandle);
    // Initialize OpenGL loader
    if (auto err = glewInit() != GLEW_OK; err)
    {
        MOCI_CORE_ASSERT(err, "Failed to initialize glew!");
    }

    MOCI_CORE_INFO("OpenGL Info:");
    MOCI_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
    MOCI_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
    MOCI_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

#ifdef MOCI_ENABLE_ASSERTS
    int versionMajor;
    int versionMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
    glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

    MOCI_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5),
                     "moci requires at least OpenGL version 4.5!");
#endif
}

void OpenGLContext::SwapBuffers()
{
    MOCI_PROFILE_FUNCTION();

    glfwSwapBuffers(m_WindowHandle);
}

}  // namespace moci