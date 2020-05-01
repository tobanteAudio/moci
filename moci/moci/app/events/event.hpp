#pragma once

#include "moci/core/preprocessor.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace moci
{

// Events in moci are currently blocking, meaning when an event occurs it
// immediately gets dispatched and must be dealt with right then an there.
// For the future, a better strategy might be to buffer events in an event
// bus and process them during the "event" part of the update stage.

enum class EventType
{
    None = 0,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    AppTick,
    AppUpdate,
    AppRender,
    KeyPressed,
    KeyReleased,
    KeyTyped,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled
};

enum EventCategory
{
    None                     = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput       = BIT(1),
    EventCategoryKeyboard    = BIT(2),
    EventCategoryMouse       = BIT(3),
    EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                                                         \
    static EventType GetStaticType() { return EventType::type; }                                                       \
    virtual EventType GetEventType() const override { return GetStaticType(); }                                        \
    virtual std::string_view GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)                                                                                 \
    virtual int GetCategoryFlags() const override { return category; }

class Event
{
public:
    bool Handled = false;

    [[nodiscard]] virtual auto GetEventType() const -> EventType   = 0;
    [[nodiscard]] virtual auto GetName() const -> std::string_view = 0;
    [[nodiscard]] virtual auto GetCategoryFlags() const -> int     = 0;
    [[nodiscard]] virtual auto ToString() const -> std::string { return std::string(GetName()); }

    inline bool IsInCategory(EventCategory category) const noexcept { return (GetCategoryFlags() & category) != 0; }
};

class EventDispatcher
{
public:
    EventDispatcher(Event& event) : m_Event(event) { }

    // F will be deduced by the compiler
    template<typename T, typename F>
    auto Dispatch(const F& func) -> bool
    {
        if (m_Event.GetEventType() == T::GetStaticType())
        {
            m_Event.Handled = func(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }

private:
    Event& m_Event;
};

inline auto operator<<(std::ostream& os, const Event& e) -> std::ostream& { return os << e.ToString(); }

}  // namespace moci
