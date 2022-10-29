#pragma once

#include "moci/events/input.hpp"
#include "moci/events/layer/layer.hpp"
#include "moci/events/timestep.hpp"
#include "moci/events/types/application_event.hpp"
#include "moci/events/types/event.hpp"
#include "moci/render/renderer.hpp"
#include "moci/widgets/component/component.hpp"
#include "moci/widgets/style/style.hpp"

#include "imgui.h"

namespace moci
{

class ComponentLayer : public Layer
{
public:
    explicit ComponentLayer(Scope<Component>&& root) : Layer("component-layer"), rootComponent_(std::move(root)) { }
    ~ComponentLayer() override = default;

    void onAttach() override;
    void onDetach() override;
    void onUpdate(Timestep ts) override;
    void onImGuiRender() override;
    void onEvent(Event& e) override;

private:
    auto onWindowResized(WindowResizeEvent& e) -> bool;
    auto onMouseMoved(MouseMovedEvent& e) -> bool;
    auto onMouseScrolled(MouseScrolledEvent& e) -> bool;
    auto onMousePressed(MouseButtonPressedEvent& e) -> bool;

    template<typename Handler>
    auto handleEvent(Handler handler) -> bool
    {
        MOCI_PROFILE_FUNCTION();
        Component* comp = nullptr;
        {
            MOCI_PROFILE_SCOPE("ComponentLayer::handleEvent::FindComponent");
            auto const x = static_cast<int>(Input::getMouseX());
            auto const y = static_cast<int>(Input::getMouseY());
            comp         = rootComponent_->findComponentAt({x, y});
        }

        MOCI_CORE_ASSERT(comp, "Should never be null");
        while (!handler(comp))
        {
            comp = comp->getParent();
            if (comp == nullptr) { return false; }
        }

        return true;
    }

    float width_  = 1280.0F;
    float height_ = 1024.0F;

    Scope<BatchRender2D> renderQueue_ {};
    Style defaultStyle_ {};
    Scope<Component> rootComponent_;
};
}  // namespace moci
