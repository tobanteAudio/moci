#define MOCI_ENTRYPOINT
#include "moci/moci.hpp"

#include "imgui.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"

class MociModelViwerLayer : public moci::Layer
{
public:
    MociModelViwerLayer()           = default;
    ~MociModelViwerLayer() override = default;

    void OnAttach() override { }

    void OnUpdate(moci::Timestep ts) override
    {
        MOCI_PROFILE_FUNCTION();
        lastTimestep_ = ts;
        moci::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        moci::RenderCommand::Clear();

        // // Camera matrix
        // glm::mat4 const projection = glm::perspective(glm::radians(cameraFOV_), width_ / height_, 0.1f, 100.0f);
        // glm::mat4 const view       = glm::lookAt(  //
        //     cameraPos_,                      // Camera is at (x,y,z), in World Space
        //     cameraLookAt_,                   // and looks at the origin
        //     glm::vec3(0, 1, 0)               // Head is up (set to 0,-1,0 to look upside-down)
        // );
    }

    void OnEvent(moci::Event& e) override
    {
        moci::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<moci::WindowResizeEvent>(MOCI_BIND_EVENT_FN(MociModelViwerLayer::OnWindowResized));
        dispatcher.Dispatch<moci::KeyPressedEvent>(MOCI_BIND_EVENT_FN(MociModelViwerLayer::OnKeyPressed));
    }

    bool OnWindowResized(moci::WindowResizeEvent& e)
    {
        width_  = static_cast<float>(e.GetWidth());
        height_ = static_cast<float>(e.GetHeight());
        return false;
    }

    bool OnKeyPressed(moci::KeyPressedEvent& e)
    {
        moci::IgnoreUnused(e);
        return false;
    }

    void OnImGuiRender() override
    {
        auto const fps = ImGui::GetIO().Framerate;
        if (drawStats_.frameCounter++ >= 100)
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
                moci::Application::Get().GetWindow().SetFullscreen(fullscreen_);

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
            ImGui::Begin("Model Viewer", &imguiWindow_);
            if (ImGui::CollapsingHeader("Stats"))
            {
                // fpsHistory_.push_back(fps);
                auto const frameCount = fmt::format("{} Frames", drawStats_.frameCounter);
                // auto const vertices   = fmt::format("{} Vertices", drawStats_.numVertices);
                // auto const triangles  = fmt::format("{} Triangles", drawStats_.numVertices / 3);
                // auto const mb         = drawStats_.numVertices * sizeof(Vertex) / 1'000'000.0f;
                // auto const megabyte   = fmt::format("{0:0.1f} Mbytes", mb);
                ImGui::TextUnformatted(frameCount.c_str());
                // ImGui::TextUnformatted(fpsStr.c_str());
                // ImGui::TextUnformatted(minFPSStr.c_str());
                // ImGui::TextUnformatted(maxFPSStr.c_str());
                // ImGui::TextUnformatted(vertices.c_str());
                // ImGui::TextUnformatted(triangles.c_str());
                // ImGui::TextUnformatted(megabyte.c_str());
                // auto const lightStr = fmt::format("Light vertices: {}", lightMesh_.GetVertices().size());
                // ImGui::TextUnformatted(lightStr.c_str());
                // auto const size = static_cast<int>(fpsHistory_.size());
                // ImGui::PlotLines("FPS", fpsHistory_.data(), size, 0, "", 50.0f, 75.0f, ImVec2(0, 80));
            }
            ImGui::End();
        }

        // ImGui Demo
        if (imguiDemo_)
        {
            ImGui::ShowDemoWindow(&imguiDemo_);
        }
    }

public:
    float width_        = 1280.0f;
    float height_       = 1024.0f;
    float lastTimestep_ = 0.0f;
    bool fullscreen_    = false;

    moci::Ref<moci::Shader> shader_    = nullptr;
    moci::Ref<moci::VertexBuffer> vbo_ = nullptr;
    moci::Ref<moci::IndexBuffer> ibo_  = nullptr;
    moci::Ref<moci::VertexArray> vao_  = nullptr;

    std::size_t numVertices_ {};

    // imgui
    struct DrawStats
    {
        std::uint32_t frameCounter {};
        std::uint32_t numVertices {};
        float minFPS = 9999.0f;
        float maxFPS = 0.0f;
    };

    DrawStats drawStats_ {};

    moci::Vector<float> fpsHistory_ = {};
    bool imguiWindow_               = true;
    bool imguiDemo_                 = false;
};

class MociModelViwerApp : public moci::Application
{
public:
    MociModelViwerApp()
    {
        MOCI_PROFILE_BEGIN_SESSION("moci-model-viewer", "moci-model-viewer.json");
        PushLayer(moci::MakeScope<MociModelViwerLayer>());
    }

    ~MociModelViwerApp() override { MOCI_PROFILE_END_SESSION(); }

private:
};

auto moci::CreateApplication() -> moci::Application* { return new MociModelViwerApp(); }