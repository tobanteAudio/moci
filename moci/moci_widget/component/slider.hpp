#pragma once

#include "moci_widget/component/component.hpp"

#include <algorithm>

namespace moci
{

class Slider : public Component
{
public:
    class StyleMethods
    {
    public:
        virtual void DrawSlider(RenderQueue& painter, Rectangle<int> bounds, Slider const& slider) = 0;
    };

public:
    Slider(Color col);
    ~Slider() override = default;

    auto OnDraw(RenderQueue& painter) -> void override;

    auto SetValue(float newValue) noexcept -> void { value_ = newValue; }
    [[nodiscard]] auto GetValue() const noexcept -> float { return value_; }

    void SetColor(Color col) noexcept { color_ = std::move(col); }
    [[nodiscard]] Color GetColor() const noexcept { return color_; }

    bool MouseClicked(moci::MouseCallback::Click click) override;
    bool MouseScrolled(MouseScrolledEvent scroll) override;

private:
    float value_ {};
    Color color_ {};
};
}  // namespace moci