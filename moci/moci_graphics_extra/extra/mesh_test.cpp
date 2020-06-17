/**
 * @file mesh_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "mesh.hpp"

TEST_CASE("moci_graphics_extra: MeshLoadFromFile", "[graphics]")
{
    SECTION("Cube")
    {
        auto const mesh = moci::Mesh {"moci_test_data/cube.obj"};
        REQUIRE(mesh.GetVertices().size() == 36);
    }
    SECTION("Teapot")
    {
        auto const mesh = moci::Mesh {"moci_test_data/teapot.obj"};
        REQUIRE(mesh.GetVertices().size() == 18960);
    }
}