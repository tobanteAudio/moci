#pragma once

#include "moci/render/graphics_context.hpp"

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