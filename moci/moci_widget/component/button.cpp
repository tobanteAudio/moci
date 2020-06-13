#include "button.hpp"

#include "moci_widget/style/style.hpp"

namespace moci
{
Button::Button(std::string text, ButtonSpecs specs) : Component(text), text_(std::move(text)), color_({}), specs_(specs)
{
}

void Button::OnDraw(Painter& painter)
{
    auto* style = GetStyle();
    MOCI_CORE_ASSERT(style != nullptr, "If yo hit this assertion, you probably forgot"
                                       "to set the style on the top level component.");

    style->DrawButton(painter, GetBounds(), *this);
}

auto Button::SetSpecs(ButtonSpecs newSpecs) -> void { specs_ = std::move(newSpecs); }
auto Button::GetSpecs() const noexcept -> ButtonSpecs const& { return specs_; }

auto Button::SetState(ButtonState newState) -> void
{
    state_ = newState;
    if (specs_.callbacks.onStateChange)
    {
        specs_.callbacks.onStateChange(newState);
    }
}
auto Button::GetState() const noexcept -> ButtonState { return state_; }

auto Button::SetText(std::string newText) -> void { text_ = std::move(newText); }
auto Button::GetText() const noexcept -> std::string const& { return text_; }
auto Button::GetTextView() const noexcept -> std::string_view { return text_; }

void Button::SetTextColor(Color col) noexcept { color_ = std::move(col); }
Color Button::GetTextColor() const noexcept { return color_; }

bool Button::MouseClicked(moci::MouseCallback::Click click)
{
    auto const isNormal = GetState() == ButtonState::Normal;
    auto const newState = isNormal ? ButtonState::Down : ButtonState::Normal;
    SetState(newState);
    return true;
}
}  // namespace moci