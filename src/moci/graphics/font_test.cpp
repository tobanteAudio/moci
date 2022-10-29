/**
 * @file font_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "font.hpp"
#include "freetype/library.hpp"

TEST_CASE("graphics: TypefaceConstruct", "[font][graphics]")
{
    moci::Typeface face {"comic-sans"};
    REQUIRE(face.getName() == "comic-sans");
}

TEST_CASE("graphics: TypefaceGetWidthForString", "[font][graphics]")
{
    const auto* const path = "moci_test_data/OpenSans-Bold.ttf";
    auto const opt         = moci::FreetypeLibrary::createTypefaceFromFont(path);
    REQUIRE(opt.has_value() == true);

    auto typeface = opt.value();
    REQUIRE(typeface.getWidthForString("") == 0);
    REQUIRE(typeface.getWidthForString("t") == 21);
    REQUIRE(typeface.getWidthForString("tt") == 42);

    REQUIRE(typeface.getWidthForString("t", 0.5F) == 10);
    REQUIRE(typeface.getWidthForString("tt", 0.5F) == 21);
}
