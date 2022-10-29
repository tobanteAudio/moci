#pragma once

#include "moci/widgets/component/component.hpp"

#include <algorithm>

namespace moci
{
class Label;
class LabelStyleMethods
{
public:
    virtual ~LabelStyleMethods() = default;

    virtual void drawLabel(Painter& painter, Rectangle<int> bounds, Label const& label) = 0;
};

class Label : public Component
{
public:
    Label(std::string text, Color col);
    ~Label() override = default;

    auto onDraw(Painter& painter) -> void override;

    auto setText(std::string newText) -> void { text_ = std::move(newText); }
    [[nodiscard]] auto getText() const noexcept -> std::string const& { return text_; }
    [[nodiscard]] auto getTextView() const noexcept -> std::string_view { return text_; }

    void setTextColor(Color col) noexcept { color_ = col; }
    [[nodiscard]] auto getTextColor() const noexcept -> Color { return color_; }

private:
    std::string text_ {};
    Color color_ {};
};
}  // namespace moci