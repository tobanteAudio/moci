#include "label.hpp"

#include "moci/widgets/style/style.hpp"

namespace moci
{
Label::Label(std::string text, Color col) : Component("label"), text_(std::move(text)), color_(col) { }

void Label::OnDraw(Painter& painter)
{
    auto* style = GetStyle();
    // If yo hit this assertion, you probably forgot to set the style on the top level component.
    MOCI_CORE_ASSERT(style != nullptr, "Style should not be null");
    style->DrawLabel(painter, GetBounds(), *this);
}

}  // namespace moci