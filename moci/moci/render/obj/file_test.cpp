/**
 * @file file_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci/render/obj/file.hpp"

#include "moci/core/preprocessor.hpp"

#if !defined(MOCI_WINDOWS)
TEST_CASE("moci/render: OBJFile", "[render]")
{
    moci::OBJFile model("test_data/cube.obj");
    REQUIRE(model.Parse() == true);
    REQUIRE(model.GetVertices().size() == 8);
    REQUIRE(model.GetNormals().size() == 6);
    REQUIRE(model.GetVertexData().size() == 36);
}
#endif