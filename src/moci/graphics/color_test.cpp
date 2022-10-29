/**
 * @file color_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "color.hpp"

TEST_CASE("graphics: ColorDefaultConstructor", "[graphics]")
{
    moci::Color color {};
    REQUIRE(color.getRed() == 0.0F);
    REQUIRE(color.getGreen() == 0.0F);
    REQUIRE(color.getBlue() == 0.0F);
    REQUIRE(color.getAlpha() == 0.0F);
}

TEST_CASE("graphics: ColorRGBConstructor", "[graphics]")
{
    moci::Color color {1.0F, 1.0F, 1.0F};

    REQUIRE(color.getRed() == 1.0F);
    REQUIRE(color.getGreen() == 1.0F);
    REQUIRE(color.getBlue() == 1.0F);
    REQUIRE(color.getAlpha() == 1.0F);
}

TEST_CASE("graphics: ColorRGBAConstructor", "[graphics]")
{
    moci::Color color {1.0F, 1.0F, 1.0F, 0.5F};

    REQUIRE(color.getRed() == 1.0F);
    REQUIRE(color.getGreen() == 1.0F);
    REQUIRE(color.getBlue() == 1.0F);
    REQUIRE(color.getAlpha() == 0.5F);
}

TEST_CASE("graphics: ColorGetData", "[graphics]")
{
    constexpr moci::Color color {};
    auto const data = color.getData();

    REQUIRE(data.r == 0.0F);
    REQUIRE(data.g == 0.0F);
    REQUIRE(data.b == 0.0F);
    REQUIRE(data.a == 0.0F);
}

TEST_CASE("graphics: ColorSetRGBA", "[graphics]")
{
    moci::Color color {};
    color.setRed(1.0F);
    color.setGreen(1.0F);
    color.setBlue(1.0F);
    color.setAlpha(1.0F);

    REQUIRE(color.getRed() == 1.0F);
    REQUIRE(color.getGreen() == 1.0F);
    REQUIRE(color.getBlue() == 1.0F);
    REQUIRE(color.getAlpha() == 1.0F);
}