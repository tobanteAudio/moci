#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

    #include <moci/render/graphics_context.hpp>

struct GLFWwindow;

namespace moci {

class OpenGLContext : public GraphicsContext
{
public:
    explicit OpenGLContext(GLFWwindow* windowHandle);

    void init() override;
    void swapBuffers() override;

private:
    GLFWwindow* _windowHandle;
};

}  // namespace moci

#endif
