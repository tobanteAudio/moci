#pragma once

#include <moci/widgets/component/component.hpp>

#include <algorithm>
#include <functional>

namespace moci
{

class Slider;
struct SliderCallbacks
{
    std::function<void(float)> valueChanged = {};
};

class SliderStyleMethods
{
public:
    virtual ~SliderStyleMethods() = default;

    virtual void drawSlider(Painter& painter, Rectangle<int> bounds, Slider const& slider) = 0;
};

class Slider : public Component
{
public:
    explicit Slider(ColorRGBA32 col, SliderCallbacks callbacks = SliderCallbacks {});
    ~Slider() override = default;

    auto onDraw(Painter& painter) -> void override;

    auto setValue(float newValue) noexcept -> void;
    [[nodiscard]] auto getValue() const noexcept -> float;

    void setColor(ColorRGBA32 col) noexcept;
    [[nodiscard]] auto getColor() const noexcept -> ColorRGBA32;

    auto mouseClicked(moci::MouseCallback::Click click) -> bool override;
    auto mouseScrolled(MouseScrolledEvent scroll) -> bool override;

private:
    float _value               = {};
    SliderCallbacks _callbacks = {};
    ColorRGBA32 _color         = {};
};
}  // namespace moci
