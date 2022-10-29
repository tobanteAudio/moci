#pragma once

#include "event.hpp"
#include "moci_events/events/key_codes.hpp"

namespace moci
{

class KeyEvent : public Event
{
public:
    ~KeyEvent() override = default;
    explicit KeyEvent(Key keycode) : m_KeyCode(keycode) { }
    [[nodiscard]] inline auto GetKeyCode() const -> Key { return m_KeyCode; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

private:
    Key m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
    ~KeyPressedEvent() override = default;
    KeyPressedEvent(Key keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) { }

    [[nodiscard]] inline auto GetRepeatCount() const -> int { return m_RepeatCount; }

    [[nodiscard]] auto ToString() const -> std::string override
    {
        return fmt::format("KeyPressedEvent: {} ({} repeats)", GetKeyCode(), m_RepeatCount);
    }

    EVENT_CLASS_TYPE(KeyPressed)
private:
    int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    ~KeyReleasedEvent() override = default;
    explicit KeyReleasedEvent(Key keycode) : KeyEvent(keycode) { }

    [[nodiscard]] auto ToString() const -> std::string override
    {
        return fmt::format("KeyReleasedEvent: {}", GetKeyCode());
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
    ~KeyTypedEvent() override = default;
    explicit KeyTypedEvent(Key keycode) : KeyEvent(keycode) { }

    [[nodiscard]] auto ToString() const -> std::string override
    {
        return fmt::format("KeyTypedEvent: {}", GetKeyCode());
    }

    EVENT_CLASS_TYPE(KeyTyped)
};
}  // namespace moci