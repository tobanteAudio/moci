#pragma once

#include <moci/core/logging.hpp>
#include <moci/events/key_codes.hpp>
#include <moci/events/mouse_button_codes.hpp>

#include <utility>

namespace moci
{

class Input
{
public:
    Input()                                = default;
    Input(const Input&)                    = delete;
    auto operator=(const Input&) -> Input& = delete;
    virtual ~Input()                       = default;

    inline static auto isKeyPressed(Key keycode) -> bool { return sInstance->isKeyPressedImpl(keycode); }

    inline static auto isMouseButtonPressed(MouseCode button) -> bool
    {
        return sInstance->isMouseButtonPressedImpl(button);
    }
    inline static auto getMousePosition() -> std::pair<float, float> { return sInstance->getMousePositionImpl(); }
    inline static auto getMouseX() -> float { return sInstance->getMouseXImpl(); }
    inline static auto getMouseY() -> float { return sInstance->getMouseYImpl(); }

private:
    virtual auto isKeyPressedImpl(Key keycode) -> bool              = 0;
    virtual auto isMouseButtonPressedImpl(MouseCode button) -> bool = 0;
    virtual auto getMousePositionImpl() -> std::pair<float, float>  = 0;
    virtual auto getMouseXImpl() -> float                           = 0;
    virtual auto getMouseYImpl() -> float                           = 0;

    static Scope<Input> sInstance;
};
}  // namespace moci