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

void DemoLayer::OnAttach()
{
    MOCI_PROFILE_FUNCTION();
    {
        MOCI_PROFILE_SCOPE("OnAttach::GPUQuery");
        MOCI_INFO("Max vertex attributes: {}", moci::RenderCommand::MaxVertexAttributes());
        MOCI_INFO("Max texture size: {}", moci::RenderCommand::MaxTextureSize());
        MOCI_INFO("Max texture units: {}", moci::RenderCommand::MaxTextureUnits());
        MOCI_INFO("Max uniform vectors: {}", moci::RenderCommand::MaxUniformVectors());
    }

    auto const numVertices = (mesh_.GetVertices().size() * 11)  //
                             + floor_.GetVertices().size();     //
                                                                //  + lightMesh_.GetVertices().size();

    vertices_.reserve(numVertices);
    {
        MOCI_PROFILE_SCOPE("OnAttach::Shader");
#if defined(MOCI_API_OPENGL_LEGACY)
        shader_ = moci::Shader::Create("sandbox3D/assets/shader/es2_general.glsl");
#else
        shader_        = moci::Shader::Create("sandbox3D/assets/shader/gl4_general.glsl");
#endif
        shader_->Bind();
    }

    {
        MOCI_PROFILE_SCOPE("Translate");
        for (auto const& vertex : mesh_.GetVertices())
        {
            auto const model       = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f));
            auto const scaleMatrix = glm::scale(glm::mat4(1.0f), {modelScale_, modelScale_, modelScale_});
            auto const position    = model * scaleMatrix * glm::vec4(vertex.position, 1.0f);
            vertices_.push_back({glm::vec3(position), vertex.normal, vertex.color, vertex.texCoord});
        }

        {
            MOCI_PROFILE_SCOPE("Translate Small Teapots");
            for (auto x = 1; x <= 10; x++)
            {
                auto const modelScale = modelScale_ / 2.0f;
                auto const scale      = glm::scale(glm::mat4(1.0f), {modelScale, modelScale, modelScale});
                auto const model      = glm::translate(glm::mat4(1.0f), glm::vec3((3.0f * x), 5.0f, 1.0f));
                for (auto const& vertex : mesh_.GetVertices())
                {
                    auto const position = model * scale * glm::vec4(vertex.position, 1.0f);
                    vertices_.push_back({glm::vec3(position), vertex.normal, vertex.color, vertex.texCoord});
                }
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

    // Mesh buffer
    {
        MOCI_PROFILE_SCOPE("OnAttach::MeshBuffer");
        moci::BufferLayout layout = {
            {moci::ShaderDataType::Float3, "a_Position"},   //
            {moci::ShaderDataType::Float3, "a_Normal"},     //
            {moci::ShaderDataType::Float4, "a_Color"},      //
            {moci::ShaderDataType::Float2, "a_TexCoords"},  //
        };
        auto* data      = reinterpret_cast<float*>(vertices_.data());
        auto const size = static_cast<std::uint32_t>(vertices_.size() * sizeof(moci::Mesh::Vertex));
        vbo_.reset(moci::VertexBuffer::Create(data, size, false));
        vbo_->SetLayout(layout);
        vbo_->Unbind();
        ibo_.reset(moci::IndexBuffer::Create(nullptr, 1, true));
        ibo_->Unbind();
        vao_ = moci::VertexArray::Create();
        vao_->AddVertexBuffer(vbo_);
        vao_->SetIndexBuffer(ibo_);
        vao_->Unbind();
    }

    // light
    light = moci::MakeScope<moci::Light>();

    {
        MOCI_PROFILE_SCOPE("OnAttach::Textures");
        textureSolid_  = moci::Texture2D::Create("sandbox3D/assets/textures/white_10x10.png");
        textureColors_ = moci::Texture2D::Create("sandbox3D/assets/textures/4color.png");
#if defined(MOCI_API_OPENGL_ES)
        textureColors_ = moci::Texture2D::Create("sandbox3D/assets/textures/cerberus_A_1024x1024.png");
#else
        textureColors_ = moci::Texture2D::Create("sandbox3D/assets/textures/cerberus_A_4096x4096.png");
#endif
    }

    fpsHistory_.reserve(10'000);
}

void DemoLayer::OnUpdate(moci::Timestep ts)
{
    MOCI_PROFILE_FUNCTION();
    drawStats_.frameCounter += 1;
    lastTimestep_          = ts.GetMilliseconds();
    drawStats_.numVertices = 0;
    moci::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    moci::RenderCommand::Clear();

    // Camera matrix
    glm::mat4 const projection = glm::perspective(glm::radians(cameraFOV_), width_ / height_, 0.1f, 100.0f);
    glm::mat4 const view       = glm::lookAt(  //
        cameraPos_,                      // Camera is at (x,y,z), in World Space
        cameraLookAt_,                   // and looks at the origin
        glm::vec3(0, 1, 0)               // Head is up (set to 0,-1,0 to look upside-down)
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
        moci::RenderCommand::DrawArrays(moci::RendererAPI::DrawMode::Triangles, 0,
                                        static_cast<std::uint32_t>(vertices_.size()));
        drawStats_.numVertices += static_cast<std::uint32_t>(vertices_.size());
        textureColors_->Unbind();
    }

    {
        MOCI_PROFILE_SCOPE("OnUpdate::Light");
        light->Render(view, projection);
    }
}

void DemoLayer::OnEvent(moci::Event& e)
{
    moci::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<moci::WindowResizeEvent>(MOCI_BIND_EVENT_FN(DemoLayer::OnWindowResized));
    dispatcher.Dispatch<moci::KeyPressedEvent>(MOCI_BIND_EVENT_FN(DemoLayer::OnKeyPressed));
}

bool DemoLayer::OnWindowResized(moci::WindowResizeEvent& e)
{
    width_  = static_cast<float>(e.GetWidth());
    height_ = static_cast<float>(e.GetHeight());
    return false;
}

bool DemoLayer::OnKeyPressed(moci::KeyPressedEvent& e)
{
    auto const moveAmount = 0.25f;
    if (e.GetKeyCode() == MOCI_KEY_LEFT)
    {
        cameraPos_.x = cameraPos_.x - moveAmount;
    }
    if (e.GetKeyCode() == MOCI_KEY_RIGHT)
    {
        cameraPos_.x = cameraPos_.x + moveAmount;
    }
    if (e.GetKeyCode() == MOCI_KEY_DOWN)
    {
        cameraPos_.y = cameraPos_.y - moveAmount;
    }
    if (e.GetKeyCode() == MOCI_KEY_UP)
    {
        cameraPos_.y = cameraPos_.y + moveAmount;
    }
    return true;
}

void DemoLayer::OnImGuiRender()
{
    auto const fps = ImGui::GetIO().Framerate;
    if (drawStats_.frameCounter >= 100)
    {
        if (fps < drawStats_.minFPS)
        {
            drawStats_.minFPS = fps;
        }
        if (fps > drawStats_.maxFPS)
        {
            drawStats_.maxFPS = fps;
        }
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

            ImGui::EndMenu();
        }

        ImGui::TextUnformatted(fpsStr.c_str());
        ImGui::TextUnformatted(minFPSStr.c_str());
        ImGui::TextUnformatted(maxFPSStr.c_str());
        ImGui::TextUnformatted(ts.c_str());
        ImGui::EndMenuBar();
    }

    if (imguiWindow_)
    {
        ImGui::Begin("Sandbox 3D", &imguiWindow_);
        if (ImGui::Button("Fullscreen"))
        {
            moci::Application::Get().GetWindow().SetFullscreen(true);
        }

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

        if (ImGui::CollapsingHeader("Model"))
        {
            ImGui::SliderFloat("Scale", &modelScale_, 0.01f, 20.0f);
        }

        if (ImGui::CollapsingHeader("Stats"))
        {
            fpsHistory_.push_back(fps);
            auto const frameCount = fmt::format("{} Frames", drawStats_.frameCounter);
            auto const vertices   = fmt::format("{} Vertices", drawStats_.numVertices);
            auto const triangles  = fmt::format("{} Triangles", drawStats_.numVertices / 3);
            auto const mb         = drawStats_.numVertices * sizeof(moci::Mesh::Vertex) / 1'000'000.0f;
            auto const megabyte   = fmt::format("{0:0.1f} Mbytes", mb);
            ImGui::TextUnformatted(frameCount.c_str());
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
    if (imguiDemo_)
    {
        ImGui::ShowDemoWindow(&imguiDemo_);
    }
}
