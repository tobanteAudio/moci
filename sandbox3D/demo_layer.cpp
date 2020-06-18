#include "demo_layer.hpp"

#include "imgui.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"

#include "glm/gtx/quaternion.hpp"

namespace
{
#if defined(MOCI_API_OPENGL_LEGACY)
constexpr auto shaderPrefix = "es2";
#else
constexpr auto shaderPrefix = "gl4";
#endif

auto skyboxVertices = std::array {
    // positions
    -1.0f, 1.0f,  -1.0f,  //
    -1.0f, -1.0f, -1.0f,  //
    1.0f,  -1.0f, -1.0f,  //
    1.0f,  -1.0f, -1.0f,  //
    1.0f,  1.0f,  -1.0f,  //
    -1.0f, 1.0f,  -1.0f,  //

    -1.0f, -1.0f, 1.0f,   //
    -1.0f, -1.0f, -1.0f,  //
    -1.0f, 1.0f,  -1.0f,  //
    -1.0f, 1.0f,  -1.0f,  //
    -1.0f, 1.0f,  1.0f,   //
    -1.0f, -1.0f, 1.0f,   //

    1.0f,  -1.0f, -1.0f,  //
    1.0f,  -1.0f, 1.0f,   //
    1.0f,  1.0f,  1.0f,   //
    1.0f,  1.0f,  1.0f,   //
    1.0f,  1.0f,  -1.0f,  //
    1.0f,  -1.0f, -1.0f,  //

    -1.0f, -1.0f, 1.0f,  //
    -1.0f, 1.0f,  1.0f,  //
    1.0f,  1.0f,  1.0f,  //
    1.0f,  1.0f,  1.0f,  //
    1.0f,  -1.0f, 1.0f,  //
    -1.0f, -1.0f, 1.0f,  //

    -1.0f, 1.0f,  -1.0f,  //
    1.0f,  1.0f,  -1.0f,  //
    1.0f,  1.0f,  1.0f,   //
    1.0f,  1.0f,  1.0f,   //
    -1.0f, 1.0f,  1.0f,   //
    -1.0f, 1.0f,  -1.0f,  //

    -1.0f, -1.0f, -1.0f,  //
    -1.0f, -1.0f, 1.0f,   //
    1.0f,  -1.0f, -1.0f,  //
    1.0f,  -1.0f, -1.0f,  //
    -1.0f, -1.0f, 1.0f,   //
    1.0f,  -1.0f, 1.0f    //
};

glm::vec2 ImGuiToGlmVec(ImVec2 const& input) { return {input.x, input.y}; }

}  // namespace

