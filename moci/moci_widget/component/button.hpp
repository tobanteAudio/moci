#pragma once

#include "moci_widget/component/component.hpp"

#include <algorithm>
#include <string_view>

namespace moci
{
class Button;

class ButtonStyleMethods
{
public:
    virtual void DrawButton(RenderQueue& painter, Rectangle<int> bounds, Button const& Button) = 0;
};

class Button : public Component
{
public:
    Button(std::string text, Color col);
    ~Button() override = default;

    auto OnDraw(RenderQueue& painter) -> void override;

    auto SetText(std::string newText) -> void { text_ = std::move(newText); }
    [[nodiscard]] auto GetText() const noexcept -> std::string const& { return text_; }
    [[nodiscard]] auto GetTextView() const noexcept -> std::string_view { return text_; }

    void SetTextColor(Color col) noexcept { color_ = std::move(col); }
    [[nodiscard]] Color GetTextColor() const noexcept { return color_; }

private:
    std::string text_ {};
    Color color_ {};
};
}  // namespace moci