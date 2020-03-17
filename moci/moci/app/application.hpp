#pragma once

#include "events/application_event.hpp"
#include "layer_stack.hpp"
#include "window.hpp"

#include "moci/render/imgui/imgui_layer.hpp"

#include <memory>

auto main(int argc, char** argv) -> int;

namespace moci
{

class Application
{
public:
    Application();
    virtual ~Application() = default;

    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline auto GetWindow() -> Window& { return *m_Window; }

    inline static auto Get() -> Application& { return *s_Instance; }

private:
    auto Run() -> void;

    auto OnWindowClose(WindowCloseEvent& e) -> bool;
    auto OnWindowResize(WindowResizeEvent& e) -> bool;

private:
    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    bool m_Running   = true;
    bool m_Minimized = false;
    LayerStack m_LayerStack;
    float m_LastFrameTime = 0.0f;

private:
    static Application* s_Instance;
    friend auto ::main(int argc, char** argv) -> int;
};

// To be defined in CLIENT
auto CreateApplication() -> Application*;

}  // namespace moci