void DemoLayer::OnAttach()
{
    MOCI_PROFILE_FUNCTION();

    auto const numVertices = (mesh_.GetVertices().size() * 5) + floor_.GetVertices().size();
    vertices_.reserve(numVertices);

    auto const path = fmt::format("sandbox3D/assets/shader/{}_general.glsl", shaderPrefix);
    shader_         = moci::RenderFactory::MakeShader(path);
    shader_->Bind();

    {
        MOCI_PROFILE_SCOPE("Translate");
        for (auto const& vertex : mesh_.GetVertices())
        {
            auto const model       = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f));
            auto const scaleMatrix = glm::scale(glm::mat4(1.0f), {modelScale_, modelScale_, modelScale_});
            auto const position    = model * scaleMatrix * glm::vec4(vertex.position, 1.0f);
            vertices_.push_back({glm::vec3(position), vertex.normal, vertex.color, vertex.texCoord});
        }

        for (auto x = 1; x <= 5; x++)
        {
            auto const modelScale = modelScale_ / 2.0f;
            auto const rotate     = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), {1.0f, 0.0f, 0.0f});
            auto const scale      = glm::scale(glm::mat4(1.0f), {modelScale, modelScale, modelScale});
            auto const model      = glm::translate(glm::mat4(1.0f), glm::vec3((3.0f * x), 5.0f, 1.0f));
            for (auto const& vertex : mesh_.GetVertices())
            {
                auto const position = model * scale * rotate * glm::vec4(vertex.position, 1.0f);
                vertices_.push_back({glm::vec3(position), vertex.normal, vertex.color, vertex.texCoord});
            }
        }

        for (auto const& vertex : floor_.GetVertices())
        {
            auto const model       = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f));
            auto const scaleMatrix = glm::scale(glm::mat4(1.0f), {5.0f, 5.0f, 5.0f});
            auto const position    = model * scaleMatrix * glm::vec4(vertex.position, 1.0f);
            vertices_.push_back({glm::vec3(position), vertex.normal, {1.0f, 1.0f, 0.5f, 1.0f}, vertex.texCoord});
        }

        numVertices_ = vertices_.size();
    }

    auto const skyBoxShaderPath = fmt::format("sandbox3D/assets/shader/{}_skybox.glsl", shaderPrefix);
    skyboxShader_               = moci::RenderFactory::MakeShader(skyBoxShaderPath);
    skyboxShader_->Bind();
    skyboxShader_->SetInt("u_Skybox", 0);
    moci::BufferLayout skyBoxLayout = {{moci::ShaderDataType::Float3, "a_Position"}};
    auto* skyBoxData                = reinterpret_cast<float*>(skyboxVertices.data());
    auto const skyBoxSize           = static_cast<std::uint32_t>(skyboxVertices.size() * sizeof(float));
    skyboxVbo_.reset(moci::RenderFactory::MakeVertexBuffer(skyBoxData, skyBoxSize, false));
    skyboxVbo_->SetLayout(skyBoxLayout);
    skyboxVbo_->Unbind();
    skyboxIbo_.reset(moci::RenderFactory::MakeIndexBuffer({{}, 1, true}));
    skyboxIbo_->Unbind();
    skyboxVao_ = moci::RenderFactory::MakeVertexArray();
    skyboxVao_->AddVertexBuffer(skyboxVbo_);
    skyboxVao_->SetIndexBuffer(skyboxIbo_);
    skyboxVao_->Unbind();
    moci::Vector<std::string> faces {
        std::string("sandbox3D/assets/textures/skybox/right.jpg"),   //
        std::string("sandbox3D/assets/textures/skybox/left.jpg"),    //
        std::string("sandbox3D/assets/textures/skybox/top.jpg"),     //
        std::string("sandbox3D/assets/textures/skybox/bottom.jpg"),  //
        std::string("sandbox3D/assets/textures/skybox/front.jpg"),   //
        std::string("sandbox3D/assets/textures/skybox/back.jpg")     //
    };

    skyBoxTexture_ = moci::RenderFactory::MakeTextureCube(faces);

    moci::BufferLayout layout = {
        {moci::ShaderDataType::Float3, "a_Position"},   //
        {moci::ShaderDataType::Float3, "a_Normal"},     //
        {moci::ShaderDataType::Float4, "a_Color"},      //
        {moci::ShaderDataType::Float2, "a_TexCoords"},  //
    };
    auto* data      = reinterpret_cast<float*>(vertices_.data());
    auto const size = static_cast<std::uint32_t>(vertices_.size() * sizeof(moci::Mesh::Vertex));
    vbo_.reset(moci::RenderFactory::MakeVertexBuffer(data, size, false));
    vbo_->SetLayout(layout);
    vbo_->Unbind();
    ibo_.reset(moci::RenderFactory::MakeIndexBuffer({{}, 1, true}));
    ibo_->Unbind();
    vao_ = moci::RenderFactory::MakeVertexArray();
    vao_->AddVertexBuffer(vbo_);
    vao_->SetIndexBuffer(ibo_);
    vao_->Unbind();

    // light
    light = moci::MakeScope<moci::Light>();

    textureSolid_  = moci::RenderFactory::MakeTexture2D("sandbox3D/assets/textures/white_10x10.png");
    textureColors_ = moci::RenderFactory::MakeTexture2D("sandbox3D/assets/textures/4color.png");
    textureColors_ = moci::RenderFactory::MakeTexture2D("sandbox3D/assets/textures/cerberus_A_4096x4096.png");

    auto fbSpec   = moci::FramebufferSpecs {};
    fbSpec.width  = 1920;
    fbSpec.height = 1080;
    framebuffer_  = moci::RenderFactory::MakeFramebuffer(fbSpec);

    fpsHistory_.reserve(10'000);
}

