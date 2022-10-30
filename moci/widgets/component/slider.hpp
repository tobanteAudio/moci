#pragma once

#include "moci/widgets/component/component.hpp"

#include <algorithm>
#include <functional>

namespace moci
{

class Slider;
struct SliderCallbacks
{
    std::function<void(float)> valueChanged = {};
};

class SliderStyleMethods
{
public:
    virtual ~SliderStyleMethods() = default;

    virtual void DrawSlider(Painter& painter, Rectangle<int> bounds, Slider const& slider) = 0;
};

class Slider : public Component
{
public:
    explicit Slider(Color col, SliderCallbacks callbacks = SliderCallbacks {});
    ~Slider() override = default;

    auto OnDraw(Painter& painter) -> void override;

    auto SetValue(float newValue) noexcept -> void;
    [[nodiscard]] auto GetValue() const noexcept -> float;

    void SetColor(Color col) noexcept;
    [[nodiscard]] auto GetColor() const noexcept -> Color;

    auto MouseClicked(moci::MouseCallback::Click click) -> bool override;
    auto MouseScrolled(MouseScrolledEvent scroll) -> bool override;

private:
    float value_               = {};
    SliderCallbacks callbacks_ = {};
    Color color_               = {};
};
}  // namespace moci