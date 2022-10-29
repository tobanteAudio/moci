/**
 * @file library_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "library.hpp"

TEST_CASE("graphics/freetype: FreetypeCreateTypeface", "[graphics][font]")
{
    const auto* const path = "moci_test_data/OpenSans-Bold.ttf";
    auto const typeface    = moci::FreetypeLibrary::CreateTypefaceFromFont(path);
    REQUIRE(typeface.has_value() == true);
    REQUIRE(typeface.value().GetNumCharacters() == 128);
}

TEST_CASE("graphics/freetype: FreetypeCreateTypefaceNoPath", "[graphics][font]")
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
