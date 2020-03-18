/**
 * @file file_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci/render/obj/file.hpp"

TEST_CASE("moci/render: OBJFile", "[render]")
{
    moci::OBJFile model("/home/tobante/Developer/tobanteAudio/moci/sandbox3D/cube.obj");
    REQUIRE(model.Parse() == true);
    // REQUIRE(model.GetVertices().size() == 8);
}
