/**
 * @file widgets_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

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