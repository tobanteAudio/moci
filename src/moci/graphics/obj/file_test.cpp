/**
 * @file file_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "file.hpp"

#include "moci/core/preprocessor.hpp"

#if !defined(MOCI_WINDOWS)
TEST_CASE("graphics: OBJFileCube", "[graphics][obj]")
{
    moci::OBJFile model("moci_test_data/cube.obj");
    REQUIRE(model.Parse() == true);
    REQUIRE(model.GetVertices().size() == 8);
    REQUIRE(model.GetNormals().size() == 6);
    REQUIRE(model.GetVertexData().size() == 36);
}

TEST_CASE("graphics: OBJFileTeapot", "[graphics][obj]")
{
    moci::OBJFile model("moci_test_data/teapot.obj");
    REQUIRE(model.Parse() == true);
    REQUIRE(model.GetVertices().size() == 3644);
    REQUIRE(model.GetNormals().empty());
    REQUIRE(model.GetVertexData().size() == 18960);
}

#endif