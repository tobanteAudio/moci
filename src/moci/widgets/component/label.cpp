#include "label.hpp"

#include "moci/widgets/style/style.hpp"

namespace moci
{
Label::Label(std::string text, Color col) : Component("label"), text_(std::move(text)), color_(col) { }

void Label::onDraw(Painter& painter)
{
    auto* style = getStyle();
    // If yo hit this assertion, you probably forgot to set the style on the top level component.
    MOCI_CORE_ASSERT(style != nullptr, "Style should not be null");
    style->drawLabel(painter, getBounds(), *this);
}

}  // namespace moci