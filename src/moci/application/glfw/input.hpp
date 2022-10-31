#pragma once

#include <moci/events/input.hpp>
#include <moci/events/types/event.hpp>

#include <tuple>

namespace moci
{

class GlfwInput : public Input
{
public:
    ~GlfwInput() override = default;

private:
    auto isKeyPressedImpl(Key keycode) -> bool override;

    auto isMouseButtonPressedImpl(MouseCode button) -> bool override;
    auto getMousePositionImpl() -> std::pair<float, float> override;
    auto getMouseXImpl() -> float override;
    auto getMouseYImpl() -> float override;
};

}  // namespace moci