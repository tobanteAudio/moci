#include "slider.hpp"

#include "moci/widgets/style/style.hpp"

namespace moci
{
Slider::Slider(Color col, SliderCallbacks callbacks)
    : Component("slider"), callbacks_(std::move(callbacks)), color_(col)
{
}

void Slider::OnDraw(Painter& painter)
{
    auto* style = GetStyle();
    // If yo hit this assertion, you probably forgot to set the style on the top level component.
    MOCI_CORE_ASSERT(style != nullptr, "Style should not be null");
    style->DrawSlider(painter, GetBounds(), *this);
}

auto Slider::MouseClicked(moci::MouseCallback::Click click) -> bool
{
    auto const bounds    = GetBounds();
    auto const relativeX = static_cast<float>(click.x - bounds.GetX());
    SetValue(relativeX / bounds.GetWidth());
    return true;
}

auto Slider::MouseScrolled(MouseScrolledEvent scroll) -> bool
{
    auto newValue = GetValue() + scroll.GetYOffset() * 0.05F;
    newValue      = std::max(0.0F, newValue);
    newValue      = std::min(1.0F, newValue);
    SetValue(newValue);
    return true;
}

auto Slider::GetValue() const noexcept -> float { return value_; }

auto Slider::SetValue(float newValue) noexcept -> void
{
    value_ = newValue;
    if (callbacks_.valueChanged) { callbacks_.valueChanged(newValue); }
}

auto Slider::GetColor() const noexcept -> Color { return color_; }
void Slider::SetColor(Color col) noexcept { color_ = col; }

}  // namespace moci