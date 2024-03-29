#include "input.hpp"

#include <moci/application/application.hpp>

#if not defined(MOCI_API_OPENGL_ES)
    #include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <cmath>

namespace moci {

std::unique_ptr<Input> Input::sInstance = std::make_unique<GlfwInput>();

auto GlfwInput::isKeyPressedImpl(Key keycode) -> bool
{
    auto* window
        = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
    auto state = glfwGetKey(window, static_cast<int>(keycode));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

auto GlfwInput::isMouseButtonPressedImpl(MouseCode button) -> bool
{
    auto* window
        = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
    auto state = glfwGetMouseButton(window, static_cast<int>(button));
    return state == GLFW_PRESS;
}

auto GlfwInput::getMousePositionImpl() -> std::pair<float, float>
{
    auto* window
        = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
    double xpos = NAN;

    double ypos = NAN;
    glfwGetCursorPos(window, &xpos, &ypos);

    return {(float)xpos, (float)ypos};
}

auto GlfwInput::getMouseXImpl() -> float
{
    auto [x, y] = getMousePositionImpl();
    return x;
}

auto GlfwInput::getMouseYImpl() -> float
{
    auto [x, y] = getMousePositionImpl();
    return y;
}

}  // namespace moci
