#include "application.hpp"

#include "moci/benchmark/profile.hpp"
#include "moci/core/logging.hpp"
#include "moci/events/types/event.hpp"
#include "moci/render/imgui/imgui_layer.hpp"

namespace moci
{

Application* Application::sInstance = nullptr;

Application::Application() : Application(WindowSpecs {}) { }

Application::Application(WindowSpecs windowSpecs)
{
    MOCI_CORE_ASSERT(!sInstance, "Application already exists!");
    sInstance = this;
    MOCI_CORE_INFO("Initializing App...");

    m_Window = Scope<Window>(Window::create(std::move(windowSpecs)));
    m_Window->setEventCallback(MOCI_EVENT_METHOD(onEvent));

    Renderer::init();
    MOCI_CORE_INFO("Max vertex attributes: {}", RenderCommand::maxVertexAttributes());
    MOCI_CORE_INFO("Max texture size: {}", RenderCommand::maxTextureSize());
    MOCI_CORE_INFO("Max texture units: {}", RenderCommand::maxTextureUnits());
    MOCI_CORE_INFO("Max uniform vectors: {}", RenderCommand::maxUniformVectors());

    pushOverlay(makeScope<ImGuiLayer>());
}

void Application::pushLayer(Layer::Ptr&& layer) { m_LayerStack.pushLayer(std::move(layer)); }

void Application::pushOverlay(Layer::Ptr&& layer) { m_LayerStack.pushOverlay(std::move(layer)); }

void Application::onEvent(Event& e)
{
    if (e.GetEventType() == EventType::KeyPressed)
    {
        auto* keyEvent = dynamic_cast<KeyPressedEvent*>(&e);
        if (keyEvent->getKeyCode() == Key::Escape)
        {
            auto closeEvent = WindowCloseEvent {};
            onWindowClose(closeEvent);
        }
    }

    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowCloseEvent>(MOCI_EVENT_METHOD(onWindowClose));
    dispatcher.dispatch<WindowResizeEvent>(MOCI_EVENT_METHOD(onWindowResize));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
        (*--it)->onEvent(e);
        if (e.Handled) { break; }
    }
}

void Application::run()
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
            for (auto& layer : m_LayerStack) { layer->onUpdate(timestep); }
        }

        {
            MOCI_PROFILE_SCOPE("App::Run::Loop::ImGui");
            moci::ImGuiLayer::begin();
            for (auto& layer : m_LayerStack) { layer->onImGuiRender(); }
            moci::ImGuiLayer::end();
        }

        {
            MOCI_PROFILE_SCOPE("App::Run::Loop::WindowUpdate");
            m_Window->onUpdate();
        }
    }
}

auto Application::onWindowClose(WindowCloseEvent& e) -> bool
{
    ignoreUnused(e);
    m_Running = false;
    return true;
}

auto Application::onWindowResize(WindowResizeEvent& e) -> bool
{
    if (e.getWidth() == 0 || e.getHeight() == 0)
    {
        m_Minimized = true;
        return false;
    }

    m_Minimized = false;
    Renderer::onWindowResize(e.getWidth(), e.getHeight());

    return false;
}

}  // namespace moci