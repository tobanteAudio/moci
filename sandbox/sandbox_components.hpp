#include "moci/moci.hpp"

#include "moci/render/opengles2/shader.hpp"

#include "imgui.h"

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <thread>

static std::atomic<float> Level = 0;

class LevelMeterView : public moci::Component
{
public:
    LevelMeterView(moci::Color col) : Component("color"), color_(col) {}
    ~LevelMeterView() override = default;

    auto OnDraw(moci::RenderQueue& painter) -> void override
    {
        auto area            = GetBounds();
        auto const ledHeight = area.GetHeight() / (numLEDs);

        for (auto i = 0; i < (numLEDs * Level.load()); i++)
        {
            auto const ledBounds = area.RemoveFromBottom(ledHeight).Reduced(1);
            painter.DrawQuad(ledBounds.ToFloat(), color_);
        }
    }

private:
    constexpr static int numLEDs = 100;

private:
    moci::Color color_ {};
};

class MultiChannel : public moci::Component
{
public:
    MultiChannel() : Component("multi-channel")
    {
        for (auto i = 0; i < 5; i++)
        {
            auto col     = moci::Color {0.2f * static_cast<float>(i), 0.8f, 0.2f, 1.0f};
            auto channel = std::make_unique<LevelMeterView>(col);
            AddChild(channel.get());
            channels_.push_back(std::move(channel));
        }

        for (auto i = 0; i < 10; i++)
        {
            auto col    = moci::Color {0.2f * static_cast<float>(i), 0.8f, 0.2f, 1.0f};
            auto slider = std::make_unique<moci::Slider>(col);
            slider->SetValue(i / 10.0f);
            AddChild(slider.get());
            sliders_.push_back(std::move(slider));
        }

        for (auto i = 0; i < 20; i++)
        {
            auto col   = moci::Color {0.2f * static_cast<float>(i), 0.8f, 0.2f, 1.0f};
            auto label = std::make_unique<moci::Label>(fmt::format("Test: {}", i), col);
            AddChild(label.get());
            labels_.push_back(std::move(label));
        }
    }

    ~MultiChannel() override = default;

    auto OnDraw(moci::RenderQueue& painter) -> void override {}

    auto OnResize() -> void override
    {
        auto area               = GetBounds();
        auto const width        = area.GetWidth();
        auto labelArea          = area.RemoveFromRight(width / 3).Reduced(20);
        auto sliderArea         = area.RemoveFromRight(width / 3).Reduced(20);
        auto const sliderHeight = sliderArea.GetHeight() / sliders_.size();
        auto const labelHeight  = labelArea.GetHeight() / labels_.size();
        auto const channelWidth = area.GetWidth() / channels_.size();

        for (auto& channel : channels_)
        {
            channel->SetBounds(area.RemoveFromLeft(channelWidth).Reduced(5, 25));
        }

        for (auto& slider : sliders_)
        {
            slider->SetBounds(sliderArea.RemoveFromTop(sliderHeight).Reduced(5, 25));
        }

        for (auto& label : labels_)
        {
            label->SetBounds(labelArea.RemoveFromTop(labelHeight).Reduced(5, 25));
        }
    }

private:
    std::vector<std::unique_ptr<LevelMeterView>> channels_;
    std::vector<std::unique_ptr<moci::Slider>> sliders_;
    std::vector<std::unique_ptr<moci::Label>> labels_;
};

float LinearToDecibel(float linear)
{
    if (linear != 0.0f)
    {
        return 20.0f * log10f(linear);
    }
    return -144.0f;  // effectively minus infinity
}

class SandboxComponentSystem : public moci::Layer
{
public:
    SandboxComponentSystem() : Layer("sandbox-component-system") {}
    ~SandboxComponentSystem() override = default;

    void OnAttach() override
    {
        listener_.Bind("", 8080);
        listener_.SetMessageCallback([](auto const& buffer, auto const size) {
            auto data = float {};
            std::memcpy(&data, buffer.data(), sizeof(data));

            Level.store((144.0f - std::abs(LinearToDecibel(data))) / 144.0f);

            // auto data = Data{};
            // std::memcpy(&data, buffer.data(), sizeof(data));
            // std::printf("Data: %u - %u, %f\n", data.type, data.id, data.value);
        });

        listener_.Listen();

        MOCI_INFO("Texture Size: {}", moci::RenderCommand::MaxTextureSize());
        MOCI_INFO("Texture Units: {}", moci::RenderCommand::MaxTextureUnits());
        MOCI_INFO("Vetrex Attributes: {}", moci::RenderCommand::MaxVertexAttributes());

        renderQueue_ = std::make_unique<moci::RenderQueue>();
        channel_.SetStyle(&style_);
    }

    void OnDetach() override { listener_.Shutdown(); }

    void OnUpdate(moci::Timestep ts) override
    {
        // Render
        moci::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        moci::RenderCommand::Clear();

        renderQueue_->StartFrame(width_, height_);

        channel_.SetPosition(0, 0);
        channel_.SetSize(static_cast<int>(width_), static_cast<int>(height_));
        channel_.Draw(*renderQueue_);

        renderQueue_->EndFrame();
    }

    void OnImGuiRender() override
    {
        MOCI_PROFILE_FUNCTION();
        ImGui::Begin("Settings");
        ImGui::Text("Draws: %d", renderQueue_->GetStats().drawCount);
        ImGui::Text("Textures: %d", renderQueue_->GetStats().textureCount);
        ImGui::Text("Vertics: %d", renderQueue_->GetStats().vertexCount);
        ImGui::Text("Quads: %d", renderQueue_->GetStats().quadCount);
        ImGui::Text("Circles: %d", renderQueue_->GetStats().circleCount);
        ImGui::End();
    }

    void OnEvent(moci::Event& e) override
    {
        moci::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<moci::WindowResizeEvent>(MOCI_BIND_EVENT_FN(SandboxComponentSystem::OnWindowResized));
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
    glm::vec2 compPos_ {0.0f, 0.0f};
    glm::vec2 compScale_ {100.0f, 100.0f};

    std::unique_ptr<moci::RenderQueue> renderQueue_ {};
    MultiChannel channel_;
    moci::Style style_ {};
    moci::DatagramSocket listener_ {};
};