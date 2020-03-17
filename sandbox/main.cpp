#define MOCI_ENTRYPOINT
#include "moci/moci.hpp"

#include "moci/render/opengles2/shader.hpp"

#include "imgui.h"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "sandbox_components.hpp"

class MociSandboxLayer : public moci::Layer
{
public:
    MociSandboxLayer() : Layer("moci-sandox") {}
    ~MociSandboxLayer() override = default;

    void OnAttach() override
    {
        MOCI_INFO("Texture Size: {}", moci::RenderCommand::MaxTextureSize());
        MOCI_INFO("Texture Units: {}", moci::RenderCommand::MaxTextureUnits());
        MOCI_INFO("Vetrex Attributes: {}", moci::RenderCommand::MaxVertexAttributes());

        renderQueue_ = std::make_unique<moci::RenderQueue>();

        textureLogoCircle_ = moci::Texture2D::Create("moci/moci/assets/512x512.png");
        textureOther_      = moci::Texture2D::Create("moci/moci/assets/white_10x10.png");
        textureOther2_     = moci::Texture2D::Create("moci/moci/assets/white_10x10.png");
        textureOther3_     = moci::Texture2D::Create("moci/moci/assets/white_10x10.png");
        textureOther4_     = moci::Texture2D::Create("moci/moci/assets/white_10x10.png");
        textureOther5_     = moci::Texture2D::Create("moci/moci/assets/white_10x10.png");
        textureOther6_     = moci::Texture2D::Create("moci/moci/assets/white_10x10.png");
        textureOther7_     = moci::Texture2D::Create("moci/moci/assets/white_10x10.png");
    }

    void OnDetach() override {}

    void OnUpdate(moci::Timestep ts) override
    {
        MOCI_PROFILE_FUNCTION();

        // Render
        moci::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        moci::RenderCommand::Clear();

        renderQueue_->StartFrame(width_, height_);

        auto const white = moci::Color {1.0f, 1.0f, 1.0f, 1.0f};
        for (auto i = 0; i < 25; i++)
        {
            renderQueue_->DrawCircle((50.0f * i) + 30.0f, 50.0f, radius_, numSides_, {1.0f, 0.0f, 0.0f, 1.0f});
            renderQueue_->DrawQuad({(50.0f * i) + 10.0f, 100.0f, 40.0f, 40.0f}, white, textureLogoCircle_);
        }
        for (auto i = 0; i < 25; i++)
        {
            renderQueue_->DrawQuad({(50.0f * i) + 10.0f, 175.0f, 40.0f, 40.0f}, white, textureOther_);
        }
        for (auto i = 0; i < 25; i++)
        {
            renderQueue_->DrawQuad({(50.0f * i) + 10.0f, 250.0f, 40.0f, 40.0f}, white, textureOther2_);
        }
        for (auto i = 0; i < 25; i++)
        {
            renderQueue_->DrawQuad({(50.0f * i) + 10.0f, 325.0f, 40.0f, 40.0f}, white, textureOther3_);
        }
        for (auto i = 0; i < 25; i++)
        {
            renderQueue_->DrawQuad({(50.0f * i) + 10.0f, 400.0f, 40.0f, 40.0f}, white, textureOther4_);
        }
        for (auto i = 0; i < 25; i++)
        {
            renderQueue_->DrawQuad({(50.0f * i) + 10.0f, 475.0f, 40.0f, 40.0f}, white, textureOther5_);
        }
        for (auto i = 0; i < 25; i++)
        {
            renderQueue_->DrawQuad({(50.0f * i) + 10.0f, 550.0f, 40.0f, 40.0f}, white, textureOther6_);
        }
        for (auto i = 0; i < 25; i++)
        {
            renderQueue_->DrawQuad({(50.0f * i) + 10.0f, 625.0f, 40.0f, 40.0f}, white, textureOther7_);
        }

        auto const fps = fmt::format("{}", static_cast<int>(ts.GetMilliseconds()));
        renderQueue_->DrawText(fps, {500.0f, 750.0f}, 1.0f, moci::Color(0.7, 0.9f, 0.5f));
        renderQueue_->DrawText("Some really really long text", {500.0f, 825.0f}, 0.9f, moci::Color(0.7, 0.9f, 0.5f));
        renderQueue_->EndFrame();
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::Text("Draws: %d", renderQueue_->GetStats().drawCount);
        ImGui::Text("Textures: %d", renderQueue_->GetStats().textureCount);
        ImGui::Text("Vertics: %d", renderQueue_->GetStats().vertexCount);
        ImGui::Text("Quads: %d", renderQueue_->GetStats().quadCount);
        ImGui::Text("Circles: %d", renderQueue_->GetStats().circleCount);
        ImGui::SliderInt("Sides: ", &numSides_, 3, 120);
        ImGui::SliderFloat("Radius: ", &radius_, 5.0f, 35.0f);
        ImGui::End();
    }

    void OnEvent(moci::Event& e) override
    {
        moci::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<moci::WindowResizeEvent>(MOCI_BIND_EVENT_FN(MociSandboxLayer::OnWindowResized));
    }

    auto OnWindowResized(moci::WindowResizeEvent& e) -> bool
    {
        width_  = static_cast<float>(e.GetWidth());
        height_ = static_cast<float>(e.GetHeight());

        return false;
    }

private:
    float width_  = 1280.0f;
    float height_ = 1024.0f;
    int numSides_ = 5;
    float radius_ = 20.0f;

    std::unique_ptr<moci::RenderQueue> renderQueue_;

    std::shared_ptr<moci::Texture2D> textureLogoCircle_;
    std::shared_ptr<moci::Texture2D> textureOther_;
    std::shared_ptr<moci::Texture2D> textureOther2_;
    std::shared_ptr<moci::Texture2D> textureOther3_;
    std::shared_ptr<moci::Texture2D> textureOther4_;
    std::shared_ptr<moci::Texture2D> textureOther5_;
    std::shared_ptr<moci::Texture2D> textureOther6_;
    std::shared_ptr<moci::Texture2D> textureOther7_;
};

class Sandbox : public moci::Application
{
public:
    Sandbox()
    {
        MOCI_PROFILE_BEGIN_SESSION("moci-sandbox", "moci-sandbox.json");
        // PushLayer(new MociSandboxLayer());
        PushLayer(new SandboxComponentSystem());
    }

    ~Sandbox() override { MOCI_PROFILE_END_SESSION(); }
};

auto moci::CreateApplication() -> moci::Application* { return new Sandbox(); }