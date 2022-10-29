/**
 * @file color_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "color.hpp"

TEST_CASE("moci_graphics_base: ColorDefaultConstructor", "[graphics]")
{
    moci::Color color {};
    REQUIRE(color.GetRed() == 0.0F);
    REQUIRE(color.GetGreen() == 0.0F);
    REQUIRE(color.GetBlue() == 0.0F);
    REQUIRE(color.GetAlpha() == 0.0F);
}

TEST_CASE("moci_graphics_base: ColorRGBConstructor", "[graphics]")
{
    moci::Color color {1.0F, 1.0F, 1.0F};

    REQUIRE(color.GetRed() == 1.0F);
    REQUIRE(color.GetGreen() == 1.0F);
    REQUIRE(color.GetBlue() == 1.0F);
    REQUIRE(color.GetAlpha() == 1.0F);
}

TEST_CASE("moci_graphics_base: ColorRGBAConstructor", "[graphics]")
{
    moci::Color color {1.0F, 1.0F, 1.0F, 0.5F};

    REQUIRE(color.GetRed() == 1.0F);
    REQUIRE(color.GetGreen() == 1.0F);
    REQUIRE(color.GetBlue() == 1.0F);
    REQUIRE(color.GetAlpha() == 0.5F);
}

TEST_CASE("moci_graphics_base: ColorGetData", "[graphics]")
{
    constexpr moci::Color color {};
    auto const data = color.GetData();

    REQUIRE(data.r == 0.0F);
    REQUIRE(data.g == 0.0F);
    REQUIRE(data.b == 0.0F);
    REQUIRE(data.a == 0.0F);
}

TEST_CASE("moci_graphics_base: ColorSetRGBA", "[graphics]")
{
    moci::Color color {};
    color.SetRed(1.0F);
    color.SetGreen(1.0F);
    color.SetBlue(1.0F);
    color.SetAlpha(1.0F);

    REQUIRE(color.GetRed() == 1.0F);
    REQUIRE(color.GetGreen() == 1.0F);
    REQUIRE(color.GetBlue() == 1.0F);
    REQUIRE(color.GetAlpha() == 1.0F);
}