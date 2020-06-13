#pragma once

#include "moci_widget/component/label.hpp"
#include "moci_widget/component/slider.hpp"

namespace moci
{
class Style
    : public Label::StyleMethods
    , public Slider::StyleMethods
{
public:
    Style() = default;

public:
    void DrawLabel(RenderQueue& painter, Rectangle<int> bounds, Label const& label) override;
    void DrawSlider(RenderQueue& painter, Rectangle<int> bounds, Slider const& slider) override;
};
}  // namespace moci