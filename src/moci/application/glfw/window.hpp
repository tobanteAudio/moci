#pragma once

#include <moci/application/window.hpp>
#include <moci/core/logging.hpp>
#include <moci/render/api.hpp>
#include <moci/render/graphics_context.hpp>

#if MOCI_API_OPENGL_ES
#define GLFW_INCLUDE_ES2
#define IMGUI_IMPL_OPENGL_ES2
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#else
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <array>
#include <memory>

namespace moci
{

class GlfwWindow : public Window
{
public:
    explicit GlfwWindow(WindowSpecs props);
    ~GlfwWindow() override;

    void onUpdate() override;

    [[nodiscard]] inline auto getWidth() const -> unsigned int override { return _data.Width; }
    [[nodiscard]] inline auto getHeight() const -> unsigned int override { return _data.Height; }

    // Window attributes
    inline void setEventCallback(const EventCallbackFn& callback) override { _data.EventCallback = callback; }
    void setVSync(bool enabled) override;
    [[nodiscard]] auto isVSync() const -> bool override;

    void setFullscreen(bool enabled) override;
    [[nodiscard]] auto isFullscreen() const -> bool override;

    [[nodiscard]] auto getFrameCount() const noexcept -> std::uint32_t override { return _frameCounter; }

    [[nodiscard]] inline auto getNativeWindow() const -> void* override { return _window; }

private:
    virtual void init(WindowSpecs props);
    virtual void shutdown();

    GLFWwindow* _window {};
    std::unique_ptr<GraphicsContext> _context;

    struct WindowData
    {
        std::string Title;
        unsigned int Width {}, Height {};
        std::array<int, 2> Position {};
        bool VSync {};
        bool Fullscreen {};

        EventCallbackFn EventCallback;
    };

    WindowData _data;
    std::uint32_t _frameCounter {};
};

}  // namespace moci
