#include "graphics_context.hpp"

#include "moci/core/logging.hpp"
#include "moci/render/opengl/es2/es2.hpp"

namespace moci
{

auto GraphicsContext::create(void* win) -> GraphicsContext*
{
    return new OpenGLESContext(static_cast<GLFWwindow*>(win));
}

OpenGLESContext::OpenGLESContext(GLFWwindow* windowHandle) : _m_WindowHandle(windowHandle)
{
    MOCI_CORE_ASSERT(windowHandle, "Window handle is null!")
}

void OpenGLESContext::init()
{
    glfwMakeContextCurrent(_m_WindowHandle);

#if defined(MOCI_API_OPENGL_ES) || defined(MOCI_MAC)
#else
    // Initialize OpenGL loader
    if (auto err = glewInit() != GLEW_OK; err) { MOCI_CORE_ASSERT(err, "Failed to initialize glew!"); }
#endif

    // msaa
    // All don't work on RPi3 Fake/Full KMS OpenGLES 2.0
    // GLCall(glEnable(GL_MULTISAMPLE));
    // GLCall(glEnable(GL_LINE_SMOOTH));
    // GLCall(glEnable(GL_POLYGON_SMOOTH));
    // GLCall(glEnable(GL_POINT_SMOOTH));
    // Enable depth test

    MOCI_CORE_INFO("OpenGL Info:");
    MOCI_CORE_INFO("  Vendor:     {0}", glGetString(GL_VENDOR));
    MOCI_CORE_INFO("  Renderer:   {0}", glGetString(GL_RENDERER));
    MOCI_CORE_INFO("  Version:    {0}", glGetString(GL_VERSION));
}

void OpenGLESContext::swapBuffers() { glfwSwapBuffers(_m_WindowHandle); }

}  // namespace moci