#include "moci_widget/style/style.hpp"

namespace moci
{
void Style::DrawLabel(RenderQueue& painter, Rectangle<int> bounds, Label const& label)
{
    auto area = bounds;
    painter.DrawText(label.GetText(), {area.GetX(), area.GetY()}, 0.5f, label.GetTextColor());
}
void Style::DrawSlider(RenderQueue& painter, Rectangle<int> bounds, Slider const& slider)
{
    auto area                   = bounds;
    auto const width            = area.GetWidth();
    auto const height           = area.GetHeight();
    auto const border           = std::max<int>(1, static_cast<int>(height * 0.1));
    auto const bottomLeftCorner = Point<int> {area.GetX(), area.GetY()};
    auto const color_           = slider.GetColor();

    // left
    auto const leftEdge = Rectangle<int> {bottomLeftCorner, border, area.GetHeight()};
    painter.DrawQuad(leftEdge.ToFloat(), color_);

    // right
    auto const rightEdge = Rectangle<int> {
        {bottomLeftCorner.GetX() + width - border, bottomLeftCorner.GetY()},  //
        border,                                                               //
        area.GetHeight()                                                      //
    };
    painter.DrawQuad(rightEdge.ToFloat(), color_);

    // bottom
    auto const bottomEdge = Rectangle<int> {bottomLeftCorner, area.GetWidth(), border};
    painter.DrawQuad(bottomEdge.ToFloat(), color_);

    // top
    auto const topEdge = Rectangle<int> {
        {bottomLeftCorner.GetX(), bottomLeftCorner.GetY() + height - border},  //
        width,                                                                 //
        border                                                                 //
    };
    painter.DrawQuad(topEdge.ToFloat(), color_);

    // infill
    auto const inFillWidth
        = static_cast<int>(static_cast<float>(width - border * 4) * std::min(slider.GetValue(), 1.0f));
    auto const inFill = Rectangle<int> {
        {bottomLeftCorner.GetX() + (border * 2), bottomLeftCorner.GetY() + (border * 2)},  //
        inFillWidth,                                                                       //
        height - (border * 4)                                                              //
    };
    painter.DrawQuad(inFill.ToFloat(), color_);
}
}  // namespace moci