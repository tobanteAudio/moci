#pragma once

#include <functional>
#include <string>
#include <utility>

#include <moci/events/types/event.hpp>

namespace moci
{

struct WindowSpecs
{
    std::string Title;
    unsigned int Width;
    unsigned int Height;

    explicit WindowSpecs(std::string title = "moci engine", unsigned int width = 1280, unsigned int height = 1024)
        : Title(std::move(title)), Width(width), Height(height)
    {
    }
};

// Interface representing a desktop system based Window
class Window
{
public:
    using EventCallbackFn = std::function<void(Event&)>;

    static auto create(WindowSpecs props) -> Window*;

    virtual ~Window() = default;

    virtual void onUpdate() = 0;

    [[nodiscard]] virtual auto getWidth() const -> unsigned int  = 0;
    [[nodiscard]] virtual auto getHeight() const -> unsigned int = 0;

    // Window attributes
    virtual void setEventCallback(const EventCallbackFn& callback) = 0;

    virtual void setVSync(bool enabled)                = 0;
    [[nodiscard]] virtual auto isVSync() const -> bool = 0;

    virtual void setFullscreen(bool enabled)                = 0;
    [[nodiscard]] virtual auto isFullscreen() const -> bool = 0;

    [[nodiscard]] virtual auto getFrameCount() const -> std::uint32_t = 0;

    [[nodiscard]] virtual auto getNativeWindow() const -> void* = 0;
};

}  // namespace moci