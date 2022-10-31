#pragma once

#include <moci/events/key_codes.hpp>
#include <moci/events/types/event.hpp>

namespace moci
{

class KeyEvent : public Event
{
public:
    ~KeyEvent() override = default;
    explicit KeyEvent(Key keycode) : _keyCode(keycode) { }
    [[nodiscard]] inline auto getKeyCode() const -> Key { return _keyCode; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

private:
    Key _keyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
    ~KeyPressedEvent() override = default;
    KeyPressedEvent(Key keycode, int repeatCount) : KeyEvent(keycode), _repeatCount(repeatCount) { }

    [[nodiscard]] inline auto getRepeatCount() const -> int { return _repeatCount; }

    [[nodiscard]] auto toString() const -> std::string override
    {
        return fmt::format("KeyPressedEvent: {} ({} repeats)", getKeyCode(), _repeatCount);
    }

    EVENT_CLASS_TYPE(KeyPressed)
private:
    int _repeatCount;
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