#pragma once

#include "moci_events/moci_events.hpp"

#include <tuple>

namespace moci
{

class GlfwInput : public Input
{
public:
    ~GlfwInput() override = default;

private:
    auto IsKeyPressedImpl(int keycode) -> bool override;

    auto IsMouseButtonPressedImpl(int button) -> bool override;
    auto GetMousePositionImpl() -> std::pair<float, float> override;
    auto GetMouseXImpl() -> float override;
    auto GetMouseYImpl() -> float override;
};

}  // namespace moci