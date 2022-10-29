#pragma once

#include "moci/widgets/component/button.hpp"
#include "moci/widgets/component/label.hpp"
#include "moci/widgets/component/slider.hpp"

namespace moci
{
class Style
    : public ButtonStyleMethods
    , public LabelStyleMethods
    , public SliderStyleMethods
{
public:
    Style()           = default;
    ~Style() override = default;

    void drawButton(Painter& painter, Rectangle<int> bounds, Button const& button) override;
    void drawLabel(Painter& painter, Rectangle<int> bounds, Label const& label) override;
    void drawSlider(Painter& painter, Rectangle<int> bounds, Slider const& slider) override;
};
}  // namespace moci