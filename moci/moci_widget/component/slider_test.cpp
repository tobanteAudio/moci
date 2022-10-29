/**
 * @file slider_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>
#include <catch2/catch_approx.hpp>

#include "moci_widget/component/slider.hpp"
#include "moci_widget/style/style.hpp"

TEST_CASE("moci_widget: SliderConstruct", "[ui]")
{
    moci::Slider const slider {moci::Colors::Black};
    REQUIRE(slider.GetValue() == 0.0f);
}

TEST_CASE("moci_widget: SliderSetValue", "[ui]")
{
    moci::Slider slider {moci::Colors::Black};
    REQUIRE(slider.GetValue() == 0.0f);
    slider.SetValue(143.0f);
    REQUIRE(slider.GetValue() == 143.0f);
}

TEST_CASE("moci_widget: SliderSetColor", "[ui]")
{
    moci::Slider slider {{}};

    auto c1 = slider.GetColor();
    REQUIRE(c1.GetRed() == 0.0f);
    REQUIRE(c1.GetGreen() == 0.0f);
    REQUIRE(c1.GetBlue() == 0.0f);
    REQUIRE(c1.GetAlpha() == 0.0f);

    slider.SetColor(moci::Colors::Black);

    auto c2 = slider.GetColor();
    REQUIRE(c2.GetRed() == 0.0f);
    REQUIRE(c2.GetGreen() == 0.0f);
    REQUIRE(c2.GetBlue() == 0.0f);
    REQUIRE(c2.GetAlpha() == 1.0f);
}

TEST_CASE("moci_widget: SliderCallbackValueChanged", "[ui]")
{
    auto wasCalled         = false;
    auto callbacks         = moci::SliderCallbacks {};
    callbacks.valueChanged = [&](auto val)
    {
        REQUIRE(val == 143.0f);
        wasCalled = true;
    };

    auto slider = moci::Slider {moci::Colors::Black, callbacks};
    REQUIRE(slider.GetValue() == 0.0f);
    slider.SetValue(143.0f);
    REQUIRE(slider.GetValue() == 143.0f);
    REQUIRE(wasCalled == true);
}

TEST_CASE("moci_widget: SliderCallbackValueChangedCallbackNotSet", "[ui]")
{
    auto wasCalled = false;
    auto callbacks = moci::SliderCallbacks {};
    auto slider    = moci::Slider {moci::Colors::Black, callbacks};
    REQUIRE(slider.GetValue() == 0.0f);
    slider.SetValue(143.0f);
    REQUIRE(slider.GetValue() == 143.0f);
    REQUIRE(wasCalled == false);
}

TEST_CASE("moci_widget: SliderMouseScrolled", "[ui]")
{
    auto callbacks = moci::SliderCallbacks {};
    auto slider    = moci::Slider {moci::Colors::Black, callbacks};

    slider.SetValue(0.5f);
    REQUIRE(slider.GetValue() == 0.5f);
    slider.MouseScrolled(moci::MouseScrolledEvent {0.0f, 1.0f});
    REQUIRE(slider.GetValue() == Catch::Approx(0.55f));

    slider.SetValue(1.0f);
    REQUIRE(slider.GetValue() == 1.0f);
    slider.MouseScrolled(moci::MouseScrolledEvent {0.0f, -1.0f});
    REQUIRE(slider.GetValue() == Catch::Approx(0.95f));
}
