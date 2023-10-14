#pragma once

#include <moci/render/graphics_context.hpp>

struct GLFWwindow;

namespace moci {

class OpenGLESContext : public GraphicsContext
{
public:
    explicit OpenGLESContext(GLFWwindow* windowHandle);

    void init() override;
    void swapBuffers() override;

private:
    GLFWwindow* _windowHandle;
};

}  // namespace moci
