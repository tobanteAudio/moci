#pragma once

#include "moci/events/input.hpp"
#include "moci/events/types/event.hpp"

#include <tuple>

namespace moci
{

class GlfwInput : public Input
{
public:
    ~GlfwInput() override = default;

private:
    auto IsKeyPressedImpl(Key keycode) -> bool override;

    auto IsMouseButtonPressedImpl(MouseCode button) -> bool override;
    auto GetMousePositionImpl() -> std::pair<float, float> override;
    auto GetMouseXImpl() -> float override;
    auto GetMouseYImpl() -> float override;
};

}  // namespace moci