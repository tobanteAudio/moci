#include "application.hpp"

#include "moci/core/logging.hpp"
#include "moci/debug/instrumentor.hpp"
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
    MOCI_CORE_INFO("Initializing App...");

    m_Window = Scope<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

    Renderer::Init();
    PushOverlay(MakeScope<ImGuiLayer>());
}

void Application::PushLayer(Layer::Ptr&& layer) { m_LayerStack.PushLayer(std::move(layer)); }

void Application::PushOverlay(Layer::Ptr&& layer) { m_LayerStack.PushOverlay(std::move(layer)); }

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
    using namespace std::chrono;
    while (m_Running)
    {
        MOCI_PROFILE_SCOPE("App::Run::Loop");
        auto const now         = steady_clock::now();
        auto const elapsedTime = time_point_cast<microseconds>(now).time_since_epoch()
                                 - time_point_cast<microseconds>(m_LastFrameTimepoint).time_since_epoch();
        auto const timestep  = Timestep {static_cast<float>(elapsedTime.count()) / 1'000.0f / 1'000.0f};
        m_LastFrameTimepoint = now;

        if (!m_Minimized)
        {
            for (auto& layer : m_LayerStack)
            {
                layer->OnUpdate(timestep);
            }
        }

        {
            MOCI_PROFILE_SCOPE("App::Run::Loop::ImGui");
            moci::ImGuiLayer::Begin();
            for (auto& layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }
            moci::ImGuiLayer::End();
        }

        {
            MOCI_PROFILE_SCOPE("App::Run::Loop::WindowUpdate");
            m_Window->OnUpdate();
        }
    }
}

auto Application::OnWindowClose(WindowCloseEvent& e) -> bool
{
    IgnoreUnused(e);
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