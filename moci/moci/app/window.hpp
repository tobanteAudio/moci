#pragma once

#include <functional>
#include <string>
#include <utility>

#include "events/event.hpp"

namespace moci
{

struct WindowProps
{
    std::string Title;
    unsigned int Width;
    unsigned int Height;

    WindowProps(std::string title = "moci engine", unsigned int width = 1280, unsigned int height = 1024)
        : Title(std::move(title)), Width(width), Height(height)
    {
    }
};

// Interface representing a desktop system based Window
class Window
{
public:
    using EventCallbackFn = std::function<void(Event&)>;


    static auto Create(const WindowProps& props = WindowProps()) -> Window*;


    virtual ~Window() = default;

    virtual void OnUpdate() = 0;

    [[nodiscard]] virtual auto GetWidth() const -> unsigned int  = 0;
    [[nodiscard]] virtual auto GetHeight() const -> unsigned int = 0;

    // Window attributes
    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

    virtual void SetVSync(bool enabled)                = 0;
    [[nodiscard]] virtual auto IsVSync() const -> bool = 0;

    virtual void SetFullscreen(bool enabled)        = 0;
    [[nodiscard]] virtual bool IsFullscreen() const = 0;

    [[nodiscard]] virtual std::uint32_t GetFrameCount() const = 0;

    [[nodiscard]] virtual auto GetNativeWindow() const -> void* = 0;
};

}  // namespace moci