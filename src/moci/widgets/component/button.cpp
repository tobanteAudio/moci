#include "button.hpp"

#include <utility>

#include <moci/widgets/style/style.hpp>

namespace moci {
Button::Button(std::string text, ButtonSpecs specs)
    : Component(text)
    , _text(std::move(text))
    , _specs(std::move(specs))
{}

void Button::onDraw(Painter& painter)
{
    auto* style = getStyle();
    MOCI_CORE_ASSERT(
        style != nullptr,
        "If yo hit this assertion, you probably forgot"
        "to set the style on the top level component."
    );

    style->drawButton(painter, getBounds(), *this);
}

auto Button::setSpecs(ButtonSpecs newSpecs) -> void { _specs = std::move(newSpecs); }

auto Button::getSpecs() const noexcept -> ButtonSpecs { return _specs; }

auto Button::setState(ButtonState newState) -> void
{
    _state = newState;
    if (_specs.callbacks.stateChanged) {
        _specs.callbacks.stateChanged(newState);
    }
}

auto Button::getState() const noexcept -> ButtonState { return _state; }

auto Button::setText(std::string newText) -> void { _text = std::move(newText); }

auto Button::getText() const noexcept -> std::string const& { return _text; }

auto Button::getTextView() const noexcept -> std::string_view { return _text; }

void Button::setTextColor(ColorRGBA32 col) noexcept { _color = col; }

auto Button::getTextColor() const noexcept -> ColorRGBA32 { return _color; }

auto Button::mouseClicked(moci::MouseCallback::Click click) -> bool
{
    ignoreUnused(click);
    auto const isNormal = getState() == ButtonState::Normal;
    auto const newState = isNormal ? ButtonState::Down : ButtonState::Normal;
    setState(newState);
    return true;
}
}  // namespace moci
