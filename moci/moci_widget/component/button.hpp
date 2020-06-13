#pragma once

#include "moci_widget/component/component.hpp"

#include <algorithm>
#include <string_view>

namespace moci
{
class Button;

enum class ButtonState
{
    Normal,
    Over,
    Down,
};

class ButtonStyleMethods
{
public:
    virtual void DrawButton(Painter& painter, Rectangle<int> bounds, Button const& Button) = 0;
};

struct ButtonCallbacks
{
    std::function<void(ButtonState)> onClick = {};
};

struct ButtonSpecs
{
    ButtonCallbacks callbacks = {};
    bool isToggle             = false;
};

class Button : public Component
{
public:
    Button(std::string text, ButtonSpecs specs = ButtonSpecs {});
    ~Button() override = default;

    auto OnDraw(Painter& painter) -> void override;

    auto SetSpecs(ButtonSpecs newSpecs) -> void { specs_ = std::move(newSpecs); }
    [[nodiscard]] auto GetSpecs() const noexcept -> ButtonSpecs const& { return specs_; }

    auto SetState(ButtonState newState) -> void { state_ = std::move(newState); }
    [[nodiscard]] auto GetState() const noexcept -> ButtonState { return state_; }

    auto SetText(std::string newText) -> void { text_ = std::move(newText); }
    [[nodiscard]] auto GetText() const noexcept -> std::string const& { return text_; }
    [[nodiscard]] auto GetTextView() const noexcept -> std::string_view { return text_; }

    void SetTextColor(Color col) noexcept { color_ = std::move(col); }
    [[nodiscard]] Color GetTextColor() const noexcept { return color_; }

private:
    std::string text_  = {};
    Color color_       = {};
    ButtonSpecs specs_ = {};
    ButtonState state_ = ButtonState::Normal;
};
}  // namespace moci