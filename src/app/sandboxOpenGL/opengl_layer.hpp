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

    float width_  = 1920.0F;
    float height_ = 1080.0F;

    bool framebufferNeedsResize_ = false;
    glm::vec2 viewportSize_      = {};

    moci::Ref<moci::Shader> shader_           = nullptr;
    moci::Ref<moci::VertexBuffer> vbo_        = nullptr;
    moci::Ref<moci::IndexBuffer> ibo_         = nullptr;
    moci::Ref<moci::VertexArray> vao_         = nullptr;
    moci::Ref<moci::Texture2D> texture_       = nullptr;
    moci::Ref<moci::Framebuffer> framebuffer_ = nullptr;
};