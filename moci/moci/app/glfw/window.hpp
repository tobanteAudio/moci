#pragma once

#include "moci/app/window.hpp"

#include "moci/render/opengl/es2/es2.hpp"
#include "moci_graphics_api_base/moci_graphics_api_base.hpp"

#include <array>
#include <memory>

namespace moci
{

class GlfwWindow : public Window
{
public:
    GlfwWindow(const WindowProps& props);
    ~GlfwWindow() override;

    void OnUpdate() override;

    [[nodiscard]] inline auto GetWidth() const -> unsigned int override { return m_Data.Width; }
    [[nodiscard]] inline auto GetHeight() const -> unsigned int override { return m_Data.Height; }

    // Window attributes
    inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
    void SetVSync(bool enabled) override;
    [[nodiscard]] auto IsVSync() const -> bool override;

    void SetFullscreen(bool enabled) override;
    [[nodiscard]] bool IsFullscreen() const override;

    [[nodiscard]] std::uint32_t GetFrameCount() const noexcept override { return frameCounter_; }

    [[nodiscard]] inline auto GetNativeWindow() const -> void* override { return m_Window; }

private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();

    GLFWwindow* m_Window {};
    Scope<GraphicsContext> m_Context;

    struct WindowData
    {
        std::string Title;
        unsigned int Width {}, Height {};
        std::array<int, 2> Position {};
        bool VSync {};
        bool Fullscreen {};

        EventCallbackFn EventCallback;
    };

    WindowData m_Data;
    std::uint32_t frameCounter_ {};
};

}  // namespace moci