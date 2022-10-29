/**
 * @file font_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "font.hpp"
#include "freetype/library.hpp"

TEST_CASE("moci_graphics_base: TypefaceConstruct", "[font][graphics]")
{
    moci::Typeface face {"comic-sans"};
    REQUIRE(face.GetName() == "comic-sans");
}

TEST_CASE("moci_graphics_base: TypefaceGetWidthForString", "[font][graphics]")
{
    auto const path = "moci_test_data/OpenSans-Bold.ttf";
    auto const opt  = moci::FreetypeLibrary::CreateTypefaceFromFont(path);
    REQUIRE(opt.has_value() == true);

    auto typeface = opt.value();
    REQUIRE(typeface.GetWidthForString("") == 0);
    REQUIRE(typeface.GetWidthForString("t") == 21);
    REQUIRE(typeface.GetWidthForString("tt") == 42);

    REQUIRE(typeface.GetWidthForString("t", 0.5f) == 10);
    REQUIRE(typeface.GetWidthForString("tt", 0.5f) == 21);
}
