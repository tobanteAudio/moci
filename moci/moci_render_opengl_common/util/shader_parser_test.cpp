/**
 * @file shader_parser_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "shader_parser.hpp"

TEST_CASE("moci_render_opengl_common: ShaderParserSplitSource", "[opengl][common]")
{
    SECTION("Empty source")
    {
        auto const sources = moci::ShaderParser::SplitSource("");
        REQUIRE(sources.VertexSource.length() == 0);
        REQUIRE(sources.FragmentSource.length() == 0);
    }

    SECTION("Inline source")
    {
        auto const sources = moci::ShaderParser::SplitSource(R"(
            #type vertex
            foo
            #type fragment
            foo
            
        )");
        REQUIRE(!sources.VertexSource.empty());
        REQUIRE(!sources.FragmentSource.empty());
    }
}