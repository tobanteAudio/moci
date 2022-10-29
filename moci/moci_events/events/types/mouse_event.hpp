#pragma once

#include "event.hpp"
#include "moci_events/events/mouse_button_codes.hpp"

#include "moci_core/moci_core.hpp"

namespace moci
{

class MouseMovedEvent : public Event
{
public:
    ~MouseMovedEvent() override = default;
    MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) { }

    [[nodiscard]] inline auto GetX() const -> float { return m_MouseX; }
    [[nodiscard]] inline auto GetY() const -> float { return m_MouseY; }

    [[nodiscard]] auto ToString() const -> std::string override
    {
        return fmt::format("MouseMovedEvent: {}, {}", m_MouseX, m_MouseY);
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    float m_MouseX, m_MouseY;
};

class MouseScrolledEvent : public Event
{
public:
    ~MouseScrolledEvent() override = default;
    MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) { }

    [[nodiscard]] inline auto GetXOffset() const -> float { return m_XOffset; }
    [[nodiscard]] inline auto GetYOffset() const -> float { return m_YOffset; }

    [[nodiscard]] auto ToString() const -> std::string override
    {
        return fmt::format("MouseScrolledEvent: {}, {}", GetXOffset(), GetYOffset());
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    float m_XOffset, m_YOffset;
};

class MouseButtonEvent : public Event
{
public:
    ~MouseButtonEvent() override = default;
    MouseButtonEvent(MouseCode button) : m_Button(button) { }
    [[nodiscard]] inline auto GetMouseButton() const -> MouseCode { return m_Button; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    MouseCode m_Button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    ~MouseButtonPressedEvent() override = default;
    MouseButtonPressedEvent(MouseCode button) : MouseButtonEvent(button) { }

    [[nodiscard]] auto ToString() const -> std::string override
    {
        return fmt::format("MouseButtonPressedEvent: {}", static_cast<int>(GetMouseButton()));
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    ~MouseButtonReleasedEvent() override = default;
    MouseButtonReleasedEvent(MouseCode button) : MouseButtonEvent(button) { }

    [[nodiscard]] auto ToString() const -> std::string override
    {
        return fmt::format("MouseButtonReleasedEvent: {}", static_cast<int>(GetMouseButton()));
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

    virtual bool MouseClicked(Click click)
    {
        IgnoreUnused(click);
        return false;
    }

    virtual bool MouseScrolled(MouseScrolledEvent scroll)
    {
        IgnoreUnused(scroll);
        return false;
    }
};

}  // namespace moci