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
    virtual ~ButtonStyleMethods() = default;

    virtual void DrawButton(Painter& painter, Rectangle<int> bounds, Button const& Button) = 0;
};

struct ButtonCallbacks
{
    std::function<void(ButtonState)> clicked      = {};
    std::function<void(ButtonState)> stateChanged = {};
};

struct ButtonSpecs
{
    ButtonCallbacks callbacks = {};
    bool isToggle             = false;
};

class Button : public Component
{
public:
    explicit Button(std::string text, ButtonSpecs specs = ButtonSpecs {});
    ~Button() override = default;

    auto OnDraw(Painter& painter) -> void override;

    auto SetSpecs(ButtonSpecs newSpecs) -> void;
    [[nodiscard]] auto GetSpecs() const noexcept -> ButtonSpecs;

    auto SetState(ButtonState newState) -> void;
    [[nodiscard]] auto GetState() const noexcept -> ButtonState;

    auto SetText(std::string newText) -> void;
    [[nodiscard]] auto GetText() const noexcept -> std::string const&;
    [[nodiscard]] auto GetTextView() const noexcept -> std::string_view;

    void SetTextColor(Color col) noexcept;
    [[nodiscard]] auto GetTextColor() const noexcept -> Color;

    auto MouseClicked(moci::MouseCallback::Click click) -> bool override;

private:
    std::string text_  = {};
    Color color_       = {};
    ButtonSpecs specs_ = {};
    ButtonState state_ = ButtonState::Normal;
};
}  // namespace moci