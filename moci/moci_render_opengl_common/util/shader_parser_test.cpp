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
        auto const program = moci::ShaderParser::SplitSource("");
        REQUIRE(program.sources.size() == 0);
    }

    SECTION("Inline source")
    {
        auto const program = moci::ShaderParser::SplitSource(R"(
            #type vertex
            foo
            #type fragment
            foo
            
        )");
        REQUIRE(!program.sources[0].source.empty());
        REQUIRE(program.sources[0].type == moci::ShaderType::Vertex);
        REQUIRE(!program.sources[1].source.empty());
        REQUIRE(program.sources[1].type == moci::ShaderType::Fragment);
    }
}