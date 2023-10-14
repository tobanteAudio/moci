#pragma once

#include <moci/core/logging.hpp>
#include <moci/events/mouse_button_codes.hpp>
#include <moci/events/types/event.hpp>

namespace moci
{

class MouseMovedEvent : public Event
{
public:
    ~MouseMovedEvent() override = default;
    MouseMovedEvent(float x, float y) : _mouseX(x), _mouseY(y) { }

    [[nodiscard]] inline auto getX() const -> float { return _mouseX; }
    [[nodiscard]] inline auto getY() const -> float { return _mouseY; }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return fmt::format("MouseMovedEvent: {}, {}", _mouseX, _mouseY);
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    float _mouseX, _mouseY;
};

class MouseScrolledEvent : public Event
{
public:
    ~MouseScrolledEvent() override = default;
    MouseScrolledEvent(float xOffset, float yOffset) : _xOffset(xOffset), _yOffset(yOffset) { }

    [[nodiscard]] inline auto getXOffset() const -> float { return _xOffset; }
    [[nodiscard]] inline auto getYOffset() const -> float { return _yOffset; }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return fmt::format("MouseScrolledEvent: {}, {}", getXOffset(), getYOffset());
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    float _xOffset, _yOffset;
};

class MouseButtonEvent : public Event
{
public:
    ~MouseButtonEvent() override = default;
    explicit MouseButtonEvent(MouseCode button) : _button(button) { }
    [[nodiscard]] inline auto getMouseButton() const -> MouseCode { return _button; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    MouseCode _button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    ~MouseButtonPressedEvent() override = default;
    explicit MouseButtonPressedEvent(MouseCode button) : MouseButtonEvent(button) { }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return fmt::format("MouseButtonPressedEvent: {}", static_cast<int>(getMouseButton()));
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    ~MouseButtonReleasedEvent() override = default;
    explicit MouseButtonReleasedEvent(MouseCode button) : MouseButtonEvent(button) { }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return fmt::format("MouseButtonReleasedEvent: {}", static_cast<int>(getMouseButton()));
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

class MouseCallback
{
public:
    struct Click
    {
        int x = 0;
        int y = 0;
    };

    virtual ~MouseCallback() = default;

    virtual auto mouseClicked(Click click) -> bool
    {
        ignoreUnused(click);
        return false;
    }

    virtual auto mouseScrolled(MouseScrolledEvent scroll) -> bool
    {
        ignoreUnused(scroll);
        return false;
    }
};

}  // namespace moci
