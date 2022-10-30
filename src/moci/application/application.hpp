#pragma once

#include "moci/events/layer/layer_stack.hpp"
#include "moci/events/types/application_event.hpp"
#include "moci/events/types/key_event.hpp"
#include "moci/events/types/mouse_event.hpp"
#include "moci/render/renderer.hpp"

#include "window.hpp"

#include <chrono>
#include <memory>

auto main(int argc, char** argv) -> int;

namespace moci
{

class Application
{
public:
    Application();
    explicit Application(WindowSpecs windowSpecs);

    virtual ~Application() = default;

    void onEvent(Event& e);

    void pushLayer(Layer::Ptr&& layer);
    void pushOverlay(Layer::Ptr&& layer);

    inline auto getWindow() -> Window& { return *_window; }

    inline static auto get() -> Application& { return *sInstance; }

private:
    auto run() -> void;

    auto onWindowClose(WindowCloseEvent& e) -> bool;
    auto onWindowResize(WindowResizeEvent& e) -> bool;

    Scope<Window> _window                                                  = nullptr;
    bool _running                                                          = true;
    bool _minimized                                                        = false;
    LayerStack _layerStack                                                 = {};
    float _lastFrameTime                                                   = 0.0F;
    std::chrono::time_point<std::chrono::steady_clock> _lastFrameTimepoint = {};

    static Application* sInstance;
    friend auto ::main(int argc, char** argv) -> int;
};

// To be defined in CLIENT
auto createApplication() -> Application*;

}  // namespace moci