#include "application.hpp"

#include "moci/core/benchmark/profile.hpp"
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

    _m_Window = Scope<Window>(Window::create(std::move(windowSpecs)));
    _m_Window->setEventCallback(MOCI_EVENT_METHOD(onEvent));

    Renderer::init();
    MOCI_CORE_INFO("Max vertex attributes: {}", RenderCommand::maxVertexAttributes());
    MOCI_CORE_INFO("Max texture size: {}", RenderCommand::maxTextureSize());
    MOCI_CORE_INFO("Max texture units: {}", RenderCommand::maxTextureUnits());
    MOCI_CORE_INFO("Max uniform vectors: {}", RenderCommand::maxUniformVectors());

    pushOverlay(makeScope<ImGuiLayer>());
}

void Application::pushLayer(Layer::Ptr&& layer) { _m_LayerStack.pushLayer(std::move(layer)); }

void Application::pushOverlay(Layer::Ptr&& layer) { _m_LayerStack.pushOverlay(std::move(layer)); }

void Application::onEvent(Event& e)
{
    if (e.getEventType() == EventType::KeyPressed)
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

    for (auto it = _m_LayerStack.end(); it != _m_LayerStack.begin();)
    {
        (*--it)->onEvent(e);
        if (e.Handled) { break; }
    }
}

void Application::run()
{
    using namespace std::chrono;
    while (_m_Running)
    {
        MOCI_PROFILE_SCOPE("App::Run::Loop");
        auto const now         = steady_clock::now();
        auto const elapsedTime = time_point_cast<microseconds>(now).time_since_epoch()
                                 - time_point_cast<microseconds>(_m_LastFrameTimepoint).time_since_epoch();
        auto const timestep   = Timestep {static_cast<float>(elapsedTime.count()) / 1'000.0F / 1'000.0F};
        _m_LastFrameTimepoint = now;

        if (!_m_Minimized)
        {
            for (auto& layer : _m_LayerStack) { layer->onUpdate(timestep); }
        }

        {
            MOCI_PROFILE_SCOPE("App::Run::Loop::ImGui");
            moci::ImGuiLayer::begin();
            for (auto& layer : _m_LayerStack) { layer->onImGuiRender(); }
            moci::ImGuiLayer::end();
        }

        {
            MOCI_PROFILE_SCOPE("App::Run::Loop::WindowUpdate");
            _m_Window->onUpdate();
        }
    }
}

auto Application::onWindowClose(WindowCloseEvent& e) -> bool
{
    ignoreUnused(e);
    _m_Running = false;
    return true;
}

auto Application::onWindowResize(WindowResizeEvent& e) -> bool
{
    if (e.getWidth() == 0 || e.getHeight() == 0)
    {
        _m_Minimized = true;
        return false;
    }

    _m_Minimized = false;
    Renderer::onWindowResize(e.getWidth(), e.getHeight());

    return false;
}

}  // namespace moci