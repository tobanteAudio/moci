#include "graphics_context.hpp"

#if defined(MOCI_API_OPENGL_MODERN)

    #include "gl4.hpp"
    #include <moci/core/benchmark/profile.hpp>
    #include <moci/core/logging.hpp>

namespace moci {

auto GraphicsContext::create(void* win) -> GraphicsContext*
{
    return new OpenGLContext(static_cast<GLFWwindow*>(win));
}

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : _windowHandle(windowHandle)
{
    MOCI_CORE_ASSERT(windowHandle, "Window handle is null!")
}

void OpenGLContext::init()
{
    MOCI_PROFILE_FUNCTION();

    glfwMakeContextCurrent(_windowHandle);
    // Initialize OpenGL loader
    if (auto err = glewInit() != GLEW_OK; err) {
        MOCI_CORE_ASSERT(err, "Failed to initialize glew!");
    }

    MOCI_CORE_INFO("OpenGL Info:");
    MOCI_CORE_INFO(
        "  Vendor: {0}",
        std::string_view{reinterpret_cast<char const*>(glGetString(GL_VENDOR))}
    );
    MOCI_CORE_INFO(
        "  Renderer: {0}",
        std::string_view{reinterpret_cast<char const*>(glGetString(GL_RENDERER))}
    );
    MOCI_CORE_INFO(
        "  Version: {0}",
        std::string_view{reinterpret_cast<char const*>(glGetString(GL_VERSION))}
    );

    int versionMajor = 0;
    int versionMinor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
    glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

    MOCI_CORE_ASSERT(
        versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5),
        "Moci requires at least OpenGL version 4.5!"
    );
}

void OpenGLContext::swapBuffers()
{
    MOCI_PROFILE_FUNCTION();

    glfwSwapBuffers(_windowHandle);
}

}  // namespace moci

#endif
