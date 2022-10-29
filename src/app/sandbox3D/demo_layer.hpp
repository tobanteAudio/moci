#pragma once

#include <moci/events/layer/layer.hpp>
#include <moci/events/types/application_event.hpp>
#include <moci/events/types/key_event.hpp>
#include <moci/events/types/mouse_event.hpp>
#include <moci/graphics_extra/light.hpp>
#include <moci/render/frame_buffer.hpp>
#include <moci/render/render_factory.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"

class DemoLayer : public moci::Layer
{
public:
    DemoLayer()           = default;
    ~DemoLayer() override = default;

    void OnAttach() override;
    void OnUpdate(moci::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(moci::Event& e) override;

private:
    bool OnWindowResized(moci::WindowResizeEvent& e);
    bool OnKeyPressed(moci::KeyPressedEvent& e);
    bool OnMouseScrolled(moci::MouseScrolledEvent& e);
    bool OnMouseMoved(moci::MouseMovedEvent& e);
    bool OnMousePressed(moci::MouseButtonPressedEvent& e);
    bool OnMouseReleased(moci::MouseButtonReleasedEvent& e);

    float width_        = 1280.0F;
    float height_       = 1024.0F;
    float lastTimestep_ = 0.0F;
    bool fullscreen_    = false;

    moci::Ref<moci::Framebuffer> framebuffer_ = nullptr;
    glm::vec2 viewportSize_                   = {};

    glm::vec3 cameraPos_   = {15.0F, 8.70F, 50.0F};
    glm::vec3 cameraFront_ = {0.0F, 0.0F, -1.0F};
    glm::vec3 cameraUp_    = {0.0F, 1.0F, 0.0F};
    float cameraYaw_       = -90.0F;
    float cameraPitch_     = 0.0F;
    float cameraFOV_       = 45.0F;
    bool firstMouse_       = true;
    float cameraLastX_     = {width_ / 2.0F};
    float cameraLastY_     = {height_ / 2.0F};
    bool isMouseDragging_  = false;

    glm::vec3 cameraLookAt_        = {0.0F, 0.0F, 0.0F};
    float ambientLight_            = 0.1F;
    float modelScale_              = 0.15F;
    moci::Scope<moci::Light> light = {};

    moci::Ref<moci::Shader> shader_    = nullptr;
    moci::Ref<moci::VertexBuffer> vbo_ = nullptr;
    moci::Ref<moci::IndexBuffer> ibo_  = nullptr;
    moci::Ref<moci::VertexArray> vao_  = nullptr;

    std::size_t numVertices_ = {};
    moci::Mesh mesh_ {"sandbox3D/assets/models/cerberus.fbx"};
    moci::Mesh floor_ {"sandbox3D/assets/models/plane.obj"};

    moci::Ref<moci::Texture2D> textureSolid_  = {};
    moci::Ref<moci::Texture2D> textureColors_ = {};

    moci::Ref<moci::Shader> skyboxShader_       = nullptr;
    moci::Ref<moci::VertexBuffer> skyboxVbo_    = nullptr;
    moci::Ref<moci::IndexBuffer> skyboxIbo_     = nullptr;
    moci::Ref<moci::VertexArray> skyboxVao_     = nullptr;
    moci::Ref<moci::TextureCube> skyBoxTexture_ = {};

    moci::Vector<moci::Mesh::Vertex> vertices_ = {};

    // imgui
    struct DrawStats
    {
        std::uint32_t numVertices {};
        float minFPS = 9999.0F;
        float maxFPS = 0.0F;
    };

    DrawStats drawStats_            = {};
    moci::Vector<float> fpsHistory_ = {};
    bool imguiWindow_               = true;
    bool imguiDemo_                 = false;
};
