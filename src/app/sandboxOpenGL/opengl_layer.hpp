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
    bool handleWindowResized(moci::WindowResizeEvent& e);
    static bool handleKeyPressed(moci::KeyPressedEvent& e);
    static bool handleMouseScrolled(moci::MouseScrolledEvent& e);
    static bool handleMouseMoved(moci::MouseMovedEvent& e);
    static bool handleMousePressed(moci::MouseButtonPressedEvent& e);
    static bool handleMouseReleased(moci::MouseButtonReleasedEvent& e);

    float _width  = 1920.0F;
    float _height = 1080.0F;

    bool _framebufferNeedsResize = false;
    glm::vec2 _viewportSize      = {};

    moci::Ref<moci::Shader> _shader           = nullptr;
    moci::Ref<moci::VertexBuffer> _vbo        = nullptr;
    moci::Ref<moci::IndexBuffer> _ibo         = nullptr;
    moci::Ref<moci::VertexArray> _vao         = nullptr;
    moci::Ref<moci::Texture2D> _texture       = nullptr;
    moci::Ref<moci::Framebuffer> _framebuffer = nullptr;
};