#include "button.hpp"

#include <utility>

#include "moci/widgets/style/style.hpp"

namespace moci
{
Button::Button(std::string text, ButtonSpecs specs) : Component(text), text_(std::move(text)), specs_(std::move(specs))
{
}

void Button::onDraw(Painter& painter)
{
    auto* style = getStyle();
    MOCI_CORE_ASSERT(style != nullptr, "If yo hit this assertion, you probably forgot"
                                       "to set the style on the top level component.");

    style->drawButton(painter, getBounds(), *this);
}

auto Button::setSpecs(ButtonSpecs newSpecs) -> void { specs_ = std::move(newSpecs); }
auto Button::getSpecs() const noexcept -> ButtonSpecs { return specs_; }

auto Button::setState(ButtonState newState) -> void
{
    state_ = newState;
    if (specs_.callbacks.stateChanged) { specs_.callbacks.stateChanged(newState); }
}
auto Button::getState() const noexcept -> ButtonState { return state_; }

auto Button::setText(std::string newText) -> void { text_ = std::move(newText); }
auto Button::getText() const noexcept -> std::string const& { return text_; }
auto Button::getTextView() const noexcept -> std::string_view { return text_; }

void Button::setTextColor(Color col) noexcept { color_ = col; }
auto Button::getTextColor() const noexcept -> Color { return color_; }

auto Button::mouseClicked(moci::MouseCallback::Click click) -> bool
{
    ignoreUnused(click);
    auto const isNormal = getState() == ButtonState::Normal;
    auto const newState = isNormal ? ButtonState::Down : ButtonState::Normal;
    setState(newState);
    return true;
}
}  // namespace moci