void DemoLayer::OnUpdate(moci::Timestep ts)
{
    MOCI_PROFILE_FUNCTION();
    lastTimestep_          = ts.GetMilliseconds();
    drawStats_.numVertices = 0;
    moci::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    moci::RenderCommand::Clear();

    framebuffer_->Bind();
    moci::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
    moci::RenderCommand::Clear();

    // Camera matrix
    auto const aspectRatio = viewportSize_.x / viewportSize_.y;
    auto const projection  = glm::perspective(glm::radians(cameraFOV_), aspectRatio, 0.1f, 100.0f);
    auto const view        = glm::lookAt(  //
        cameraPos_,                 // Camera is at (x,y,z), in World Space
        cameraPos_ + cameraFront_,  // and looks at
        cameraUp_                   // Head is up (set to 0,-1,0 to look upside-down)
    );

    {
        MOCI_PROFILE_SCOPE("OnUpdate::Mesh");
        shader_->Bind();
        shader_->SetMat4("u_View", view);
        shader_->SetMat4("u_Projection", projection);
        shader_->SetFloat("u_Ambient", ambientLight_);
        shader_->SetFloat3("u_LightPos", light->position);
        shader_->SetFloat3("u_LightColor", glm::vec3(light->color));
        shader_->SetFloat3("u_ViewPos", cameraPos_);

        vao_->Bind();
        textureColors_->Bind(0);
        moci::RenderCommand::DrawArrays(moci::RenderDrawMode::Triangles, 0,
                                        static_cast<std::uint32_t>(vertices_.size()));
        drawStats_.numVertices += static_cast<std::uint32_t>(vertices_.size());
        textureColors_->Unbind();
    }

    {
        MOCI_PROFILE_SCOPE("OnUpdate::Light");
        light->Render(view, projection);
    }

    // skybox
    // change depth function so depth test passes when values are equal to depth buffer's content
    GLCall(glDepthFunc(GL_LEQUAL));
    skyboxShader_->Bind();
    auto const skyboxView = glm::mat4(glm::mat3(view));
    skyboxShader_->SetMat4("u_View", skyboxView);
    skyboxShader_->SetMat4("u_Projection", projection);
    skyboxShader_->SetInt("u_Skybox", 0);
    // skybox cube
    skyboxVao_->Bind();
    skyBoxTexture_->Bind(0);
    moci::RenderCommand::DrawArrays(moci::RenderDrawMode::Triangles, 0, 36);
    skyboxVao_->Unbind();
    GLCall(glDepthFunc(GL_LESS));  // set depth function back to default

    framebuffer_->Unbind();
}

void DemoLayer::OnEvent(moci::Event& e)
{
    moci::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<moci::WindowResizeEvent>(MOCI_EVENT_METHOD(DemoLayer::OnWindowResized));
    dispatcher.Dispatch<moci::KeyPressedEvent>(MOCI_EVENT_METHOD(DemoLayer::OnKeyPressed));
    dispatcher.Dispatch<moci::MouseScrolledEvent>(MOCI_EVENT_METHOD(DemoLayer::OnMouseScrolled));
    dispatcher.Dispatch<moci::MouseMovedEvent>(MOCI_EVENT_METHOD(DemoLayer::OnMouseMoved));
    dispatcher.Dispatch<moci::MouseButtonPressedEvent>(MOCI_EVENT_METHOD(DemoLayer::OnMousePressed));
    dispatcher.Dispatch<moci::MouseButtonReleasedEvent>(MOCI_EVENT_METHOD(DemoLayer::OnMouseReleased));
}

bool DemoLayer::OnWindowResized(moci::WindowResizeEvent& e)
{
    width_  = static_cast<float>(e.GetWidth());
    height_ = static_cast<float>(e.GetHeight());
    return false;
}

bool DemoLayer::OnKeyPressed(moci::KeyPressedEvent& e)
{
    auto const cameraSpeed = 0.15f * lastTimestep_;
    if (e.GetKeyCode() == moci::Key::Up) { cameraPos_ += cameraSpeed * cameraFront_; }
    if (e.GetKeyCode() == moci::Key::Down) { cameraPos_ -= cameraSpeed * cameraFront_; }
    if (e.GetKeyCode() == moci::Key::Left)
    { cameraPos_ -= glm::normalize(glm::cross(cameraFront_, cameraUp_)) * cameraSpeed; }
    if (e.GetKeyCode() == moci::Key::Right)
    { cameraPos_ += glm::normalize(glm::cross(cameraFront_, cameraUp_)) * cameraSpeed; }

    return true;
}

bool DemoLayer::OnMouseScrolled(moci::MouseScrolledEvent& e)
{
    cameraFOV_ -= e.GetYOffset();
    constexpr auto minFOV = 1.0f;
    constexpr auto maxFOV = 75.0f;
    if (cameraFOV_ < minFOV) { cameraFOV_ = minFOV; }
    if (cameraFOV_ > maxFOV) { cameraFOV_ = maxFOV; }
    return true;
}

bool DemoLayer::OnMousePressed(moci::MouseButtonPressedEvent& e)
{
    if (e.GetMouseButton() == moci::MouseCode::ButtonMiddle)
    {
        isMouseDragging_ = true;
        return true;
    }

    return false;
}

