#pragma once

#include <moci/widgets/component/component.hpp>

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
    Label(std::string text, ColorRGBA32 col);
    ~Label() override = default;

    auto onDraw(Painter& painter) -> void override;

    auto setText(std::string newText) -> void { _text = std::move(newText); }
    [[nodiscard]] auto getText() const noexcept -> std::string const& { return _text; }
    [[nodiscard]] auto getTextView() const noexcept -> std::string_view { return _text; }

    void setTextColor(ColorRGBA32 col) noexcept { _color = col; }
    [[nodiscard]] auto getTextColor() const noexcept -> ColorRGBA32 { return _color; }

private:
    std::string _text {};
    ColorRGBA32 _color {};
};
}  // namespace moci