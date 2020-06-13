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
}  // namespace moci