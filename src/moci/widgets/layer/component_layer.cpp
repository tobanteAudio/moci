#include "moci/widgets/layer/component_layer.hpp"

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
    RenderCommand::SetClearColor({0.1F, 0.1F, 0.1F, 1});
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
    dispatcher.Dispatch<WindowResizeEvent>([this](auto& e) { return onWindowResized(e); });
    dispatcher.Dispatch<MouseMovedEvent>([this](auto& e) { return onMouseMoved(e); });
    dispatcher.Dispatch<MouseScrolledEvent>([this](auto& e) { return onMouseScrolled(e); });
    dispatcher.Dispatch<MouseButtonPressedEvent>([this](auto& e) { return onMousePressed(e); });
}

auto ComponentLayer::onWindowResized(WindowResizeEvent& e) -> bool
{
    width_  = static_cast<float>(e.GetWidth());
    height_ = static_cast<float>(e.GetHeight());
    rootComponent_->SetSize(static_cast<int>(width_), static_cast<int>(height_));
    return false;
}

auto ComponentLayer::onMouseMoved(MouseMovedEvent& e) -> bool
{
    IgnoreUnused(e);
    (void)this;
    return true;
}

auto ComponentLayer::onMouseScrolled(MouseScrolledEvent& e) -> bool
{
    MOCI_PROFILE_FUNCTION();
    return handleEvent([&](Component* comp) { return comp->MouseScrolled(e); });
}

auto ComponentLayer::onMousePressed(MouseButtonPressedEvent& e) -> bool
{
    MOCI_PROFILE_FUNCTION();
    IgnoreUnused(e);
    auto const x = static_cast<int>(Input::GetMouseX());
    auto const y = static_cast<int>(Input::GetMouseY());
    return handleEvent([&](auto* comp) { return comp->MouseClicked(MouseCallback::Click {x, y}); });
}

}  // namespace moci
