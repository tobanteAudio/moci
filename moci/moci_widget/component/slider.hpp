#pragma once

#include "moci_widget/component/component.hpp"

#include <algorithm>
#include <functional>

namespace moci
{

struct SliderCallbacks
{
    std::function<void(float)> valueChanged = {};
};

class Slider : public Component
{
public:
    class StyleMethods
    {
    public:
        virtual void DrawSlider(Painter& painter, Rectangle<int> bounds, Slider const& slider) = 0;
    };

public:
    Slider(Color col, SliderCallbacks callbacks = SliderCallbacks {});
    ~Slider() override = default;

    auto OnDraw(Painter& painter) -> void override;

    auto SetValue(float newValue) noexcept -> void;
    [[nodiscard]] auto GetValue() const noexcept -> float;

    void SetColor(Color col) noexcept;
    [[nodiscard]] Color GetColor() const noexcept;

    bool MouseClicked(moci::MouseCallback::Click click) override;
    bool MouseScrolled(MouseScrolledEvent scroll) override;

private:
    float value_               = {};
    SliderCallbacks callbacks_ = {};
    Color color_               = {};
};
}  // namespace moci