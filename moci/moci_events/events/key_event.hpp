#pragma once

#include "event.hpp"
#include "moci_events/key_codes.hpp"

namespace moci
{

class KeyEvent : public Event
{
public:
    KeyEvent(Key keycode) : m_KeyCode(keycode) { }
    [[nodiscard]] inline auto GetKeyCode() const -> Key { return m_KeyCode; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

private:
    Key m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(Key keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) { }

    [[nodiscard]] inline auto GetRepeatCount() const -> int { return m_RepeatCount; }

    [[nodiscard]] auto ToString() const -> std::string override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << GetKeyCode() << " (" << m_RepeatCount << " repeats)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)
private:
    int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(Key keycode) : KeyEvent(keycode) { }

    [[nodiscard]] auto ToString() const -> std::string override
    {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << GetKeyCode();
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
    KeyTypedEvent(Key keycode) : KeyEvent(keycode) { }

    [[nodiscard]] auto ToString() const -> std::string override
    {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << GetKeyCode();
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
};
}  // namespace moci