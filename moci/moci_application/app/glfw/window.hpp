#pragma once

#include "moci_application/app/window.hpp"

#include "moci_render_base/moci_render_base.hpp"

// glew is not needed for opengl es 2.0 or on macOS
#if not defined(MOCI_API_OPENGL_ES)
#if not defined(MOCI_MAC)
#include <GL/glew.h>
#endif
#endif

#include <GLFW/glfw3.h>

#include <array>
#include <memory>

namespace moci
{

class GlfwWindow : public Window
{
public:
    GlfwWindow(WindowSpecs props);
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
    virtual void Init(WindowSpecs props);
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