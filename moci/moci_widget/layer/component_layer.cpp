#include "moci_widget/layer/component_layer.hpp"

namespace moci
{
void ComponentLayer::OnAttach()
{
    MOCI_PROFILE_FUNCTION();
    renderQueue_ = MakeScope<BatchRender2D>();
    rootComponent_->SetStyle(&defaultStyle_);
    rootComponent_->SetSize(static_cast<int>(width_), static_cast<int>(height_));
}

void ComponentLayer::OnDetach() { MOCI_PROFILE_FUNCTION(); }

void ComponentLayer::OnUpdate(Timestep ts)
{
    MOCI_PROFILE_FUNCTION();
    IgnoreUnused(ts);
    // MOCI_CORE_ASSERT(rootComponent_ != nullptr);
    RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    RenderCommand::Clear();

    renderQueue_->StartFrame(width_, height_);
    rootComponent_->SetPosition(0, 0);
    rootComponent_->Draw(*renderQueue_);
    renderQueue_->EndFrame();
}

void ComponentLayer::OnImGuiRender()
{
    MOCI_PROFILE_FUNCTION();
    ImGui::Begin("Settings");
    ImGui::Text("Draws: %d", renderQueue_->GetFrameStats().drawCount);
    ImGui::Text("Textures: %d", renderQueue_->GetFrameStats().textureCount);
    ImGui::Text("Vertics: %d", renderQueue_->GetFrameStats().vertexCount);
    ImGui::Text("Quads: %d", renderQueue_->GetFrameStats().quadCount);
    ImGui::Text("Circles: %d", renderQueue_->GetFrameStats().circleCount);
    ImGui::End();
}

void ComponentLayer::OnEvent(Event& e)
{
    MOCI_PROFILE_FUNCTION();
    auto dispatcher = EventDispatcher {e};
    dispatcher.Dispatch<WindowResizeEvent>([self = this](auto& e) { return self->onWindowResized(e); });
    dispatcher.Dispatch<MouseMovedEvent>([self = this](auto& e) { return self->onMouseMoved(e); });
    dispatcher.Dispatch<MouseScrolledEvent>([self = this](auto& e) { return self->onMouseScrolled(e); });
    dispatcher.Dispatch<MouseButtonPressedEvent>([self = this](auto& e) { return self->onMousePressed(e); });
}

bool ComponentLayer::onWindowResized(WindowResizeEvent& e)
{
    width_  = static_cast<float>(e.GetWidth());
    height_ = static_cast<float>(e.GetHeight());
    rootComponent_->SetSize(static_cast<int>(width_), static_cast<int>(height_));
    return false;
}

bool ComponentLayer::onMouseMoved(MouseMovedEvent& e)
{
    IgnoreUnused(e);
    return true;
}

bool ComponentLayer::onMouseScrolled(MouseScrolledEvent& e)
{
    MOCI_PROFILE_FUNCTION();
    return handleEvent([&](Component* comp) { return comp->MouseScrolled(e); });
}

bool ComponentLayer::onMousePressed(MouseButtonPressedEvent& e)
{
    MOCI_PROFILE_FUNCTION();
    IgnoreUnused(e);
    auto const x = static_cast<int>(Input::GetMouseX());
    auto const y = static_cast<int>(Input::GetMouseY());
    return handleEvent([&](auto* comp) { return comp->MouseClicked(MouseCallback::Click {x, y}); });
}

}  // namespace moci
