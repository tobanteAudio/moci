#include "application.hpp"

#include "moci/benchmark/profile.hpp"
#include "moci/core/logging.hpp"
#include "moci/events/types/event.hpp"
#include "moci_render_api/moci_render_api.hpp"

namespace moci
{

Application* Application::s_Instance = nullptr;

Application::Application() : Application(WindowSpecs {}) { }

Application::Application(WindowSpecs windowSpecs)
{
    MOCI_CORE_ASSERT(!s_Instance, "Application already exists!");
    s_Instance = this;
    MOCI_CORE_INFO("Initializing App...");

    m_Window = Scope<Window>(Window::Create(std::move(windowSpecs)));
    m_Window->SetEventCallback(MOCI_EVENT_METHOD(OnEvent));

    Renderer::Init();
    MOCI_CORE_INFO("Max vertex attributes: {}", RenderCommand::MaxVertexAttributes());
    MOCI_CORE_INFO("Max texture size: {}", RenderCommand::MaxTextureSize());
    MOCI_CORE_INFO("Max texture units: {}", RenderCommand::MaxTextureUnits());
    MOCI_CORE_INFO("Max uniform vectors: {}", RenderCommand::MaxUniformVectors());

    PushOverlay(MakeScope<ImGuiLayer>());
}

void Application::PushLayer(Layer::Ptr&& layer) { m_LayerStack.PushLayer(std::move(layer)); }

void Application::PushOverlay(Layer::Ptr&& layer) { m_LayerStack.PushOverlay(std::move(layer)); }

void Application::OnEvent(Event& e)
{
    if (e.GetEventType() == EventType::KeyPressed)
    {
        auto* keyEvent = dynamic_cast<KeyPressedEvent*>(&e);
        if (keyEvent->GetKeyCode() == Key::Escape)
        {
            auto closeEvent = WindowCloseEvent {};
            OnWindowClose(closeEvent);
        }
    }

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(MOCI_EVENT_METHOD(OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(MOCI_EVENT_METHOD(OnWindowResize));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
        (*--it)->OnEvent(e);
        if (e.Handled) { break; }
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
        auto const timestep  = Timestep {static_cast<float>(elapsedTime.count()) / 1'000.0F / 1'000.0F};
        m_LastFrameTimepoint = now;

        if (!m_Minimized)
        {
            for (auto& layer : m_LayerStack) { layer->OnUpdate(timestep); }
        }

        {
            MOCI_PROFILE_SCOPE("App::Run::Loop::ImGui");
            moci::ImGuiLayer::Begin();
            for (auto& layer : m_LayerStack) { layer->OnImGuiRender(); }
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