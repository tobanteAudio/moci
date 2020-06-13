#pragma once

#include "moci_widget/widgets/component.hpp"

#include <algorithm>

namespace moci
{

class Label : public Component
{
public:
    class StyleMethods
    {
    public:
        virtual void DrawLabel(RenderQueue& painter, Rectangle<int> bounds, Label const& label) = 0;
    };

public:
    Label(std::string text, Color col);
    ~Label() override = default;

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

class Slider : public Component
{
public:
    class StyleMethods
    {
    public:
        virtual void DrawSlider(RenderQueue& painter, Rectangle<int> bounds, Slider const& slider) = 0;
    };

public:
    Slider(Color col);
    ~Slider() override = default;

    auto OnDraw(RenderQueue& painter) -> void override;

    auto SetValue(float newValue) noexcept -> void { value_ = newValue; }
    [[nodiscard]] auto GetValue() const noexcept -> float { return value_; }

    void SetColor(Color col) noexcept { color_ = std::move(col); }
    [[nodiscard]] Color GetColor() const noexcept { return color_; }

    bool MouseClicked(moci::MouseCallback::Click click) override
    {
        auto const bounds    = GetBounds();
        auto const relativeX = static_cast<float>(click.x - bounds.GetX());
        SetValue(relativeX / bounds.GetWidth());
        return true;
    }

private:
    float value_ {};
    Color color_ {};
};
}  // namespace moci