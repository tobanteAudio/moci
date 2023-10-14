/**
 * @file color.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "color.hpp"

TEST_CASE("graphics: ColorDefaultConstructor", "[graphics]")
{
    moci::ColorRGBA32 color {};
    REQUIRE(color.red() == 0.0F);
    REQUIRE(color.green() == 0.0F);
    REQUIRE(color.blue() == 0.0F);
    REQUIRE(color.alpha() == 0.0F);
}

TEST_CASE("graphics: ColorRGBConstructor", "[graphics]")
{
    moci::ColorRGBA32 color {1.0F, 1.0F, 1.0F};

    REQUIRE(color.red() == 1.0F);
    REQUIRE(color.green() == 1.0F);
    REQUIRE(color.blue() == 1.0F);
    REQUIRE(color.alpha() == 1.0F);
}

TEST_CASE("graphics: ColorRGBAConstructor", "[graphics]")
{
    moci::ColorRGBA32 color {1.0F, 1.0F, 1.0F, 0.5F};

    REQUIRE(color.red() == 1.0F);
    REQUIRE(color.green() == 1.0F);
    REQUIRE(color.blue() == 1.0F);
    REQUIRE(color.alpha() == 0.5F);
}

TEST_CASE("graphics: ColorGetData", "[graphics]")
{
    constexpr moci::ColorRGBA32 color {};
    auto const data = static_cast<glm::vec4>(color);

    REQUIRE(data.r == 0.0F);
    REQUIRE(data.g == 0.0F);
    REQUIRE(data.b == 0.0F);
    REQUIRE(data.a == 0.0F);
}

TEST_CASE("graphics: ColorSetRGBA", "[graphics]")
{
    moci::ColorRGBA32 color {};
    color.red(1.0F);
    color.green(1.0F);
    color.blue(1.0F);
    color.alpha(1.0F);

    REQUIRE(color.red() == 1.0F);
    REQUIRE(color.green() == 1.0F);
    REQUIRE(color.blue() == 1.0F);
    REQUIRE(color.alpha() == 1.0F);
}
