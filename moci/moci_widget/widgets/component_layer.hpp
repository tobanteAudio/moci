#pragma once

#include "moci_events/moci_events.hpp"

#include "moci_widget/widgets/component.hpp"
#include "moci_widget/widgets/style.hpp"

#include "moci_graphics/render_queue.hpp"
#include "moci_render_api/moci_render_api.hpp"

#include "imgui.h"

namespace moci
{

class ComponentLayer : public Layer
{
public:
    ComponentLayer(Scope<Component>&& root) : Layer("component-layer"), rootComponent_(std::move(root)) { }
    ~ComponentLayer() override = default;

    void OnAttach() override
    {
        renderQueue_ = MakeScope<RenderQueue>();
        rootComponent_->SetStyle(&defaultStyle_);
        rootComponent_->SetSize(static_cast<int>(width_), static_cast<int>(height_));
    }

    void OnDetach() override { }

    void OnUpdate(Timestep ts) override
    {
        IgnoreUnused(ts);
        // MOCI_CORE_ASSERT(rootComponent_ != nullptr);
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();

        renderQueue_->StartFrame(width_, height_);
        rootComponent_->SetPosition(0, 0);
        rootComponent_->Draw(*renderQueue_);
        renderQueue_->EndFrame();
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::Text("Draws: %d", renderQueue_->GetStats().drawCount);
        ImGui::Text("Textures: %d", renderQueue_->GetStats().textureCount);
        ImGui::Text("Vertics: %d", renderQueue_->GetStats().vertexCount);
        ImGui::Text("Quads: %d", renderQueue_->GetStats().quadCount);
        ImGui::Text("Circles: %d", renderQueue_->GetStats().circleCount);
        ImGui::End();
    }

    void OnEvent(Event& e) override
    {
        auto dispatcher = EventDispatcher {e};
        dispatcher.Dispatch<WindowResizeEvent>(MOCI_EVENT_METHOD(ComponentLayer::OnWindowResized));
        dispatcher.Dispatch<MouseMovedEvent>(MOCI_EVENT_METHOD(ComponentLayer::OnMouseMoved));
        dispatcher.Dispatch<MouseButtonPressedEvent>(MOCI_EVENT_METHOD(ComponentLayer::OnMousePressed));
    }

    bool OnWindowResized(WindowResizeEvent& e)
    {
        width_  = static_cast<float>(e.GetWidth());
        height_ = static_cast<float>(e.GetHeight());
        rootComponent_->SetSize(static_cast<int>(width_), static_cast<int>(height_));
        return false;
    }

    bool OnMouseMoved(MouseMovedEvent& e)
    {
        IgnoreUnused(e);
        // MOCI_CORE_INFO("Mouse: {}, {}", e.GetX(), e.GetY());
        return true;
    }

    bool OnMousePressed(MouseButtonPressedEvent& e)
    {

        auto const code = e.GetMouseButton();
        auto const x    = static_cast<int>(Input::GetMouseX());
        auto const y    = static_cast<int>(Input::GetMouseY());
        auto* comp      = rootComponent_->FindComponentAt({x, y});
        MOCI_CORE_ASSERT(comp, "Should never be null");

        while (!comp->MouseClicked(MouseCallback::Click {x, y}))
        {
            comp = comp->GetParent();
            if (comp == nullptr)
            {
                return false;
            }
        }

        return true;
    }

private:
    float width_  = 1280.0f;
    float height_ = 1024.0f;

    Scope<RenderQueue> renderQueue_ {};
    Style defaultStyle_ {};
    Scope<Component> rootComponent_;
};
}  // namespace moci
