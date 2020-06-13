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

    bool MouseClicked(moci::MouseCallback::Click click) override
    {
        auto const bounds    = GetBounds();
        auto const relativeX = static_cast<float>(click.x - bounds.GetX());
        SetValue(relativeX / bounds.GetWidth());
        return true;
    }

    bool MouseScrolled(MouseScrolledEvent scroll) override
    {
        auto newValue = GetValue() + scroll.GetYOffset() * 0.05f;
        newValue      = std::max(0.0f, newValue);
        newValue      = std::min(1.0f, newValue);
        SetValue(newValue);
        return true;
    }

private:
    float value_ {};
    Color color_ {};
};
}  // namespace moci