#include "component_layer.hpp"

namespace moci
{
void ComponentLayer::onAttach()
{
    MOCI_PROFILE_FUNCTION();
    _renderQueue = std::make_unique<BatchRender2D>();
    _rootComponent->setStyle(&_defaultStyle);
    _rootComponent->setSize(static_cast<int>(_width), static_cast<int>(_height));
}

void ComponentLayer::onDetach() { MOCI_PROFILE_FUNCTION(); }

void ComponentLayer::onUpdate(Timestep ts)
{
    MOCI_PROFILE_FUNCTION();
    ignoreUnused(ts);
    // MOCI_CORE_ASSERT(rootComponent_ != nullptr);
    RenderCommand::setClearColor({0.1F, 0.1F, 0.1F, 1});
    RenderCommand::clear();

    _renderQueue->startFrame(_width, _height);
    _rootComponent->setPosition(0, 0);
    _rootComponent->draw(*_renderQueue);
    _renderQueue->endFrame();
}

void ComponentLayer::onImGuiRender()
{
    MOCI_PROFILE_FUNCTION();
    ImGui::Begin("Settings");
    ImGui::Text("Draws: %d", _renderQueue->getFrameStats().drawCount);
    ImGui::Text("Textures: %d", _renderQueue->getFrameStats().textureCount);
    ImGui::Text("Vertics: %d", _renderQueue->getFrameStats().vertexCount);
    ImGui::Text("Quads: %d", _renderQueue->getFrameStats().quadCount);
    ImGui::Text("Circles: %d", _renderQueue->getFrameStats().circleCount);
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
    _width  = static_cast<float>(e.getWidth());
    _height = static_cast<float>(e.getHeight());
    _rootComponent->setSize(static_cast<int>(_width), static_cast<int>(_height));
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
