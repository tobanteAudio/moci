#include "button.hpp"

#include "moci_widget/style/style.hpp"

namespace moci
{
Button::Button(std::string text, ButtonSpecs specs) : Component(text), text_(std::move(text)), color_({}), specs_(specs)
{
}

void Button::OnDraw(RenderQueue& painter)
{
    auto* style = GetStyle();
    // If yo hit this assertion, you probably forgot to set the style on the top level component.
    MOCI_CORE_ASSERT(style != nullptr, "Style should not be null");
    style->DrawButton(painter, GetBounds(), *this);
}

}  // namespace moci