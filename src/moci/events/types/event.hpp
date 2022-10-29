#pragma once

#include "moci/core/logging.hpp"

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
    virtual ~Event() = default;
    bool Handled     = false;

    [[nodiscard]] virtual auto getEventType() const -> EventType   = 0;
    [[nodiscard]] virtual auto getName() const -> std::string_view = 0;
    [[nodiscard]] virtual auto getCategoryFlags() const -> int     = 0;
    [[nodiscard]] virtual auto toString() const -> std::string { return std::string(getName()); }

    [[nodiscard]] inline auto isInCategory(EventCategory category) const noexcept -> bool
    {
        return (getCategoryFlags() & category) != 0;
    }
};

#define MOCI_EVENT_METHOD(fn) [self = this](auto& e) { return self->fn(e); }

class EventDispatcher
{
public:
    explicit EventDispatcher(Event& event) : m_Event(event) { }

    // F will be deduced by the compiler
    template<typename T, typename F>
    auto dispatch(const F& func) -> bool
    {
        if (m_Event.getEventType() == T::GetStaticType())
        {
            m_Event.Handled = func(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }

private:
    Event& m_Event;
};

inline auto operator<<(std::ostream& out, const Event& e) -> std::ostream& { return out << e.toString(); }

}  // namespace moci
