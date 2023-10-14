#include "demo_layer.hpp"

#include <moci/application/application.hpp>
#include <moci/render/opengl/es2/es2.hpp>

#include "imgui.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

#include "glm/gtx/quaternion.hpp"

#include <array>

namespace {
#if defined(MOCI_API_OPENGL_LEGACY)
constexpr auto shaderPrefix = "es2";
#else
constexpr auto shaderPrefix = "gl4";
#endif

auto skyboxVertices = std::array{
    // positions
    -1.0F, 1.0F,  -1.0F,  //
    -1.0F, -1.0F, -1.0F,  //
    1.0F,  -1.0F, -1.0F,  //
    1.0F,  -1.0F, -1.0F,  //
    1.0F,  1.0F,  -1.0F,  //
    -1.0F, 1.0F,  -1.0F,  //

    -1.0F, -1.0F, 1.0F,   //
    -1.0F, -1.0F, -1.0F,  //
    -1.0F, 1.0F,  -1.0F,  //
    -1.0F, 1.0F,  -1.0F,  //
    -1.0F, 1.0F,  1.0F,   //
    -1.0F, -1.0F, 1.0F,   //

    1.0F,  -1.0F, -1.0F,  //
    1.0F,  -1.0F, 1.0F,   //
    1.0F,  1.0F,  1.0F,   //
    1.0F,  1.0F,  1.0F,   //
    1.0F,  1.0F,  -1.0F,  //
    1.0F,  -1.0F, -1.0F,  //

    -1.0F, -1.0F, 1.0F,  //
    -1.0F, 1.0F,  1.0F,  //
    1.0F,  1.0F,  1.0F,  //
    1.0F,  1.0F,  1.0F,  //
    1.0F,  -1.0F, 1.0F,  //
    -1.0F, -1.0F, 1.0F,  //

    -1.0F, 1.0F,  -1.0F,  //
    1.0F,  1.0F,  -1.0F,  //
    1.0F,  1.0F,  1.0F,   //
    1.0F,  1.0F,  1.0F,   //
    -1.0F, 1.0F,  1.0F,   //
    -1.0F, 1.0F,  -1.0F,  //

    -1.0F, -1.0F, -1.0F,  //
    -1.0F, -1.0F, 1.0F,   //
    1.0F,  -1.0F, -1.0F,  //
    1.0F,  -1.0F, -1.0F,  //
    -1.0F, -1.0F, 1.0F,   //
    1.0F,  -1.0F, 1.0F    //
};

auto imGuiToGlmVec(ImVec2 const& input) -> glm::vec2 { return {input.x, input.y}; }

}  // namespace

