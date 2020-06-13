#include "slider.hpp"

#include "moci_widget/style/style.hpp"

namespace moci
{
Slider::Slider(Color col) : Component("slider"), color_(col) { }

void Slider::OnDraw(RenderQueue& painter)
{
    auto* style = GetStyle();
    // If yo hit this assertion, you probably forgot to set the style on the top level component.
    MOCI_CORE_ASSERT(style != nullptr, "Style should not be null");
    style->DrawSlider(painter, GetBounds(), *this);
}

bool Slider::MouseClicked(moci::MouseCallback::Click click)
{
    auto const bounds    = GetBounds();
    auto const relativeX = static_cast<float>(click.x - bounds.GetX());
    SetValue(relativeX / bounds.GetWidth());
    return true;
}

bool Slider::MouseScrolled(MouseScrolledEvent scroll)
{
    auto newValue = GetValue() + scroll.GetYOffset() * 0.05f;
    newValue      = std::max(0.0f, newValue);
    newValue      = std::min(1.0f, newValue);
    SetValue(newValue);
    return true;
}
}  // namespace moci