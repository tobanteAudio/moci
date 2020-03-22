#pragma once

#include "moci/render/graphics_context.hpp"

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