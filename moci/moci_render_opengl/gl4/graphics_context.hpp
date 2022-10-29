#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

#include "moci_render_base/moci_render_base.hpp"

struct GLFWwindow;

namespace moci
{

class OpenGLContext : public GraphicsContext
{
public:
    explicit OpenGLContext(GLFWwindow* windowHandle);

    void Init() override;
    void SwapBuffers() override;

private:
    GLFWwindow* m_WindowHandle;
};

}  // namespace moci

#endif