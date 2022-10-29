#include "moci/widgets/layer/component_layer.hpp"

namespace moci
{
void ComponentLayer::onAttach()
{
    MOCI_PROFILE_FUNCTION();
    renderQueue_ = makeScope<BatchRender2D>();
    rootComponent_->setStyle(&defaultStyle_);
    rootComponent_->setSize(static_cast<int>(width_), static_cast<int>(height_));
}

void ComponentLayer::onDetach() { MOCI_PROFILE_FUNCTION(); }

void ComponentLayer::onUpdate(Timestep ts)
{
    MOCI_PROFILE_FUNCTION();
    ignoreUnused(ts);
    // MOCI_CORE_ASSERT(rootComponent_ != nullptr);
    RenderCommand::setClearColor({0.1F, 0.1F, 0.1F, 1});
    RenderCommand::clear();

    renderQueue_->startFrame(width_, height_);
    rootComponent_->setPosition(0, 0);
    rootComponent_->draw(*renderQueue_);
    renderQueue_->endFrame();
}

void ComponentLayer::onImGuiRender()
{
    MOCI_PROFILE_FUNCTION();
    ImGui::Begin("Settings");
    ImGui::Text("Draws: %d", renderQueue_->getFrameStats().drawCount);
    ImGui::Text("Textures: %d", renderQueue_->getFrameStats().textureCount);
    ImGui::Text("Vertics: %d", renderQueue_->getFrameStats().vertexCount);
    ImGui::Text("Quads: %d", renderQueue_->getFrameStats().quadCount);
    ImGui::Text("Circles: %d", renderQueue_->getFrameStats().circleCount);
    ImGui::End();
}

void ComponentLayer::onEvent(Event& e)
{
    MOCI_PROFILE_FUNCTION();
    auto dispatcher = EventDispatcher {e};
    dispatcher.dispatch<WindowResizeEvent>([this](auto& e) { return onWindowResized(e); });
    dispatcher.dispatch<MouseMovedEvent>([this](auto& e) { return onMouseMoved(e); });
    dispatcher.dispatch<MouseScrolledEvent>([this](auto& e) { return onMouseScrolled(e); });
    dispatcher.dispatch<MouseButtonPressedEvent>([this](auto& e) { return onMousePressed(e); });
}

auto ComponentLayer::onWindowResized(WindowResizeEvent& e) -> bool
{
    width_  = static_cast<float>(e.getWidth());
    height_ = static_cast<float>(e.getHeight());
    rootComponent_->setSize(static_cast<int>(width_), static_cast<int>(height_));
    return false;
}

auto ComponentLayer::onMouseMoved(MouseMovedEvent& e) -> bool
{
    ignoreUnused(e);
    (void)this;
    return true;
}

auto ComponentLayer::onMouseScrolled(MouseScrolledEvent& e) -> bool
{
    MOCI_PROFILE_FUNCTION();
    return handleEvent([&](Component* comp) { return comp->mouseScrolled(e); });
}

auto ComponentLayer::onMousePressed(MouseButtonPressedEvent& e) -> bool
{
    MOCI_PROFILE_FUNCTION();
    ignoreUnused(e);
    auto const x = static_cast<int>(Input::getMouseX());
    auto const y = static_cast<int>(Input::getMouseY());
    return handleEvent([&](auto* comp) { return comp->mouseClicked(MouseCallback::Click {x, y}); });
}

}  // namespace moci
