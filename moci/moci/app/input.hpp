#pragma once

#include "moci/core/memory.hpp"

#include <tuple>

namespace moci
{

class Input
{
public:
    Input()             = default;
    Input(const Input&) = delete;
    auto operator=(const Input&) -> Input& = delete;
    virtual ~Input()                       = default;

    inline static auto IsKeyPressed(int keycode) -> bool { return s_Instance->IsKeyPressedImpl(keycode); }

    inline static auto IsMouseButtonPressed(int button) -> bool { return s_Instance->IsMouseButtonPressedImpl(button); }
    inline static auto GetMousePosition() -> std::pair<float, float> { return s_Instance->GetMousePositionImpl(); }
    inline static auto GetMouseX() -> float { return s_Instance->GetMouseXImpl(); }
    inline static auto GetMouseY() -> float { return s_Instance->GetMouseYImpl(); }

private:
    virtual auto IsKeyPressedImpl(int keycode) -> bool             = 0;
    virtual auto IsMouseButtonPressedImpl(int button) -> bool      = 0;
    virtual auto GetMousePositionImpl() -> std::pair<float, float> = 0;
    virtual auto GetMouseXImpl() -> float                          = 0;
    virtual auto GetMouseYImpl() -> float                          = 0;

private:
    static Scope<Input> s_Instance;
};
}  // namespace moci