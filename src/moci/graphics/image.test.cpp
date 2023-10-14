/**
 * @file image.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "image.hpp"

TEST_CASE("graphics: ImageDefaultConstructor", "[graphics]")
{
    moci::Image img{};
    REQUIRE(img.getWidth() == 0);
    REQUIRE(img.getHeight() == 0);
    REQUIRE(img.empty() == true);
    REQUIRE(img.data().empty());
}

TEST_CASE("graphics: ImagePathConstructor", "[graphics]")
{
    moci::Image img{"moci_test_data/colors.png"};
    REQUIRE(img.getWidth() == 256);
    REQUIRE(img.getHeight() == 256);
    REQUIRE(img.getNumChannels() == 3);
    REQUIRE(img.empty() == false);
    REQUIRE(img.data().size() == 196608);
}

TEST_CASE("graphics: ImageLoadFromFileSuccess", "[graphics]")
{
    moci::Image img{};
    REQUIRE(img.loadFromFile("moci_test_data/colors.png") == true);
    REQUIRE(img.getWidth() == 256);
    REQUIRE(img.getHeight() == 256);
    REQUIRE(img.getNumChannels() == 3);
    REQUIRE(img.empty() == false);
    REQUIRE(img.data().size() == 196608);
}

TEST_CASE("graphics: ImageLoadFromFileFail", "[graphics]")
{
    moci::Image img{};
    REQUIRE(img.loadFromFile("noexist.png") == false);
    REQUIRE(img.getWidth() == 0);
    REQUIRE(img.getHeight() == 0);
    REQUIRE(img.getNumChannels() == 0);
    REQUIRE(img.empty() == true);
    REQUIRE(img.data().empty());
}
