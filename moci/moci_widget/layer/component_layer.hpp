#pragma once

#include "moci/events/input.hpp"
#include "moci/events/timestep.hpp"
#include "moci/events/types/event.hpp"

#include "moci_widget/component/component.hpp"
#include "moci_widget/style/style.hpp"

#include "moci_graphics_extra/moci_graphics_extra.hpp"
#include "moci_render_api/moci_render_api.hpp"

#include "imgui.h"

namespace moci
{

class ComponentLayer : public Layer
{
public:
    explicit ComponentLayer(Scope<Component>&& root) : Layer("component-layer"), rootComponent_(std::move(root)) { }
    ~ComponentLayer() override = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Event& e) override;

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
            auto const x = static_cast<int>(Input::GetMouseX());
            auto const y = static_cast<int>(Input::GetMouseY());
            comp         = rootComponent_->FindComponentAt({x, y});
        }

        MOCI_CORE_ASSERT(comp, "Should never be null");
        while (!handler(comp))
        {
            comp = comp->GetParent();
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
