#include "slider.hpp"

#include <moci/widgets/style/style.hpp>

namespace moci {
Slider::Slider(ColorRGBA32 col, SliderCallbacks callbacks)
    : Component("slider")
    , _callbacks(std::move(callbacks))
    , _color(col)
{}

void Slider::onDraw(Painter& painter)
{
    auto* style = getStyle();
    // If yo hit this assertion, you probably forgot to set the style on the top level
    // component.
    MOCI_CORE_ASSERT(style != nullptr, "Style should not be null");
    style->drawSlider(painter, getBounds(), *this);
}

auto Slider::mouseClicked(moci::MouseCallback::Click click) -> bool
{
    auto const bounds    = getBounds();
    auto const relativeX = static_cast<float>(click.x - bounds.getX());
    setValue(relativeX / bounds.getWidth());
    return true;
}

auto Slider::mouseScrolled(MouseScrolledEvent scroll) -> bool
{
    auto newValue = getValue() + scroll.getYOffset() * 0.05F;
    newValue      = std::max(0.0F, newValue);
    newValue      = std::min(1.0F, newValue);
    setValue(newValue);
    return true;
}

auto Slider::getValue() const noexcept -> float { return _value; }

auto Slider::setValue(float newValue) noexcept -> void
{
    _value = newValue;
    if (_callbacks.valueChanged) {
        _callbacks.valueChanged(newValue);
    }
}

auto Slider::getColor() const noexcept -> ColorRGBA32 { return _color; }

void Slider::setColor(ColorRGBA32 col) noexcept { _color = col; }

}  // namespace moci
