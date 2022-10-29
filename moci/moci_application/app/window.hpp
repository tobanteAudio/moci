#pragma once

#include <functional>
#include <string>
#include <utility>

#include "moci_events/moci_events.hpp"

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

    static auto Create(WindowSpecs props) -> Window*;

    virtual ~Window() = default;

    virtual void OnUpdate() = 0;

    [[nodiscard]] virtual auto GetWidth() const -> unsigned int  = 0;
    [[nodiscard]] virtual auto GetHeight() const -> unsigned int = 0;

    // Window attributes
    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

    virtual void SetVSync(bool enabled)                = 0;
    [[nodiscard]] virtual auto IsVSync() const -> bool = 0;

    virtual void SetFullscreen(bool enabled)                = 0;
    [[nodiscard]] virtual auto IsFullscreen() const -> bool = 0;

    [[nodiscard]] virtual auto GetFrameCount() const -> std::uint32_t = 0;

    [[nodiscard]] virtual auto GetNativeWindow() const -> void* = 0;
};

}  // namespace moci