#pragma once

#include "moci/app/window.hpp"

#include "moci/render/graphics_context.hpp"
#include "moci/render/opengl/es2/es2.hpp"

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

    [[nodiscard]] inline auto GetNativeWindow() const -> void* override { return m_Window; }

private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();

private:
    GLFWwindow* m_Window {};
    std::unique_ptr<GraphicsContext> m_Context;

    struct WindowData
    {
        std::string Title;
        unsigned int Width {}, Height {};
        bool VSync {};

        EventCallbackFn EventCallback;
    };

    WindowData m_Data;
};

}  // namespace moci