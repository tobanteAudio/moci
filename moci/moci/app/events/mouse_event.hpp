#pragma once

#include "event.hpp"

#include <sstream>

namespace moci
{

class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

    [[nodiscard]] inline auto GetX() const -> float { return m_MouseX; }
    [[nodiscard]] inline auto GetY() const -> float { return m_MouseY; }

    [[nodiscard]] auto ToString() const -> std::string override
    {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    float m_MouseX, m_MouseY;
};

class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

    [[nodiscard]] inline auto GetXOffset() const -> float { return m_XOffset; }
    [[nodiscard]] inline auto GetYOffset() const -> float { return m_YOffset; }

    [[nodiscard]] auto ToString() const -> std::string override
    {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    float m_XOffset, m_YOffset;
};

class MouseButtonEvent : public Event
{
public:
    MouseButtonEvent(int button) : m_Button(button) {}
    [[nodiscard]] inline auto GetMouseButton() const -> int { return m_Button; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    int m_Button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    [[nodiscard]] auto ToString() const -> std::string override
    {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << GetMouseButton();
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    [[nodiscard]] auto ToString() const -> std::string override
    {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << GetMouseButton();
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

}  // namespace moci