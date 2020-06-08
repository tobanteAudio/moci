#pragma once

#include "moci_graphics_api_base/moci_graphics_api_base.hpp"

struct GLFWwindow;
namespace moci
{

class OpenGLESContext : public GraphicsContext
{
public:
    OpenGLESContext(GLFWwindow* windowHandle);

    void Init() override;
    void SwapBuffers() override;

private:
    GLFWwindow* m_WindowHandle;
};

}  // namespace moci