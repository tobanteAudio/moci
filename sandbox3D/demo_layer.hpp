#pragma once

#include "moci/moci.hpp"

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

private:
    float width_        = 1280.0f;
    float height_       = 1024.0f;
    float lastTimestep_ = 0.0f;
    bool fullscreen_    = false;

    glm::vec3 cameraPos_   = {15.0f, 8.70f, 50.0f};
    glm::vec3 cameraFront_ = {0.0f, 0.0f, -1.0f};
    glm::vec3 cameraUp_    = {0.0f, 1.0f, 0.0f};
    float cameraYaw_       = -90.0f;
    float cameraPitch_     = 0.0f;
    float cameraFOV_       = 45.0f;
    bool firstMouse_       = true;
    float cameraLastX_     = {width_ / 2.0f};
    float cameraLastY_     = {height_ / 2.0f};
    bool isMouseDragging_  = false;

    glm::vec3 cameraLookAt_        = {0.0f, 0.0f, 0.0f};
    float ambientLight_            = 0.1f;
    float modelScale_              = 0.15f;
    moci::Scope<moci::Light> light = {};

    moci::Ref<moci::Shader> shader_    = nullptr;
    moci::Ref<moci::VertexBuffer> vbo_ = nullptr;
    moci::Ref<moci::IndexBuffer> ibo_  = nullptr;
    moci::Ref<moci::VertexArray> vao_  = nullptr;

    std::size_t numVertices_ = {};
    moci::Mesh mesh_         = {"sandbox3D/assets/models/cerberus.fbx"};
    moci::Mesh floor_        = {"sandbox3D/assets/models/plane.obj"};

    moci::Ref<moci::Texture2D> textureSolid_  = {};
    moci::Ref<moci::Texture2D> textureColors_ = {};

    moci::Ref<moci::Shader> skyboxShader_       = nullptr;
    moci::Ref<moci::VertexBuffer> skyboxVbo_    = nullptr;
    moci::Ref<moci::IndexBuffer> skyboxIbo_     = nullptr;
    moci::Ref<moci::VertexArray> skyboxVao_     = nullptr;
    moci::Ref<moci::TextureCube> skyBoxTexture_ = {};

    std::vector<moci::Mesh::Vertex> vertices_ = {};

    // imgui
    struct DrawStats
    {
        std::uint32_t numVertices {};
        float minFPS = 9999.0f;
        float maxFPS = 0.0f;
    };

    DrawStats drawStats_           = {};
    std::vector<float> fpsHistory_ = {};
    bool imguiWindow_              = true;
    bool imguiDemo_                = false;
};