#include "moci/widgets/style/style.hpp"

namespace moci
{
void Style::drawButton(Painter& painter, Rectangle<int> bounds, Button const& button)
{
    auto area                   = bounds;
    auto const width            = area.getWidth();
    auto const height           = area.getHeight();
    auto const border           = std::max<int>(1, static_cast<int>(height * 0.1));
    auto const bottomLeftCorner = Point<int> {area.getX(), area.getY()};
    auto const color            = button.getTextColor();

    // left
    auto const leftEdge = Rectangle<int> {bottomLeftCorner, border, area.getHeight()};
    painter.drawQuad(leftEdge.toFloat(), color);

    // right
    auto const rightEdge = Rectangle<int> {
        {bottomLeftCorner.getX() + width - border, bottomLeftCorner.getY()},  //
        border,                                                               //
        area.getHeight()                                                      //
    };
    painter.drawQuad(rightEdge.toFloat(), color);

    // bottom
    auto const bottomEdge = Rectangle<int> {bottomLeftCorner, area.getWidth(), border};
    painter.drawQuad(bottomEdge.toFloat(), color);

    // top
    auto const topEdge = Rectangle<int> {
        {bottomLeftCorner.getX(), bottomLeftCorner.getY() + height - border},  //
        width,                                                                 //
        border                                                                 //
    };
    painter.drawQuad(topEdge.toFloat(), color);

    // text
    painter.drawText(button.getText(), {bounds.getX() + width / 2, bounds.getY() + height}, 0.5F, color);
}

void Style::drawLabel(Painter& painter, Rectangle<int> bounds, Label const& label)
{
    auto area = bounds;
    painter.drawText(label.getText(), {area.getX(), area.getY()}, 0.5F, label.getTextColor());
}

void Style::drawSlider(Painter& painter, Rectangle<int> bounds, Slider const& slider)
{
    auto area                   = bounds;
    auto const width            = area.getWidth();
    auto const height           = area.getHeight();
    auto const border           = std::max<int>(1, static_cast<int>(height * 0.1));
    auto const bottomLeftCorner = Point<int> {area.getX(), area.getY()};
    auto const color            = slider.getColor();

    // left
    auto const leftEdge = Rectangle<int> {bottomLeftCorner, border, area.getHeight()};
    painter.drawQuad(leftEdge.toFloat(), color);

    // right
    auto const rightEdge = Rectangle<int> {
        {bottomLeftCorner.getX() + width - border, bottomLeftCorner.getY()},  //
        border,                                                               //
        area.getHeight()                                                      //
    };
    painter.drawQuad(rightEdge.toFloat(), color);

    // bottom
    auto const bottomEdge = Rectangle<int> {bottomLeftCorner, area.getWidth(), border};
    painter.drawQuad(bottomEdge.toFloat(), color);

    // top
    auto const topEdge = Rectangle<int> {
        {bottomLeftCorner.getX(), bottomLeftCorner.getY() + height - border},  //
        width,                                                                 //
        border                                                                 //
    };
    painter.drawQuad(topEdge.toFloat(), color);

    // infill
    auto const inFillWidth
        = static_cast<int>(static_cast<float>(width - border * 4) * std::min(slider.getValue(), 1.0F));
    auto const inFill = Rectangle<int> {
        {bottomLeftCorner.getX() + (border * 2), bottomLeftCorner.getY() + (border * 2)},  //
        inFillWidth,                                                                       //
        height - (border * 4)                                                              //
    };
    painter.drawQuad(inFill.toFloat(), color);
}
}  // namespace moci