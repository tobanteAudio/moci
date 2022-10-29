/**
 * @file slider_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>
#include <catch2/catch_approx.hpp>

#include "moci/widgets/component/slider.hpp"
#include "moci/widgets/style/style.hpp"

TEST_CASE("widget: SliderConstruct", "[ui]")
{
    moci::Slider const slider {moci::Colors::black};
    REQUIRE(slider.getValue() == 0.0F);
}

TEST_CASE("widget: SliderSetValue", "[ui]")
{
    moci::Slider slider {moci::Colors::black};
    REQUIRE(slider.getValue() == 0.0F);
    slider.setValue(143.0F);
    REQUIRE(slider.getValue() == 143.0F);
}

TEST_CASE("widget: SliderSetColor", "[ui]")
{
    moci::Slider slider {{}};

    auto c1 = slider.getColor();
    REQUIRE(c1.getRed() == 0.0F);
    REQUIRE(c1.getGreen() == 0.0F);
    REQUIRE(c1.getBlue() == 0.0F);
    REQUIRE(c1.getAlpha() == 0.0F);

    slider.setColor(moci::Colors::black);

    auto c2 = slider.getColor();
    REQUIRE(c2.getRed() == 0.0F);
    REQUIRE(c2.getGreen() == 0.0F);
    REQUIRE(c2.getBlue() == 0.0F);
    REQUIRE(c2.getAlpha() == 1.0F);
}

TEST_CASE("widget: SliderCallbackValueChanged", "[ui]")
{
    auto wasCalled         = false;
    auto callbacks         = moci::SliderCallbacks {};
    callbacks.valueChanged = [&](auto val)
    {
        REQUIRE(val == 143.0F);
        wasCalled = true;
    };

    auto slider = moci::Slider {moci::Colors::black, callbacks};
    REQUIRE(slider.getValue() == 0.0F);
    slider.setValue(143.0F);
    REQUIRE(slider.getValue() == 143.0F);
    REQUIRE(wasCalled == true);
}

TEST_CASE("widget: SliderCallbackValueChangedCallbackNotSet", "[ui]")
{
    auto wasCalled = false;
    auto callbacks = moci::SliderCallbacks {};
    auto slider    = moci::Slider {moci::Colors::black, callbacks};
    REQUIRE(slider.getValue() == 0.0F);
    slider.setValue(143.0F);
    REQUIRE(slider.getValue() == 143.0F);
    REQUIRE(wasCalled == false);
}

TEST_CASE("widget: SliderMouseScrolled", "[ui]")
{
    auto callbacks = moci::SliderCallbacks {};
    auto slider    = moci::Slider {moci::Colors::black, callbacks};

    slider.setValue(0.5F);
    REQUIRE(slider.getValue() == 0.5F);
    slider.mouseScrolled(moci::MouseScrolledEvent {0.0F, 1.0F});
    REQUIRE(slider.getValue() == Catch::Approx(0.55F));

    slider.setValue(1.0F);
    REQUIRE(slider.getValue() == 1.0F);
    slider.mouseScrolled(moci::MouseScrolledEvent {0.0F, -1.0F});
    REQUIRE(slider.getValue() == Catch::Approx(0.95F));
}
