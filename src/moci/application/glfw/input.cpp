#include "input.hpp"

#include <cmath>

#include "moci/application/application.hpp"

// glew is not needed for opengl es 2.0 or on macOS
#if not defined(MOCI_API_OPENGL_ES)
#if not defined(MOCI_MAC)
#include <GL/glew.h>
#endif
#endif

#include <GLFW/glfw3.h>

namespace moci
{

Scope<Input> Input::sInstance = makeScope<GlfwInput>();

auto GlfwInput::isKeyPressedImpl(Key keycode) -> bool
{
    auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
    auto state   = glfwGetKey(window, static_cast<int>(keycode));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

auto GlfwInput::isMouseButtonPressedImpl(MouseCode button) -> bool
{
    auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
    auto state   = glfwGetMouseButton(window, static_cast<int>(button));
    return state == GLFW_PRESS;
}

auto GlfwInput::getMousePositionImpl() -> std::pair<float, float>
{
    auto* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
    double xpos  = NAN;

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