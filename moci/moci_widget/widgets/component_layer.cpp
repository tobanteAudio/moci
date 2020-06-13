#include "moci_widget/widgets/component_layer.hpp"

namespace moci
{
void ComponentLayer::OnAttach()
{
    renderQueue_ = MakeScope<RenderQueue>();
    rootComponent_->SetStyle(&defaultStyle_);
    rootComponent_->SetSize(static_cast<int>(width_), static_cast<int>(height_));
}

void ComponentLayer::OnDetach() { }

void ComponentLayer::OnUpdate(Timestep ts)
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

void ComponentLayer::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::Text("Draws: %d", renderQueue_->GetStats().drawCount);
    ImGui::Text("Textures: %d", renderQueue_->GetStats().textureCount);
    ImGui::Text("Vertics: %d", renderQueue_->GetStats().vertexCount);
    ImGui::Text("Quads: %d", renderQueue_->GetStats().quadCount);
    ImGui::Text("Circles: %d", renderQueue_->GetStats().circleCount);
    ImGui::End();
}

void ComponentLayer::OnEvent(Event& e)
{
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
    // MOCI_CORE_INFO("Mouse: {}, {}", e.GetX(), e.GetY());
    return true;
}

bool ComponentLayer::onMouseScrolled(MouseScrolledEvent& e)
{
    return handleEvent([&](Component* comp) { return comp->MouseScrolled(e); });
}

bool ComponentLayer::onMousePressed(MouseButtonPressedEvent& e)
{
    IgnoreUnused(e);
    auto const x = static_cast<int>(Input::GetMouseX());
    auto const y = static_cast<int>(Input::GetMouseY());
    return handleEvent([&](auto* comp) { return comp->MouseClicked(MouseCallback::Click {x, y}); });
}

}  // namespace moci
