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

namespace
{
#if defined(MOCI_API_OPENGL_LEGACY)
constexpr auto ShaderPrefix = "es2";
#else
constexpr auto shaderPrefix = "gl4";
#endif

auto skyboxVertices = std::array {
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

glm::vec2 imGuiToGlmVec(ImVec2 const& input) { return {input.x, input.y}; }

}  // namespace

void DemoLayer::onAttach()
{
    MOCI_PROFILE_FUNCTION();

    auto const numVertices = (mesh_.getVertices().size() * 5) + floor_.getVertices().size();
    vertices_.reserve(numVertices);

    auto const path = fmt::format("sandbox3D/assets/shader/{}_general.glsl", ShaderPrefix);
    shader_         = moci::RenderFactory::makeShader(path);
    shader_->bind();

    {
        MOCI_PROFILE_SCOPE("Translate");
        for (auto const& vertex : mesh_.getVertices())
        {
            auto const model       = glm::translate(glm::mat4(1.0F), glm::vec3(1.0F));
            auto const scaleMatrix = glm::scale(glm::mat4(1.0F), {modelScale_, modelScale_, modelScale_});
            auto const position    = model * scaleMatrix * glm::vec4(vertex.position, 1.0F);
            vertices_.push_back({glm::vec3(position), vertex.normal, vertex.color, vertex.texCoord});
        }

        for (auto x = 1; x <= 5; x++)
        {
            auto const modelScale = modelScale_ / 2.0F;
            auto const rotate     = glm::rotate(glm::mat4(1.0F), glm::radians(-90.0F), {1.0F, 0.0F, 0.0F});
            auto const scale      = glm::scale(glm::mat4(1.0F), {modelScale, modelScale, modelScale});
            auto const model      = glm::translate(glm::mat4(1.0F), glm::vec3((3.0F * x), 5.0F, 1.0F));
            for (auto const& vertex : mesh_.getVertices())
            {
                auto const position = model * scale * rotate * glm::vec4(vertex.position, 1.0F);
                vertices_.push_back({glm::vec3(position), vertex.normal, vertex.color, vertex.texCoord});
            }
        }

        for (auto const& vertex : floor_.getVertices())
        {
            auto const model       = glm::translate(glm::mat4(1.0F), glm::vec3(1.0F));
            auto const scaleMatrix = glm::scale(glm::mat4(1.0F), {5.0F, 5.0F, 5.0F});
            auto const position    = model * scaleMatrix * glm::vec4(vertex.position, 1.0F);
            vertices_.push_back({glm::vec3(position), vertex.normal, {1.0F, 1.0F, 0.5F, 1.0F}, vertex.texCoord});
        }

        numVertices_ = vertices_.size();
    }

    auto const skyBoxShaderPath = fmt::format("sandbox3D/assets/shader/{}_skybox.glsl", ShaderPrefix);
    skyboxShader_               = moci::RenderFactory::makeShader(skyBoxShaderPath);
    skyboxShader_->bind();
    skyboxShader_->setInt("u_Skybox", 0);
    moci::BufferLayout skyBoxLayout = {{moci::ShaderDataType::Float3, "a_Position"}};
    auto* skyBoxData                = reinterpret_cast<float*>(skyboxVertices.data());
    auto const skyBoxSize           = static_cast<std::uint32_t>(skyboxVertices.size() * sizeof(float));
    skyboxVbo_.reset(moci::RenderFactory::makeVertexBuffer(skyBoxData, skyBoxSize, false));
    skyboxVbo_->setLayout(skyBoxLayout);
    skyboxVbo_->unbind();
    skyboxIbo_.reset(moci::RenderFactory::makeIndexBuffer({{}, 1, true}));
    skyboxIbo_->unbind();
    skyboxVao_ = moci::RenderFactory::makeVertexArray();
    skyboxVao_->addVertexBuffer(skyboxVbo_);
    skyboxVao_->setIndexBuffer(skyboxIbo_);
    skyboxVao_->unbind();
    moci::Vector<std::string> faces {
        std::string("sandbox3D/assets/textures/skybox/right.jpg"),   //
        std::string("sandbox3D/assets/textures/skybox/left.jpg"),    //
        std::string("sandbox3D/assets/textures/skybox/top.jpg"),     //
        std::string("sandbox3D/assets/textures/skybox/bottom.jpg"),  //
        std::string("sandbox3D/assets/textures/skybox/front.jpg"),   //
        std::string("sandbox3D/assets/textures/skybox/back.jpg")     //
    };

    skyBoxTexture_ = moci::RenderFactory::makeTextureCube(faces);

    moci::BufferLayout layout = {
        {moci::ShaderDataType::Float3, "a_Position"},   //
        {moci::ShaderDataType::Float3, "a_Normal"},     //
        {moci::ShaderDataType::Float4, "a_Color"},      //
        {moci::ShaderDataType::Float2, "a_TexCoords"},  //
    };
    auto* data      = reinterpret_cast<float*>(vertices_.data());
    auto const size = static_cast<std::uint32_t>(vertices_.size() * sizeof(moci::Mesh::Vertex));
    vbo_.reset(moci::RenderFactory::makeVertexBuffer(data, size, false));
    vbo_->setLayout(layout);
    vbo_->unbind();
    ibo_.reset(moci::RenderFactory::makeIndexBuffer({{}, 1, true}));
    ibo_->unbind();
    vao_ = moci::RenderFactory::makeVertexArray();
    vao_->addVertexBuffer(vbo_);
    vao_->setIndexBuffer(ibo_);
    vao_->unbind();

    // light
    light = moci::makeScope<moci::Light>();

    textureSolid_  = moci::RenderFactory::makeTexture2D("sandbox3D/assets/textures/white_10x10.png");
    textureColors_ = moci::RenderFactory::makeTexture2D("sandbox3D/assets/textures/4color.png");
    textureColors_ = moci::RenderFactory::makeTexture2D("sandbox3D/assets/textures/cerberus_A_4096x4096.png");

    auto fbSpec   = moci::FramebufferSpecs {};
    fbSpec.width  = 1920;
    fbSpec.height = 1080;
    framebuffer_  = moci::RenderFactory::makeFramebuffer(fbSpec);

    fpsHistory_.reserve(10'000);
}

void DemoLayer::onUpdate(moci::Timestep ts)
{
    MOCI_PROFILE_FUNCTION();
    lastTimestep_          = ts.getMilliseconds();
    drawStats_.numVertices = 0;
    moci::RenderCommand::setClearColor({0.1F, 0.1F, 0.1F, 1});
    moci::RenderCommand::clear();

    framebuffer_->bind();
    moci::RenderCommand::setClearColor({0.1F, 0.1F, 0.1F, 1.0F});
    moci::RenderCommand::clear();

    // Camera matrix
    auto const aspectRatio = viewportSize_.x / viewportSize_.y;
    auto const projection  = glm::perspective(glm::radians(cameraFOV_), aspectRatio, 0.1F, 100.0F);
    auto const view        = glm::lookAt(  //
        cameraPos_,                 // Camera is at (x,y,z), in World Space
        cameraPos_ + cameraFront_,  // and looks at
        cameraUp_                   // Head is up (set to 0,-1,0 to look upside-down)
    );

    {
        MOCI_PROFILE_SCOPE("OnUpdate::Mesh");
        shader_->bind();
        shader_->setMat4("u_View", view);
        shader_->setMat4("u_Projection", projection);
        shader_->setFloat("u_Ambient", ambientLight_);
        shader_->setFloat3("u_LightPos", light->position);
        shader_->setFloat3("u_LightColor", glm::vec3(light->color));
        shader_->setFloat3("u_ViewPos", cameraPos_);

        vao_->bind();
        textureColors_->bind(0);
        moci::RenderCommand::drawArrays(moci::RenderDrawMode::Triangles, 0,
                                        static_cast<std::uint32_t>(vertices_.size()));
        drawStats_.numVertices += static_cast<std::uint32_t>(vertices_.size());
        textureColors_->unbind();
    }

    {
        MOCI_PROFILE_SCOPE("OnUpdate::Light");
        light->render(view, projection);
    }

    // skybox
    // change depth function so depth test passes when values are equal to depth buffer's content
    GLCall(glDepthFunc(GL_LEQUAL));
    skyboxShader_->bind();
    auto const skyboxView = glm::mat4(glm::mat3(view));
    skyboxShader_->setMat4("u_View", skyboxView);
    skyboxShader_->setMat4("u_Projection", projection);
    skyboxShader_->setInt("u_Skybox", 0);
    // skybox cube
    skyboxVao_->bind();
    skyBoxTexture_->bind(0);
    moci::RenderCommand::drawArrays(moci::RenderDrawMode::Triangles, 0, 36);
    skyboxVao_->unbind();
    GLCall(glDepthFunc(GL_LESS));  // set depth function back to default

    framebuffer_->unbind();
}

void DemoLayer::onEvent(moci::Event& e)
{
    moci::EventDispatcher dispatcher(e);
    dispatcher.dispatch<moci::WindowResizeEvent>(MOCI_EVENT_METHOD(DemoLayer::onWindowResized));
    dispatcher.dispatch<moci::KeyPressedEvent>(MOCI_EVENT_METHOD(DemoLayer::onKeyPressed));
    dispatcher.dispatch<moci::MouseScrolledEvent>(MOCI_EVENT_METHOD(DemoLayer::onMouseScrolled));
    dispatcher.dispatch<moci::MouseMovedEvent>(MOCI_EVENT_METHOD(DemoLayer::onMouseMoved));
    dispatcher.dispatch<moci::MouseButtonPressedEvent>(MOCI_EVENT_METHOD(DemoLayer::onMousePressed));
    dispatcher.dispatch<moci::MouseButtonReleasedEvent>(MOCI_EVENT_METHOD(DemoLayer::onMouseReleased));
}

bool DemoLayer::onWindowResized(moci::WindowResizeEvent& e)
{
    width_  = static_cast<float>(e.getWidth());
    height_ = static_cast<float>(e.getHeight());
    return false;
}

bool DemoLayer::onKeyPressed(moci::KeyPressedEvent& e)
{
    auto const cameraSpeed = 0.15F * lastTimestep_;
    if (e.getKeyCode() == moci::Key::Up) { cameraPos_ += cameraSpeed * cameraFront_; }
    if (e.getKeyCode() == moci::Key::Down) { cameraPos_ -= cameraSpeed * cameraFront_; }
    if (e.getKeyCode() == moci::Key::Left)
    {
        cameraPos_ -= glm::normalize(glm::cross(cameraFront_, cameraUp_)) * cameraSpeed;
    }
    if (e.getKeyCode() == moci::Key::Right)
    {
        cameraPos_ += glm::normalize(glm::cross(cameraFront_, cameraUp_)) * cameraSpeed;
    }

    return true;
}

bool DemoLayer::onMouseScrolled(moci::MouseScrolledEvent& e)
{
    cameraFOV_ -= e.getYOffset();
    constexpr auto minFOV = 1.0F;
    constexpr auto maxFOV = 75.0F;
    if (cameraFOV_ < minFOV) { cameraFOV_ = minFOV; }
    if (cameraFOV_ > maxFOV) { cameraFOV_ = maxFOV; }
    return true;
}

bool DemoLayer::onMousePressed(moci::MouseButtonPressedEvent& e)
{
    if (e.getMouseButton() == moci::MouseCode::ButtonMiddle)
    {
        isMouseDragging_ = true;
        return true;
    }

    return false;
}

bool DemoLayer::onMouseReleased(moci::MouseButtonReleasedEvent& e)
{
    if (e.getMouseButton() == moci::MouseCode::ButtonMiddle)
    {
        isMouseDragging_ = false;
        firstMouse_      = true;
        return true;
    }

    return false;
}

bool DemoLayer::onMouseMoved(moci::MouseMovedEvent& e)
{
    if (isMouseDragging_)
    {
        if (firstMouse_)
        {
            cameraLastX_ = e.getX();
            cameraLastY_ = e.getY();
            firstMouse_  = false;
        }

        auto xOffset = e.getX() - cameraLastX_;
        auto yOffset = cameraLastY_ - e.getY();
        cameraLastX_ = e.getX();
        cameraLastY_ = e.getY();

        auto const sensitifity = 0.1F;
        xOffset *= sensitifity;
        yOffset *= sensitifity;

        cameraYaw_ += xOffset;
        cameraPitch_ += yOffset;

        if (cameraPitch_ > 89.0F) { cameraPitch_ = 89.0F; }

        if (cameraPitch_ < -89.0F) { cameraPitch_ = -89.0F; }

        auto front   = glm::vec3 {};
        front.x      = glm::cos(glm::radians(cameraYaw_)) * glm::cos(glm::radians(cameraPitch_));
        front.y      = glm::sin(glm::radians(cameraPitch_));
        front.z      = glm::sin(glm::radians(cameraYaw_)) * glm::cos(glm::radians(cameraPitch_));
        cameraFront_ = glm::normalize(front);
    }

    return true;
}

void DemoLayer::onImGuiRender()
{
    auto const fps        = ImGui::GetIO().Framerate;
    auto const frameCount = moci::Application::get().getWindow().getFrameCount();
    if (frameCount >= 100)
    {
        if (fps < drawStats_.minFPS) { drawStats_.minFPS = fps; }
        if (fps > drawStats_.maxFPS) { drawStats_.maxFPS = fps; }
    }

    auto const fpsStr    = fmt::format("FPS: {0:0.1f}", fps);
    auto const minFPSStr = fmt::format("Min: {0:0.1f}", drawStats_.minFPS);
    auto const maxFPSStr = fmt::format("Max: {0:0.1f}", drawStats_.maxFPS);
    auto const ts        = fmt::format("Timestep: {0:0.1f}", lastTimestep_);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Debug"))
        {
            ImGui::Checkbox("Sandbox3D", &imguiWindow_);
            ImGui::Checkbox("Imgui Demo", &imguiDemo_);
            ImGui::Checkbox("Fullscreen", &fullscreen_);
            moci::Application::get().getWindow().setFullscreen(fullscreen_);

            ImGui::EndMenu();
        }

        ImGui::TextUnformatted(fpsStr.c_str());
        ImGui::TextUnformatted(minFPSStr.c_str());
        ImGui::TextUnformatted(maxFPSStr.c_str());
        ImGui::TextUnformatted(ts.c_str());
        ImGui::EndMenuBar();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 {0, 0});
    ImGui::Begin("Viewport");
    auto const viewportRegion = imGuiToGlmVec(ImGui::GetContentRegionAvail());
    if (viewportSize_ != viewportRegion)
    {
        viewportSize_        = viewportRegion;
        auto const newWidth  = static_cast<std::uint32_t>(viewportSize_.x);
        auto const newHeight = static_cast<std::uint32_t>(viewportSize_.y);
        framebuffer_->resize(newWidth, newHeight);
    }

    auto* const textureID = reinterpret_cast<void*>((size_t)framebuffer_->getColorAttachmentRendererId());
    ImGui::Image(textureID, {viewportRegion.x, viewportRegion.y}, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();

    if (imguiWindow_)
    {
        ImGui::Begin("Sandbox 3D", &imguiWindow_);
        if (ImGui::CollapsingHeader("Camera"))
        {
            ImGui::SliderFloat3("Position", glm::value_ptr(cameraPos_), -100.0F, 100.0F);
            ImGui::SliderFloat3("Look At", glm::value_ptr(cameraLookAt_), -10.0F, 10.0F);
            ImGui::SliderFloat("FOV", &cameraFOV_, 5.0F, 85.0F);
        }

        if (ImGui::CollapsingHeader("Light"))
        {

            ImGui::SliderFloat("Ambient", &ambientLight_, 0.0F, 0.4F);
            ImGui::SliderFloat3("Light Position", glm::value_ptr(light->position), -20.0F, 20.0F);
            ImGui::ColorEdit4("Light Color", glm::value_ptr(light->color), 0);
            ImGui::SliderFloat("Light Scale", &light->scale, 0.1F, 1.0F);
        }

        if (ImGui::CollapsingHeader("Model")) { ImGui::SliderFloat("Scale", &modelScale_, 0.01F, 20.0F); }

        if (ImGui::CollapsingHeader("Stats"))
        {
            fpsHistory_.push_back(fps);
            auto const frameCounter = fmt::format("{} Frames", frameCount);
            auto const vertices     = fmt::format("{} Vertices", drawStats_.numVertices);
            auto const triangles    = fmt::format("{} Triangles", drawStats_.numVertices / 3);
            auto const mb           = drawStats_.numVertices * sizeof(moci::Mesh::Vertex) / 1'000'000.0F;
            auto const megabyte     = fmt::format("{0:0.1f} Mbytes", mb);
            ImGui::TextUnformatted(frameCounter.c_str());
            ImGui::TextUnformatted(fpsStr.c_str());
            ImGui::TextUnformatted(minFPSStr.c_str());
            ImGui::TextUnformatted(maxFPSStr.c_str());
            ImGui::TextUnformatted(vertices.c_str());
            ImGui::TextUnformatted(triangles.c_str());
            ImGui::TextUnformatted(megabyte.c_str());
            // auto const lightStr = fmt::format("Light vertices: {}", lightMesh_.GetVertices().size());
            // ImGui::TextUnformatted(lightStr.c_str());
            auto const size = static_cast<int>(fpsHistory_.size());
            ImGui::PlotLines("FPS", fpsHistory_.data(), size, 0, "", 50.0F, 75.0F, ImVec2(0, 80));
        }
        ImGui::End();
    }

    // ImGui Demo
    if (imguiDemo_) { ImGui::ShowDemoWindow(&imguiDemo_); }
}
