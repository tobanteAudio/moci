#pragma once

#include "moci/core/logging.hpp"
#include "moci/events/mouse_button_codes.hpp"
#include "moci/events/types/event.hpp"

namespace moci
{

class MouseMovedEvent : public Event
{
public:
    ~MouseMovedEvent() override = default;
    MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) { }

    [[nodiscard]] inline auto getX() const -> float { return m_MouseX; }
    [[nodiscard]] inline auto getY() const -> float { return m_MouseY; }

    [[nodiscard]] auto toString() const -> std::string override
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

    [[nodiscard]] inline auto getXOffset() const -> float { return m_XOffset; }
    [[nodiscard]] inline auto getYOffset() const -> float { return m_YOffset; }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return fmt::format("MouseScrolledEvent: {}, {}", getXOffset(), getYOffset());
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
    explicit MouseButtonEvent(MouseCode button) : m_Button(button) { }
    [[nodiscard]] inline auto getMouseButton() const -> MouseCode { return m_Button; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    MouseCode m_Button;
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