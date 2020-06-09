/**
 * @file image_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci_graphics/image.hpp"

TEST_CASE("moci_graphics: ImageDefaultConstructor", "[render]")
{
    moci::Image img {};
    REQUIRE(img.GetWidth() == 0);
    REQUIRE(img.GetHeight() == 0);
    REQUIRE(img.Empty() == true);
    REQUIRE(img.Data().empty());
}

TEST_CASE("moci_graphics: ImagePathConstructor", "[render]")
{
    moci::Image img {"moci_test_data/colors.png"};
    REQUIRE(img.GetWidth() == 256);
    REQUIRE(img.GetHeight() == 256);
    REQUIRE(img.GetNumChannels() == 3);
    REQUIRE(img.Empty() == false);
    REQUIRE(img.Data().size() == 196608);
}

TEST_CASE("moci_graphics: ImageLoadFromFileSuccess", "[render]")
{
    moci::Image img {};
    REQUIRE(img.LoadFromFile("moci_test_data/colors.png") == true);
    REQUIRE(img.GetWidth() == 256);
    REQUIRE(img.GetHeight() == 256);
    REQUIRE(img.GetNumChannels() == 3);
    REQUIRE(img.Empty() == false);
    REQUIRE(img.Data().size() == 196608);
}

TEST_CASE("moci_graphics: ImageLoadFromFileFail", "[render]")
{
    moci::Image img {};
    REQUIRE(img.LoadFromFile("noexist.png") == false);
    REQUIRE(img.GetWidth() == 0);
    REQUIRE(img.GetHeight() == 0);
    REQUIRE(img.GetNumChannels() == 0);
    REQUIRE(img.Empty() == true);
    REQUIRE(img.Data().empty());
}