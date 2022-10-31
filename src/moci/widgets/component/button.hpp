#pragma once

#include <moci/widgets/component/component.hpp>

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

    virtual void drawButton(Painter& painter, Rectangle<int> bounds, Button const& button) = 0;
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

    auto onDraw(Painter& painter) -> void override;

    auto setSpecs(ButtonSpecs newSpecs) -> void;
    [[nodiscard]] auto getSpecs() const noexcept -> ButtonSpecs;

    auto setState(ButtonState newState) -> void;
    [[nodiscard]] auto getState() const noexcept -> ButtonState;

    auto setText(std::string newText) -> void;
    [[nodiscard]] auto getText() const noexcept -> std::string const&;
    [[nodiscard]] auto getTextView() const noexcept -> std::string_view;

    void setTextColor(ColorRGBA32 col) noexcept;
    [[nodiscard]] auto getTextColor() const noexcept -> ColorRGBA32;

    auto mouseClicked(moci::MouseCallback::Click click) -> bool override;

private:
    std::string _text  = {};
    ColorRGBA32 _color = {};
    ButtonSpecs _specs = {};
    ButtonState _state = ButtonState::Normal;
};
}  // namespace moci