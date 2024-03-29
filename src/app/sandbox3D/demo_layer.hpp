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

    void onAttach() override;
    void onUpdate(moci::Timestep ts) override;
    void onImGuiRender() override;
    void onEvent(moci::Event& e) override;

private:
    auto onWindowResized(moci::WindowResizeEvent& e) -> bool;
    auto onKeyPressed(moci::KeyPressedEvent& e) -> bool;
    auto onMouseScrolled(moci::MouseScrolledEvent& e) -> bool;
    auto onMouseMoved(moci::MouseMovedEvent& e) -> bool;
    auto onMousePressed(moci::MouseButtonPressedEvent& e) -> bool;
    auto onMouseReleased(moci::MouseButtonReleasedEvent& e) -> bool;

    float _width        = 1280.0F;
    float _height       = 1024.0F;
    float _lastTimestep = 0.0F;
    bool _fullscreen    = false;

    std::shared_ptr<moci::Framebuffer> _framebuffer = nullptr;
    glm::vec2 _viewportSize                         = {};

    glm::vec3 _cameraPos   = {15.0F, 8.70F, 50.0F};
    glm::vec3 _cameraFront = {0.0F, 0.0F, -1.0F};
    glm::vec3 _cameraUp    = {0.0F, 1.0F, 0.0F};
    float _cameraYaw       = -90.0F;
    float _cameraPitch     = 0.0F;
    float _cameraFOV       = 45.0F;
    bool _firstMouse       = true;
    float _cameraLastX     = {_width / 2.0F};
    float _cameraLastY     = {_height / 2.0F};
    bool _isMouseDragging  = false;

    glm::vec3 _cameraLookAt             = {0.0F, 0.0F, 0.0F};
    float _ambientLight                 = 0.1F;
    float _modelScale                   = 0.15F;
    std::unique_ptr<moci::Light> _light = {};

    std::shared_ptr<moci::Shader> _shader    = nullptr;
    std::shared_ptr<moci::VertexBuffer> _vbo = nullptr;
    std::shared_ptr<moci::IndexBuffer> _ibo  = nullptr;
    std::shared_ptr<moci::VertexArray> _vao  = nullptr;

    std::size_t _numVertices = {};
    moci::Mesh _mesh{"src/app/sandbox3D/assets/models/cerberus.fbx"};
    moci::Mesh _floor{"src/app/sandbox3D/assets/models/plane.obj"};

    std::shared_ptr<moci::Texture2D> _textureSolid  = {};
    std::shared_ptr<moci::Texture2D> _textureColors = {};

    std::shared_ptr<moci::Shader> _skyboxShader       = nullptr;
    std::shared_ptr<moci::VertexBuffer> _skyboxVbo    = nullptr;
    std::shared_ptr<moci::IndexBuffer> _skyboxIbo     = nullptr;
    std::shared_ptr<moci::VertexArray> _skyboxVao     = nullptr;
    std::shared_ptr<moci::TextureCube> _skyBoxTexture = {};

    std::vector<moci::Mesh::Vertex> _vertices = {};

    // imgui
    struct DrawStats
    {
        std::uint32_t numVertices{};
        float minFPS = 9999.0F;
        float maxFPS = 0.0F;
    };

    DrawStats _drawStats           = {};
    std::vector<float> _fpsHistory = {};
    bool _imguiWindow              = true;
    bool _imguiDemo                = false;
};
