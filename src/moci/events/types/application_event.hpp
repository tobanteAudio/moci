#pragma once

#include "event.hpp"

#include <sstream>

namespace moci {

class WindowResizeEvent : public Event
{
public:
    ~WindowResizeEvent() override = default;

    WindowResizeEvent(unsigned int width, unsigned int height)
        : _width(width)
        , _height(height)
    {}

    [[nodiscard]] inline auto getWidth() const -> unsigned int { return _width; }

    [[nodiscard]] inline auto getHeight() const -> unsigned int { return _height; }

    [[nodiscard]] auto toString() const -> std::string override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << _width << ", " << _height;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
    unsigned int _width, _height;
};

class WindowCloseEvent : public Event
{
public:
    ~WindowCloseEvent() override = default;
    WindowCloseEvent()           = default;

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppTickEvent : public Event
{
public:
    ~AppTickEvent() override = default;
    AppTickEvent()           = default;

    EVENT_CLASS_TYPE(AppTick)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent : public Event
{
public:
    ~AppUpdateEvent() override = default;
    AppUpdateEvent()           = default;

    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent : public Event
{
public:
    ~AppRenderEvent() override = default;
    AppRenderEvent()           = default;

    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
}  // namespace moci
