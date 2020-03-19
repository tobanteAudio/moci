/**
 * @file file_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci/render/obj/file.hpp"

#include "moci/core/preprocessor.hpp"

#if !defined(MOCI_WINDOWS)
TEST_CASE("moci/render: OBJFileCube", "[render]")
{
    moci::OBJFile model("test_data/cube.obj");
    REQUIRE(model.Parse() == true);
    REQUIRE(model.GetVertices().size() == 8);
    REQUIRE(model.GetNormals().size() == 6);
    REQUIRE(model.GetVertexData().size() == 36);
}

TEST_CASE("moci/render: OBJFileTeapot", "[render]")
{
    moci::OBJFile model("test_data/teapot.obj");
    REQUIRE(model.Parse() == true);
    REQUIRE(model.GetVertices().size() == 3644);
    REQUIRE(model.GetNormals().size() == 0);
    REQUIRE(model.GetVertexData().size() == 18960);
}

TEST_CASE("moci/render: OBJFileRedPepper", "[render]")
{
    moci::OBJFile model("test_data/red_pepper.obj");
    REQUIRE(model.Parse() == true);
    REQUIRE(model.GetVertices().size() == 14980);
    REQUIRE(model.GetNormals().size() == 0);
    REQUIRE(model.GetVertexData().size() == 89868);
}
#endif