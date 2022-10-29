#pragma once

#include "moci_render_base/moci_render_base.hpp"

struct GLFWwindow;
namespace moci
{

class OpenGLESContext : public GraphicsContext
{
public:
    explicit OpenGLESContext(GLFWwindow* windowHandle);

    void Init() override;
    void SwapBuffers() override;

private:
    GLFWwindow* m_WindowHandle;
};

}  // namespace moci