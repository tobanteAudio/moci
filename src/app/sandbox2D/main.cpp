#include <moci/application/application.hpp>
#include <moci/core/network/datagram.hpp>
#include <moci/widgets/component/button.hpp>
#include <moci/widgets/component/label.hpp>
#include <moci/widgets/component/slider.hpp>
#include <moci/widgets/layer/component_layer.hpp>

#define MOCI_ENTRYPOINT
#include <moci/application/entry_point.hpp>

#include "imgui.h"

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <thread>

static std::atomic<float> level{0.5F};

class LevelMeterView : public moci::Component
{
public:
    LevelMeterView(moci::ColorRGBA32 col, std::string id)
        : Component(std::move(id))
        , _color(col)
    {}

    ~LevelMeterView() override = default;

    auto onDraw(moci::Painter& painter) -> void override
    {
        MOCI_PROFILE_FUNCTION();
        auto area            = getBounds();
        auto const ledHeight = area.getHeight() / (numLEDs);

        for (auto i = 0; i < (numLEDs * level.load()); i++) {
            auto const ledBounds = area.removeFromBottom(ledHeight).reduced(1);
            painter.drawQuad(ledBounds.toFloat(), _color);
        }
    }

private:
    static constexpr int numLEDs = 100;

    moci::ColorRGBA32 _color{};
};

auto linearToDecibel(float linear) -> float
{
    if (linear != 0.0F) {
        return 20.0F * log10f(linear);
    }
    return -144.0F;  // effectively minus infinity
}

class MultiChannel : public moci::Component
{
public:
    MultiChannel() : Component("multi-channel")
    {
        MOCI_PROFILE_FUNCTION();
        _listener.bind("", 8080);
        _listener.setMessageCallback([](auto const& buffer, auto const size) {
            moci::ignoreUnused(size);
            auto data = float{};
            std::memcpy(&data, buffer.data(), sizeof(data));
            level.store((144.0F - std::abs(linearToDecibel(data))) / 144.0F);

            // auto data = Data{};
            // std::memcpy(&data, buffer.data(), sizeof(data));
            // std::printf("Data: %u - %u, %f\n", data.type, data.id, data.value);
        });

        _listener.listen();

        for (auto i = 0; i < 5; i++) {
            auto col = moci::ColorRGBA32{0.2F * static_cast<float>(i), 0.8F, 0.2F, 1.0F};
            auto channel = std::make_unique<LevelMeterView>(col, fmt::format("{}", i));
            addChild(channel.get());
            _channels.push_back(std::move(channel));
        }

        for (auto i = 0; i < 10; i++) {
            auto col    = moci::ColorRGBA32{0.2F * static_cast<float>(i), 0.8F, 0.2F, 1.0F};
            auto slider = std::make_unique<moci::Slider>(col);
            slider->setValue(i / 10.0F);
            addChild(slider.get());
            _sliders.push_back(std::move(slider));
        }

        for (auto i = 0; i < 20; i++) {
            auto specs                   = moci::ButtonSpecs{};
            specs.callbacks.stateChanged = [](auto state) { moci::ignoreUnused(state); };

            auto button = std::make_unique<moci::Button>(fmt::format("Test: {}", i), specs);
            button->setTextColor(
                moci::ColorRGBA32{0.2F * static_cast<float>(i), 0.8F, 0.2F, 1.0F}
            );

            addChild(button.get());
            _buttons.push_back(std::move(button));
        }
    }

    ~MultiChannel() override { _listener.shutdown(); };

    auto onDraw(moci::Painter& painter) -> void override { moci::ignoreUnused(painter); }

    auto onResize() -> void override
    {
        MOCI_PROFILE_FUNCTION();
        auto area        = getBounds();
        auto const width = area.getWidth();
        auto buttonArea  = area.removeFromRight(width / 3).reduced(20);
        auto sliderArea  = area.removeFromRight(width / 3).reduced(20);
        auto const sliderHeight
            = static_cast<int>(sliderArea.getHeight() / _sliders.size());
        auto const buttonHeight
            = static_cast<int>(buttonArea.getHeight() / _buttons.size());
        auto const channelWidth = static_cast<int>(area.getWidth() / _channels.size());

        for (auto& channel : _channels) {
            channel->setBounds(area.removeFromLeft(channelWidth).reduced(5, 25));
        }
        for (auto& slider : _sliders) {
            slider->setBounds(sliderArea.removeFromTop(sliderHeight).reduced(5, 25));
        }
        for (auto& button : _buttons) {
            button->setBounds(buttonArea.removeFromTop(buttonHeight).reduced(5, 5));
        }
    }

private:
    moci::DatagramSocket _listener{};
    std::vector<std::unique_ptr<LevelMeterView>> _channels;
    std::vector<std::unique_ptr<moci::Slider>> _sliders;
    std::vector<std::unique_ptr<moci::Button>> _buttons;
};

class Sandbox : public moci::Application
{
public:
    Sandbox() : moci::Application(moci::WindowSpecs{"Sandbox 2D", 1280, 720})
    {
        MOCI_PROFILE_BEGIN_SESSION("moci-sandbox", "moci-sandbox.json");
        getWindow().setFullscreen(true);
        pushLayer(std::make_unique<moci::ComponentLayer>(std::make_unique<MultiChannel>()));
    }

    ~Sandbox() override { MOCI_PROFILE_END_SESSION(); }

private:
};

auto moci::createApplication() -> moci::Application* { return new Sandbox(); }
