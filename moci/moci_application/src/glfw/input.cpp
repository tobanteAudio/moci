#include "input.hpp"

#include <cmath>

#include "moci_application/src/application.hpp"
// glew is not needed for opengl es 2.0 or on macOS
#if not defined(MOCI_API_OPENGL_ES)
#if not defined(MOCI_MAC)
#include <GL/glew.h>
#endif
#endif

#include <GLFW/glfw3.h>

namespace moci
{

Scope<Input> Input::s_Instance = MakeScope<GlfwInput>();

auto GlfwInput::IsKeyPressedImpl(int keycode) -> bool
{
    auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state   = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

auto GlfwInput::IsMouseButtonPressedImpl(int button) -> bool
{
    auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state   = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

auto GlfwInput::GetMousePositionImpl() -> std::pair<float, float>
{
    auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    double xpos  = NAN;

    double ypos = NAN;
    glfwGetCursorPos(window, &xpos, &ypos);

    return {(float)xpos, (float)ypos};
}

auto GlfwInput::GetMouseXImpl() -> float
{
    auto [x, y] = GetMousePositionImpl();
    return x;
}

auto GlfwInput::GetMouseYImpl() -> float
{
    auto [x, y] = GetMousePositionImpl();
    return y;
}

}  // namespace moci