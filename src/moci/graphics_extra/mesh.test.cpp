/**
 * @file mesh.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "mesh.hpp"

TEST_CASE("graphics_extra: MeshLoadFromFile", "[graphics]")
{
    SECTION("Cube")
    {
        auto const mesh = moci::Mesh{"moci_test_data/cube.obj"};
        REQUIRE(mesh.getVertices().size() == 36);
    }
    SECTION("Teapot")
    {
        auto const mesh = moci::Mesh{"moci_test_data/teapot.obj"};
        REQUIRE(mesh.getVertices().size() == 18960);
    }
}
