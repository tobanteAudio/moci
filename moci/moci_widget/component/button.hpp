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
    std::function<void(ButtonState)> onClick       = {};
    std::function<void(ButtonState)> onStateChange = {};
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

    auto SetSpecs(ButtonSpecs newSpecs) -> void;
    [[nodiscard]] auto GetSpecs() const noexcept -> ButtonSpecs const&;

    auto SetState(ButtonState newState) -> void;
    [[nodiscard]] auto GetState() const noexcept -> ButtonState;

    auto SetText(std::string newText) -> void;
    [[nodiscard]] auto GetText() const noexcept -> std::string const&;
    [[nodiscard]] auto GetTextView() const noexcept -> std::string_view;

    void SetTextColor(Color col) noexcept;
    [[nodiscard]] Color GetTextColor() const noexcept;

    bool MouseClicked(moci::MouseCallback::Click click) override;

private:
    std::string text_  = {};
    Color color_       = {};
    ButtonSpecs specs_ = {};
    ButtonState state_ = ButtonState::Normal;
};
}  // namespace moci