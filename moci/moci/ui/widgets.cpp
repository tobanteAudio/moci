#include "widgets.hpp"

#include "moci/ui/style.hpp"

namespace moci
{
Label::Label(std::string text, Color col) : Component("label"), text_(std::move(text)), color_(col) { }

void Label::OnDraw(RenderQueue& painter)
{
    auto style = GetStyle();
    // If yo hit this assertion, you probably forgot to set the style on the top level component.
    MOCI_CORE_ASSERT(style != nullptr, "Style should not be null");
    style->DrawLabel(painter, GetBounds(), *this);
}

Slider::Slider(Color col) : Component("slider"), color_(col) { }

void Slider::OnDraw(RenderQueue& painter)
{
    auto style = GetStyle();
    // If yo hit this assertion, you probably forgot to set the style on the top level component.
    MOCI_CORE_ASSERT(style != nullptr, "Style should not be null");
    style->DrawSlider(painter, GetBounds(), *this);
}
}  // namespace moci