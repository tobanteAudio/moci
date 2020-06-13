#pragma once

#include "moci_widget/component/component.hpp"

#include <algorithm>

namespace moci
{

class Label : public Component
{
public:
    class StyleMethods
    {
    public:
        virtual void DrawLabel(Painter& painter, Rectangle<int> bounds, Label const& label) = 0;
    };

public:
    Label(std::string text, Color col);
    ~Label() override = default;

    auto OnDraw(Painter& painter) -> void override;

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