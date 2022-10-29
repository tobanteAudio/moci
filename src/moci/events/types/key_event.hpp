#pragma once

#include "moci/events/key_codes.hpp"
#include "moci/events/types/event.hpp"

namespace moci
{

class KeyEvent : public Event
{
public:
    ~KeyEvent() override = default;
    explicit KeyEvent(Key keycode) : _m_KeyCode(keycode) { }
    [[nodiscard]] inline auto getKeyCode() const -> Key { return _m_KeyCode; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

private:
    Key _m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
    ~KeyPressedEvent() override = default;
    KeyPressedEvent(Key keycode, int repeatCount) : KeyEvent(keycode), _m_RepeatCount(repeatCount) { }

    [[nodiscard]] inline auto getRepeatCount() const -> int { return _m_RepeatCount; }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return fmt::format("KeyPressedEvent: {} ({} repeats)", getKeyCode(), _m_RepeatCount);
    }

    EVENT_CLASS_TYPE(KeyPressed)
private:
    int _m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    ~KeyReleasedEvent() override = default;
    explicit KeyReleasedEvent(Key keycode) : KeyEvent(keycode) { }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return fmt::format("KeyReleasedEvent: {}", getKeyCode());
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
    ~KeyTypedEvent() override = default;
    explicit KeyTypedEvent(Key keycode) : KeyEvent(keycode) { }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return fmt::format("KeyTypedEvent: {}", getKeyCode());
    }

    EVENT_CLASS_TYPE(KeyTyped)
};
}  // namespace moci