#pragma once

#include <moci/events/layer/layer.hpp>
#include <moci/events/types/application_event.hpp>
#include <moci/events/types/key_event.hpp>
#include <moci/events/types/mouse_event.hpp>
#include <moci/graphics_extra/light.hpp>
#include <moci/render/frame_buffer.hpp>
#include <moci/render/render_factory.hpp>

class OpenGLLayer final : public moci::Layer
{
public:
    OpenGLLayer()           = default;
    ~OpenGLLayer() override = default;

    void onAttach() override;
    void onUpdate(moci::Timestep ts) override;
    void onImGuiRender() override;
    void onEvent(moci::Event& e) override;

private:
    auto handleWindowResized(moci::WindowResizeEvent& e) -> bool;
    static auto handleKeyPressed(moci::KeyPressedEvent& e) -> bool;
    static auto handleMouseScrolled(moci::MouseScrolledEvent& e) -> bool;
    static auto handleMouseMoved(moci::MouseMovedEvent& e) -> bool;
    static auto handleMousePressed(moci::MouseButtonPressedEvent& e) -> bool;
    static auto handleMouseReleased(moci::MouseButtonReleasedEvent& e) -> bool;

    float _width  = 1920.0F;
    float _height = 1080.0F;

    bool _framebufferNeedsResize = false;
    glm::vec2 _viewportSize      = {};

    std::shared_ptr<moci::Shader> _shader           = nullptr;
    std::shared_ptr<moci::VertexBuffer> _vbo        = nullptr;
    std::shared_ptr<moci::IndexBuffer> _ibo         = nullptr;
    std::shared_ptr<moci::VertexArray> _vao         = nullptr;
    std::shared_ptr<moci::Texture2D> _texture       = nullptr;
    std::shared_ptr<moci::Framebuffer> _framebuffer = nullptr;
};
