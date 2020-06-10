#define MOCI_ENTRYPOINT
#include "moci/moci.hpp"

#include "imgui.h"

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <thread>

static std::atomic<float> Level = 0;

class LevelMeterView : public moci::Component
{
public:
    LevelMeterView(moci::Color col) : Component("color"), color_(col) { }
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

    moci::Color color_ {};
};

float LinearToDecibel(float linear)
{
    if (linear != 0.0f)
    {
        return 20.0f * log10f(linear);
    }
    return -144.0f;  // effectively minus infinity
}

class MultiChannel : public moci::Component
{
public:
    MultiChannel() : Component("multi-channel")
    {
        listener_.Bind("", 8080);
        listener_.SetMessageCallback([](auto const& buffer, auto const size) {
            moci::IgnoreUnused(size);
            auto data = float {};
            std::memcpy(&data, buffer.data(), sizeof(data));

            Level.store((144.0f - std::abs(LinearToDecibel(data))) / 144.0f);

            // auto data = Data{};
            // std::memcpy(&data, buffer.data(), sizeof(data));
            // std::printf("Data: %u - %u, %f\n", data.type, data.id, data.value);
        });

        listener_.Listen();

        for (auto i = 0; i < 5; i++)
        {
            auto col     = moci::Color {0.2f * static_cast<float>(i), 0.8f, 0.2f, 1.0f};
            auto channel = moci::MakeScope<LevelMeterView>(col);
            AddChild(channel.get());
            channels_.push_back(std::move(channel));
        }

        for (auto i = 0; i < 10; i++)
        {
            auto col    = moci::Color {0.2f * static_cast<float>(i), 0.8f, 0.2f, 1.0f};
            auto slider = moci::MakeScope<moci::Slider>(col);
            slider->SetValue(i / 10.0f);
            AddChild(slider.get());
            sliders_.push_back(std::move(slider));
        }

        for (auto i = 0; i < 20; i++)
        {
            auto col   = moci::Color {0.2f * static_cast<float>(i), 0.8f, 0.2f, 1.0f};
            auto label = moci::MakeScope<moci::Label>(fmt::format("Test: {}", i), col);
            AddChild(label.get());
            labels_.push_back(std::move(label));
        }
    }

    ~MultiChannel() override { listener_.Shutdown(); };

    auto OnDraw(moci::RenderQueue& painter) -> void override { moci::IgnoreUnused(painter); }

    auto OnResize() -> void override
    {
        auto area               = GetBounds();
        auto const width        = area.GetWidth();
        auto labelArea          = area.RemoveFromRight(width / 3).Reduced(20);
        auto sliderArea         = area.RemoveFromRight(width / 3).Reduced(20);
        auto const sliderHeight = static_cast<int>(sliderArea.GetHeight() / sliders_.size());
        auto const labelHeight  = static_cast<int>(labelArea.GetHeight() / labels_.size());
        auto const channelWidth = static_cast<int>(area.GetWidth() / channels_.size());

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
    moci::DatagramSocket listener_ {};
    moci::Vector<moci::Scope<LevelMeterView>> channels_;
    moci::Vector<moci::Scope<moci::Slider>> sliders_;
    moci::Vector<moci::Scope<moci::Label>> labels_;
};

class Sandbox : public moci::Application
{
public:
    Sandbox() : moci::Application(moci::WindowSpecs {"Sandbox 2D", 1280, 720})
    {
        MOCI_PROFILE_BEGIN_SESSION("moci-sandbox", "moci-sandbox.json");
        GetWindow().SetFullscreen(true);
        PushLayer(moci::MakeScope<moci::ComponentLayer>(moci::MakeScope<MultiChannel>()));
    }

    ~Sandbox() override { MOCI_PROFILE_END_SESSION(); }

private:
};

auto moci::CreateApplication() -> moci::Application* { return new Sandbox(); }