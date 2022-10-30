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
    moci::Slider const slider {moci::Colors::Black};
    REQUIRE(slider.GetValue() == 0.0F);
}

TEST_CASE("widget: SliderSetValue", "[ui]")
{
    moci::Slider slider {moci::Colors::Black};
    REQUIRE(slider.GetValue() == 0.0F);
    slider.SetValue(143.0F);
    REQUIRE(slider.GetValue() == 143.0F);
}

TEST_CASE("widget: SliderSetColor", "[ui]")
{
    moci::Slider slider {{}};

    auto c1 = slider.GetColor();
    REQUIRE(c1.GetRed() == 0.0F);
    REQUIRE(c1.GetGreen() == 0.0F);
    REQUIRE(c1.GetBlue() == 0.0F);
    REQUIRE(c1.GetAlpha() == 0.0F);

    slider.SetColor(moci::Colors::Black);

    auto c2 = slider.GetColor();
    REQUIRE(c2.GetRed() == 0.0F);
    REQUIRE(c2.GetGreen() == 0.0F);
    REQUIRE(c2.GetBlue() == 0.0F);
    REQUIRE(c2.GetAlpha() == 1.0F);
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

    auto slider = moci::Slider {moci::Colors::Black, callbacks};
    REQUIRE(slider.GetValue() == 0.0F);
    slider.SetValue(143.0F);
    REQUIRE(slider.GetValue() == 143.0F);
    REQUIRE(wasCalled == true);
}

TEST_CASE("widget: SliderCallbackValueChangedCallbackNotSet", "[ui]")
{
    auto wasCalled = false;
    auto callbacks = moci::SliderCallbacks {};
    auto slider    = moci::Slider {moci::Colors::Black, callbacks};
    REQUIRE(slider.GetValue() == 0.0F);
    slider.SetValue(143.0F);
    REQUIRE(slider.GetValue() == 143.0F);
    REQUIRE(wasCalled == false);
}

TEST_CASE("widget: SliderMouseScrolled", "[ui]")
{
    auto callbacks = moci::SliderCallbacks {};
    auto slider    = moci::Slider {moci::Colors::Black, callbacks};

    slider.SetValue(0.5F);
    REQUIRE(slider.GetValue() == 0.5F);
    slider.MouseScrolled(moci::MouseScrolledEvent {0.0F, 1.0F});
    REQUIRE(slider.GetValue() == Catch::Approx(0.55F));

    slider.SetValue(1.0F);
    REQUIRE(slider.GetValue() == 1.0F);
    slider.MouseScrolled(moci::MouseScrolledEvent {0.0F, -1.0F});
    REQUIRE(slider.GetValue() == Catch::Approx(0.95F));
}
