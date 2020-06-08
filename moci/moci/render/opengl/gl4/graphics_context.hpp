#pragma once

#if defined(MOCI_API_OPENGL_MODERN)

#include "moci_graphics_api_base/moci_graphics_api_base.hpp"

struct GLFWwindow;

namespace moci
{

class OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext(GLFWwindow* windowHandle);

    void Init() override;
    void SwapBuffers() override;

private:
    GLFWwindow* m_WindowHandle;
};

}  // namespace moci

#endif