bool DemoLayer::OnMouseReleased(moci::MouseButtonReleasedEvent& e)
{
    if (e.GetMouseButton() == moci::MouseCode::ButtonMiddle)
    {
        isMouseDragging_ = false;
        firstMouse_      = true;
        return true;
    }

    return false;
}

bool DemoLayer::OnMouseMoved(moci::MouseMovedEvent& e)
{
    if (isMouseDragging_)
    {
        if (firstMouse_)
        {
            cameraLastX_ = e.GetX();
            cameraLastY_ = e.GetY();
            firstMouse_  = false;
        }

        auto xOffset = e.GetX() - cameraLastX_;
        auto yOffset = cameraLastY_ - e.GetY();
        cameraLastX_ = e.GetX();
        cameraLastY_ = e.GetY();

        auto const sensitifity = 0.1f;
        xOffset *= sensitifity;
        yOffset *= sensitifity;

        cameraYaw_ += xOffset;
        cameraPitch_ += yOffset;

        if (cameraPitch_ > 89.0f) { cameraPitch_ = 89.0f; }

        if (cameraPitch_ < -89.0f) { cameraPitch_ = -89.0f; }

        auto front   = glm::vec3 {};
        front.x      = glm::cos(glm::radians(cameraYaw_)) * glm::cos(glm::radians(cameraPitch_));
        front.y      = glm::sin(glm::radians(cameraPitch_));
        front.z      = glm::sin(glm::radians(cameraYaw_)) * glm::cos(glm::radians(cameraPitch_));
        cameraFront_ = glm::normalize(front);
    }

    return true;
}

void DemoLayer::OnImGuiRender()
{
    auto const fps        = ImGui::GetIO().Framerate;
    auto const frameCount = moci::Application::Get().GetWindow().GetFrameCount();
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
            moci::Application::Get().GetWindow().SetFullscreen(fullscreen_);

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
    auto const viewportRegion = ImGuiToGlmVec(ImGui::GetContentRegionAvail());
    if (viewportSize_ != viewportRegion)
    {
        viewportSize_        = viewportRegion;
        auto const newWidth  = static_cast<std::uint32_t>(viewportSize_.x);
        auto const newHeight = static_cast<std::uint32_t>(viewportSize_.y);
        framebuffer_->Resize(newWidth, newHeight);
    }

    auto const textureID = reinterpret_cast<void*>(framebuffer_->GetColorAttachmentRendererID());
    ImGui::Image(textureID, {viewportRegion.x, viewportRegion.y}, ImVec2(0, 1), ImVec2(1, 0));
    ImGui::End();
    ImGui::PopStyleVar();

    if (imguiWindow_)
    {
        ImGui::Begin("Sandbox 3D", &imguiWindow_);
        if (ImGui::CollapsingHeader("Camera"))
        {
            ImGui::SliderFloat3("Position", glm::value_ptr(cameraPos_), -100.0f, 100.0f);
            ImGui::SliderFloat3("Look At", glm::value_ptr(cameraLookAt_), -10.0f, 10.0f);
            ImGui::SliderFloat("FOV", &cameraFOV_, 5.0f, 85.0f);
        }

        if (ImGui::CollapsingHeader("Light"))
        {

            ImGui::SliderFloat("Ambient", &ambientLight_, 0.0f, 0.4f);
            ImGui::SliderFloat3("Light Position", glm::value_ptr(light->position), -20.0f, 20.0f);
            ImGui::ColorEdit4("Light Color", glm::value_ptr(light->color), 0);
            ImGui::SliderFloat("Light Scale", &light->scale, 0.1f, 1.0f);
        }

        if (ImGui::CollapsingHeader("Model")) { ImGui::SliderFloat("Scale", &modelScale_, 0.01f, 20.0f); }

        if (ImGui::CollapsingHeader("Stats"))
        {
            fpsHistory_.push_back(fps);
            auto const frameCounter = fmt::format("{} Frames", frameCount);
            auto const vertices     = fmt::format("{} Vertices", drawStats_.numVertices);
            auto const triangles    = fmt::format("{} Triangles", drawStats_.numVertices / 3);
            auto const mb           = drawStats_.numVertices * sizeof(moci::Mesh::Vertex) / 1'000'000.0f;
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
            ImGui::PlotLines("FPS", fpsHistory_.data(), size, 0, "", 50.0f, 75.0f, ImVec2(0, 80));
        }
        ImGui::End();
    }

    // ImGui Demo
    if (imguiDemo_) { ImGui::ShowDemoWindow(&imguiDemo_); }
}
