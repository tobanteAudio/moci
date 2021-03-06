/**
 * @file color_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "color.hpp"

TEST_CASE("moci_graphics_base: ColorDefaultConstructor", "[graphics]")
{
    moci::Color color {};
    REQUIRE(color.GetRed() == 0.0f);
    REQUIRE(color.GetGreen() == 0.0f);
    REQUIRE(color.GetBlue() == 0.0f);
    REQUIRE(color.GetAlpha() == 0.0f);
}

TEST_CASE("moci_graphics_base: ColorRGBConstructor", "[graphics]")
{
    moci::Color color {1.0f, 1.0f, 1.0f};

    REQUIRE(color.GetRed() == 1.0f);
    REQUIRE(color.GetGreen() == 1.0f);
    REQUIRE(color.GetBlue() == 1.0f);
    REQUIRE(color.GetAlpha() == 1.0f);
}

TEST_CASE("moci_graphics_base: ColorRGBAConstructor", "[graphics]")
{
    moci::Color color {1.0f, 1.0f, 1.0f, 0.5f};

    REQUIRE(color.GetRed() == 1.0f);
    REQUIRE(color.GetGreen() == 1.0f);
    REQUIRE(color.GetBlue() == 1.0f);
    REQUIRE(color.GetAlpha() == 0.5f);
}

TEST_CASE("moci_graphics_base: ColorGetData", "[graphics]")
{
    constexpr moci::Color color {};
    auto const data = color.GetData();

    REQUIRE(data.r == 0.0f);
    REQUIRE(data.g == 0.0f);
    REQUIRE(data.b == 0.0f);
    REQUIRE(data.a == 0.0f);
}

TEST_CASE("moci_graphics_base: ColorSetRGBA", "[graphics]")
{
    moci::Color color {};
    color.SetRed(1.0f);
    color.SetGreen(1.0f);
    color.SetBlue(1.0f);
    color.SetAlpha(1.0f);

    REQUIRE(color.GetRed() == 1.0f);
    REQUIRE(color.GetGreen() == 1.0f);
    REQUIRE(color.GetBlue() == 1.0f);
    REQUIRE(color.GetAlpha() == 1.0f);
}