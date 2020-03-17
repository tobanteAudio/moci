#include "application.hpp"

#include "moci/core/logging.hpp"
#include "moci/render/opengles2/opengles2.hpp"
#include "moci/render/renderer.hpp"

#include "moci/app/events/key_event.hpp"
#include "moci/app/events/mouse_event.hpp"
#include "moci/app/key_codes.hpp"

namespace moci
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application()
{
    MOCI_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;

    m_Window = std::unique_ptr<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

    Renderer::Init();

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
}

void Application::PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }

void Application::PushOverlay(Layer* layer) { m_LayerStack.PushOverlay(layer); }

void Application::OnEvent(Event& e)
{
    if (e.GetEventType() == EventType::KeyPressed)
    {
        auto* keyEvent = dynamic_cast<KeyPressedEvent*>(&e);
        if (keyEvent->GetKeyCode() == MOCI_KEY_ESCAPE)
        {
            auto closeEvent = WindowCloseEvent {};
            OnWindowClose(closeEvent);
        }
    }

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
        (*--it)->OnEvent(e);
        if (e.Handled)
        {
            break;
        }
    }
}

void Application::Run()
{
    while (m_Running)
    {
        auto time         = (float)glfwGetTime();
        Timestep timestep = time - m_LastFrameTime;
        m_LastFrameTime   = time;

        if (!m_Minimized)
        {
            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate(timestep);
            }
        }

        moci::ImGuiLayer::Begin();
        for (Layer* layer : m_LayerStack)
        {
            layer->OnImGuiRender();
        }
        moci::ImGuiLayer::End();

        m_Window->OnUpdate();
    }
}

auto Application::OnWindowClose(WindowCloseEvent& e) -> bool
{
    m_Running = false;
    return true;
}

auto Application::OnWindowResize(WindowResizeEvent& e) -> bool
{
    if (e.GetWidth() == 0 || e.GetHeight() == 0)
    {
        m_Minimized = true;
        return false;
    }

    m_Minimized = false;
    Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

    return false;
}

}  // namespace moci