void DemoLayer::onAttach()
{
    MOCI_PROFILE_FUNCTION();

    auto const numVertices = (_mesh.getVertices().size() * 5) + _floor.getVertices().size();
    _vertices.reserve(numVertices);

    auto const path
        = fmt::format("src/app/sandbox3D/assets/shader/{}_general.glsl", shaderPrefix);
    _shader = moci::RenderFactory::makeShader(path);
    _shader->bind();

    {
        MOCI_PROFILE_SCOPE("Translate");
        for (auto const& vertex : _mesh.getVertices()) {
            auto const model = glm::translate(glm::mat4(1.0F), glm::vec3(1.0F));
            auto const scaleMatrix
                = glm::scale(glm::mat4(1.0F), {_modelScale, _modelScale, _modelScale});
            auto const position = model * scaleMatrix * glm::vec4(vertex.position, 1.0F);
            _vertices.push_back(
                {glm::vec3(position), vertex.normal, vertex.color, vertex.texCoord}
            );
        }

        for (auto x = 1; x <= 5; x++) {
            auto const modelScale = _modelScale / 2.0F;
            auto const rotate
                = glm::rotate(glm::mat4(1.0F), glm::radians(-90.0F), {1.0F, 0.0F, 0.0F});
            auto const scale
                = glm::scale(glm::mat4(1.0F), {modelScale, modelScale, modelScale});
            auto const model
                = glm::translate(glm::mat4(1.0F), glm::vec3((3.0F * x), 5.0F, 1.0F));
            for (auto const& vertex : _mesh.getVertices()) {
                auto const position
                    = model * scale * rotate * glm::vec4(vertex.position, 1.0F);
                _vertices.push_back(
                    {glm::vec3(position), vertex.normal, vertex.color, vertex.texCoord}
                );
            }
        }

        for (auto const& vertex : _floor.getVertices()) {
            auto const model       = glm::translate(glm::mat4(1.0F), glm::vec3(1.0F));
            auto const scaleMatrix = glm::scale(glm::mat4(1.0F), {5.0F, 5.0F, 5.0F});
            auto const position    = model * scaleMatrix * glm::vec4(vertex.position, 1.0F);
            _vertices.push_back({
                glm::vec3(position),
                vertex.normal,
                {1.0F, 1.0F, 0.5F, 1.0F},
                vertex.texCoord
            });
        }

        _numVertices = _vertices.size();
    }

    auto const skyBoxShaderPath
        = fmt::format("src/app/sandbox3D/assets/shader/{}_skybox.glsl", shaderPrefix);
    _skyboxShader = moci::RenderFactory::makeShader(skyBoxShaderPath);
    _skyboxShader->bind();
    _skyboxShader->setInt("u_Skybox", 0);
    moci::BufferLayout const skyBoxLayout = {
        {moci::ShaderDataType::Float3, "a_Position"}
    };
    auto* skyBoxData = reinterpret_cast<float*>(skyboxVertices.data());
    auto const skyBoxSize
        = static_cast<std::uint32_t>(skyboxVertices.size() * sizeof(float));
    _skyboxVbo.reset(moci::RenderFactory::makeVertexBuffer(skyBoxData, skyBoxSize, false));
    _skyboxVbo->setLayout(skyBoxLayout);
    _skyboxVbo->unbind();
    _skyboxIbo.reset(moci::RenderFactory::makeIndexBuffer({{}, 1, true}));
    _skyboxIbo->unbind();
    _skyboxVao = moci::RenderFactory::makeVertexArray();
    _skyboxVao->addVertexBuffer(_skyboxVbo);
    _skyboxVao->setIndexBuffer(_skyboxIbo);
    _skyboxVao->unbind();
    std::vector<std::string> const faces{
        std::string("src/app/sandbox3D/assets/textures/skybox/right.jpg"),   //
        std::string("src/app/sandbox3D/assets/textures/skybox/left.jpg"),    //
        std::string("src/app/sandbox3D/assets/textures/skybox/top.jpg"),     //
        std::string("src/app/sandbox3D/assets/textures/skybox/bottom.jpg"),  //
        std::string("src/app/sandbox3D/assets/textures/skybox/front.jpg"),   //
        std::string("src/app/sandbox3D/assets/textures/skybox/back.jpg")     //
    };

    _skyBoxTexture = moci::RenderFactory::makeTextureCube(faces);

    moci::BufferLayout const layout = {
        {moci::ShaderDataType::Float3, "a_Position" }, //
        {moci::ShaderDataType::Float3, "a_Normal"   }, //
        {moci::ShaderDataType::Float4, "a_Color"    }, //
        {moci::ShaderDataType::Float2, "a_TexCoords"}, //
    };
    auto* data = reinterpret_cast<float*>(_vertices.data());
    auto const size
        = static_cast<std::uint32_t>(_vertices.size() * sizeof(moci::Mesh::Vertex));
    _vbo.reset(moci::RenderFactory::makeVertexBuffer(data, size, false));
    _vbo->setLayout(layout);
    _vbo->unbind();
    _ibo.reset(moci::RenderFactory::makeIndexBuffer({{}, 1, true}));
    _ibo->unbind();
    _vao = moci::RenderFactory::makeVertexArray();
    _vao->addVertexBuffer(_vbo);
    _vao->setIndexBuffer(_ibo);
    _vao->unbind();

    // light
    _light = std::make_unique<moci::Light>();

    _textureSolid = moci::RenderFactory::makeTexture2D(
        "src/app/sandbox3D/assets/textures/white_10x10.png"
    );
    _textureColors
        = moci::RenderFactory::makeTexture2D("src/app/sandbox3D/assets/textures/4color.png"
        );
    _textureColors = moci::RenderFactory::makeTexture2D(
        "src/app/sandbox3D/assets/textures/cerberus_A_4096x4096.png"
    );

    auto fbSpec   = moci::FramebufferSpecs{};
    fbSpec.width  = 1920;
    fbSpec.height = 1080;
    _framebuffer  = moci::RenderFactory::makeFramebuffer(fbSpec);

    _fpsHistory.reserve(10'000);
}

void DemoLayer::onUpdate(moci::Timestep ts)
{
    MOCI_PROFILE_FUNCTION();
    _lastTimestep          = ts.getMilliseconds();
    _drawStats.numVertices = 0;
    moci::RenderCommand::setClearColor({0.1F, 0.1F, 0.1F, 1});
    moci::RenderCommand::clear();

    _framebuffer->bind();
    moci::RenderCommand::setClearColor({0.1F, 0.1F, 0.1F, 1.0F});
    moci::RenderCommand::clear();

    // Camera matrix
    auto const aspectRatio = _viewportSize.x / _viewportSize.y;
    auto const projection
        = glm::perspective(glm::radians(_cameraFOV), aspectRatio, 0.1F, 100.0F);
    auto const view = glm::lookAt(  //
        _cameraPos,                 // Camera is at (x,y,z), in World Space
        _cameraPos + _cameraFront,  // and looks at
        _cameraUp                   // Head is up (set to 0,-1,0 to look upside-down)
    );

    {
        MOCI_PROFILE_SCOPE("OnUpdate::Mesh");
        _shader->bind();
        _shader->setMat4("u_View", view);
        _shader->setMat4("u_Projection", projection);
        _shader->setFloat("u_Ambient", _ambientLight);
        _shader->setFloat3("u_LightPos", _light->position);
        _shader->setFloat3("u_LightColor", glm::vec3(_light->color));
        _shader->setFloat3("u_ViewPos", _cameraPos);

        _vao->bind();
        _textureColors->bind(0);
        moci::RenderCommand::drawArrays(
            moci::RenderDrawMode::Triangles,
            0,
            static_cast<std::uint32_t>(_vertices.size())
        );
        _drawStats.numVertices += static_cast<std::uint32_t>(_vertices.size());
        _textureColors->unbind();
    }

    {
        MOCI_PROFILE_SCOPE("OnUpdate::Light");
        _light->render(view, projection);
    }

    // skybox
    // change depth function so depth test passes when values are equal to depth buffer's
    // content
    GLCall(glDepthFunc(GL_LEQUAL));
    _skyboxShader->bind();
    auto const skyboxView = glm::mat4(glm::mat3(view));
    _skyboxShader->setMat4("u_View", skyboxView);
    _skyboxShader->setMat4("u_Projection", projection);
    _skyboxShader->setInt("u_Skybox", 0);
    // skybox cube
    _skyboxVao->bind();
    _skyBoxTexture->bind(0);
    moci::RenderCommand::drawArrays(moci::RenderDrawMode::Triangles, 0, 36);
    _skyboxVao->unbind();
    GLCall(glDepthFunc(GL_LESS));  // set depth function back to default

    _framebuffer->unbind();
}

void DemoLayer::onEvent(moci::Event& e)
{
    moci::EventDispatcher dispatcher(e);
    dispatcher.dispatch<moci::WindowResizeEvent>(
        MOCI_EVENT_METHOD(DemoLayer::onWindowResized)
    );
    dispatcher.dispatch<moci::KeyPressedEvent>(MOCI_EVENT_METHOD(DemoLayer::onKeyPressed));
    dispatcher.dispatch<moci::MouseScrolledEvent>(
        MOCI_EVENT_METHOD(DemoLayer::onMouseScrolled)
    );
    dispatcher.dispatch<moci::MouseMovedEvent>(MOCI_EVENT_METHOD(DemoLayer::onMouseMoved));
    dispatcher.dispatch<moci::MouseButtonPressedEvent>(
        MOCI_EVENT_METHOD(DemoLayer::onMousePressed)
    );
    dispatcher.dispatch<moci::MouseButtonReleasedEvent>(
        MOCI_EVENT_METHOD(DemoLayer::onMouseReleased)
    );
}

auto DemoLayer::onWindowResized(moci::WindowResizeEvent& e) -> bool
{
    _width  = static_cast<float>(e.getWidth());
    _height = static_cast<float>(e.getHeight());
    return false;
}

auto DemoLayer::onKeyPressed(moci::KeyPressedEvent& e) -> bool
{
    auto const cameraSpeed = 0.15F * _lastTimestep;
    if (e.getKeyCode() == moci::Key::Up) {
        _cameraPos += cameraSpeed * _cameraFront;
    }
    if (e.getKeyCode() == moci::Key::Down) {
        _cameraPos -= cameraSpeed * _cameraFront;
    }
    if (e.getKeyCode() == moci::Key::Left) {
        _cameraPos -= glm::normalize(glm::cross(_cameraFront, _cameraUp)) * cameraSpeed;
    }
    if (e.getKeyCode() == moci::Key::Right) {
        _cameraPos += glm::normalize(glm::cross(_cameraFront, _cameraUp)) * cameraSpeed;
    }

    return true;
}

auto DemoLayer::onMouseScrolled(moci::MouseScrolledEvent& e) -> bool
{
    _cameraFOV -= e.getYOffset();
    constexpr auto minFOV = 1.0F;
    constexpr auto maxFOV = 75.0F;
    if (_cameraFOV < minFOV) {
        _cameraFOV = minFOV;
    }
    if (_cameraFOV > maxFOV) {
        _cameraFOV = maxFOV;
    }
    return true;
}

auto DemoLayer::onMousePressed(moci::MouseButtonPressedEvent& e) -> bool
{
    if (e.getMouseButton() == moci::MouseCode::ButtonMiddle) {
        _isMouseDragging = true;
        return true;
    }

    return false;
}

auto DemoLayer::onMouseReleased(moci::MouseButtonReleasedEvent& e) -> bool
{
    if (e.getMouseButton() == moci::MouseCode::ButtonMiddle) {
        _isMouseDragging = false;
        _firstMouse      = true;
        return true;
    }

    return false;
}

auto DemoLayer::onMouseMoved(moci::MouseMovedEvent& e) -> bool
{
    if (_isMouseDragging) {
        if (_firstMouse) {
            _cameraLastX = e.getX();
            _cameraLastY = e.getY();
            _firstMouse  = false;
        }

        auto xOffset = e.getX() - _cameraLastX;
        auto yOffset = _cameraLastY - e.getY();
        _cameraLastX = e.getX();
        _cameraLastY = e.getY();

        auto const sensitifity = 0.1F;
        xOffset *= sensitifity;
        yOffset *= sensitifity;

        _cameraYaw += xOffset;
        _cameraPitch += yOffset;

        if (_cameraPitch > 89.0F) {
            _cameraPitch = 89.0F;
        }

        if (_cameraPitch < -89.0F) {
            _cameraPitch = -89.0F;
        }

        auto front = glm::vec3{};
        front.x = glm::cos(glm::radians(_cameraYaw)) * glm::cos(glm::radians(_cameraPitch));
        front.y = glm::sin(glm::radians(_cameraPitch));
        front.z = glm::sin(glm::radians(_cameraYaw)) * glm::cos(glm::radians(_cameraPitch));
        _cameraFront = glm::normalize(front);
    }

    return true;
}

void DemoLayer::onImGuiRender()
{
    auto const fps        = ImGui::GetIO().Framerate;
    auto const frameCount = moci::Application::get().getWindow().getFrameCount();
    if (frameCount >= 100) {
        if (fps < _drawStats.minFPS) {
            _drawStats.minFPS = fps;
        }
        if (fps > _drawStats.maxFPS) {
            _drawStats.maxFPS = fps;
        }
    }

    auto const fpsStr    = fmt::format("FPS: {0:0.1f}", fps);
    auto const minFPSStr = fmt::format("Min: {0:0.1f}", _drawStats.minFPS);
    auto const maxFPSStr = fmt::format("Max: {0:0.1f}", _drawStats.maxFPS);
    auto const ts        = fmt::format("Timestep: {0:0.1f}", _lastTimestep);

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Debug")) {
            ImGui::Checkbox("Sandbox3D", &_imguiWindow);
            ImGui::Checkbox("Imgui Demo", &_imguiDemo);
            ImGui::Checkbox("Fullscreen", &_fullscreen);
            moci::Application::get().getWindow().setFullscreen(_fullscreen);

            ImGui::EndMenu();
        }

        ImGui::TextUnformatted(fpsStr.c_str());
        ImGui::TextUnformatted(minFPSStr.c_str());
        ImGui::TextUnformatted(maxFPSStr.c_str());
        ImGui::TextUnformatted(ts.c_str());
        ImGui::EndMenuBar();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");
    auto const viewportRegion = imGuiToGlmVec(ImGui::GetContentRegionAvail());
    if (_viewportSize != viewportRegion) {
        _viewportSize        = viewportRegion;
        auto const newWidth  = static_cast<std::uint32_t>(_viewportSize.x);
        auto const newHeight = static_cast<std::uint32_t>(_viewportSize.y);
        _framebuffer->resize(newWidth, newHeight);
    }

    auto* const textureID
        = reinterpret_cast<void*>((size_t)_framebuffer->getColorAttachmentRendererId());
    ImGui::Image(
        textureID,
        {viewportRegion.x, viewportRegion.y},
        ImVec2(0, 1),
        ImVec2(1, 0)
    );
    ImGui::End();
    ImGui::PopStyleVar();

    if (_imguiWindow) {
        ImGui::Begin("Sandbox 3D", &_imguiWindow);
        if (ImGui::CollapsingHeader("Camera")) {
            ImGui::SliderFloat3("Position", glm::value_ptr(_cameraPos), -100.0F, 100.0F);
            ImGui::SliderFloat3("Look At", glm::value_ptr(_cameraLookAt), -10.0F, 10.0F);
            ImGui::SliderFloat("FOV", &_cameraFOV, 5.0F, 85.0F);
        }

        if (ImGui::CollapsingHeader("Light")) {

            ImGui::SliderFloat("Ambient", &_ambientLight, 0.0F, 0.4F);
            ImGui::SliderFloat3(
                "Light Position",
                glm::value_ptr(_light->position),
                -20.0F,
                20.0F
            );
            ImGui::ColorEdit4("Light Color", glm::value_ptr(_light->color), 0);
            ImGui::SliderFloat("Light Scale", &_light->scale, 0.1F, 1.0F);
        }

        if (ImGui::CollapsingHeader("Model")) {
            ImGui::SliderFloat("Scale", &_modelScale, 0.01F, 20.0F);
        }

        if (ImGui::CollapsingHeader("Stats")) {
            _fpsHistory.push_back(fps);
            auto const frameCounter = fmt::format("{} Frames", frameCount);
            auto const vertices     = fmt::format("{} Vertices", _drawStats.numVertices);
            auto const triangles = fmt::format("{} Triangles", _drawStats.numVertices / 3);
            auto const mb
                = _drawStats.numVertices * sizeof(moci::Mesh::Vertex) / 1'000'000.0F;
            auto const megabyte = fmt::format("{0:0.1f} Mbytes", mb);
            ImGui::TextUnformatted(frameCounter.c_str());
            ImGui::TextUnformatted(fpsStr.c_str());
            ImGui::TextUnformatted(minFPSStr.c_str());
            ImGui::TextUnformatted(maxFPSStr.c_str());
            ImGui::TextUnformatted(vertices.c_str());
            ImGui::TextUnformatted(triangles.c_str());
            ImGui::TextUnformatted(megabyte.c_str());
            // auto const lightStr = fmt::format("Light vertices: {}",
            // lightMesh_.getVertices().size()); ImGui::TextUnformatted(lightStr.c_str());
            auto const size = static_cast<int>(_fpsHistory.size());
            ImGui::PlotLines(
                "FPS",
                _fpsHistory.data(),
                size,
                0,
                "",
                50.0F,
                75.0F,
                ImVec2(0, 80)
            );
        }
        ImGui::End();
    }

    // ImGui Demo
    if (_imguiDemo) {
        ImGui::ShowDemoWindow(&_imguiDemo);
    }
}
