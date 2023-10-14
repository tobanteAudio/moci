/**
 * @file file.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "file.hpp"

#include <moci/core/preprocessor.hpp>

#if !defined(MOCI_WINDOWS)
TEST_CASE("graphics: OBJFileCube", "[graphics][obj]")
{
    moci::OBJFile model("moci_test_data/cube.obj");
    REQUIRE(model.parse() == true);
    REQUIRE(model.getVertices().size() == 8);
    REQUIRE(model.getNormals().size() == 6);
    REQUIRE(model.getVertexData().size() == 36);
}

TEST_CASE("graphics: OBJFileTeapot", "[graphics][obj]")
{
    moci::OBJFile model("moci_test_data/teapot.obj");
    REQUIRE(model.parse() == true);
    REQUIRE(model.getVertices().size() == 3644);
    REQUIRE(model.getNormals().empty());
    REQUIRE(model.getVertexData().size() == 18960);
}

#endif
