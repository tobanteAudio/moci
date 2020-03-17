/**
 * @file library_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci/render/freetype/library.hpp"

// TEST_CASE("moci/render: FreetypeCreateTypeface", "[render]")
// {
//     auto const path     = "test_data/OpenSans-Bold.ttf";
//     auto const typeface = moci::FreetypeLibrary::CreateTypefaceFromFont(path);
//     REQUIRE(typeface.has_value() == true);
//     REQUIRE(typeface.value().GetNumCharacters() == 128);
// }

TEST_CASE("moci/render: FreetypeCreateTypefaceNoPath", "[render]")
{
    {
        auto typeface = moci::FreetypeLibrary::CreateTypefaceFromFont("");
        REQUIRE(typeface.has_value() == false);
    }
    {
        auto typeface = moci::FreetypeLibrary::CreateTypefaceFromFont("noexist");
        REQUIRE(typeface.has_value() == false);
    }
}
