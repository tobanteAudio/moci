#include "input.hpp"

#include "moci/app/application.hpp"
#include "moci/render/opengl/es2/es2.hpp"

namespace moci
{

Scope<Input> Input::s_Instance = MakeScope<GlfwInput>();

auto GlfwInput::IsKeyPressedImpl(int keycode) -> bool
{
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state  = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

auto GlfwInput::IsMouseButtonPressedImpl(int button) -> bool
{
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state  = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

auto GlfwInput::GetMousePositionImpl() -> std::pair<float, float>
{
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    double xpos;

    double ypos;
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