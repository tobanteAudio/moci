#pragma once

#include "moci/moci.hpp"

class OpenGLLayer final : public moci::Layer
{
public:
    OpenGLLayer()           = default;
    ~OpenGLLayer() override = default;

    void OnAttach() override;
    void OnUpdate(moci::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(moci::Event& e) override;

private:
    bool handleWindowResized(moci::WindowResizeEvent& e);
    bool handleKeyPressed(moci::KeyPressedEvent& e);
    bool handleMouseScrolled(moci::MouseScrolledEvent& e);
    bool handleMouseMoved(moci::MouseMovedEvent& e);
    bool handleMousePressed(moci::MouseButtonPressedEvent& e);
    bool handleMouseReleased(moci::MouseButtonReleasedEvent& e);

    float width_  = 1920.0f;
    float height_ = 1080.0f;

    moci::Ref<moci::Shader> shader_           = nullptr;
    moci::Ref<moci::VertexBuffer> vbo_        = nullptr;
    moci::Ref<moci::IndexBuffer> ibo_         = nullptr;
    moci::Ref<moci::VertexArray> vao_         = nullptr;
    moci::Ref<moci::Texture2D> texture_       = nullptr;
    moci::Ref<moci::Framebuffer> framebuffer_ = nullptr;
};