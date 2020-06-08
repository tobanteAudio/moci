/**
 * @file buffer_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci_render_base/moci_render_base.hpp"

TEST_CASE("moci/render: BufferElement", "[render]")
{
    moci::BufferElement element {moci::ShaderDataType::Float3, "test"};

    REQUIRE(element.Name == "test");
    REQUIRE(element.Size == sizeof(float) * 3);
    REQUIRE(element.Normalized == false);
    REQUIRE(element.GetComponentCount() == 3);
}

TEST_CASE("moci/render: BufferLayout", "[render]")
{
    moci::BufferLayout layout = {
        {moci::ShaderDataType::Float3, "position"},  //
        {moci::ShaderDataType::Float4, "color"},     //
    };

    REQUIRE(layout.GetStride() == sizeof(float) * 7);

    REQUIRE(layout.GetElements().at(0).Name == "position");
    REQUIRE(layout.GetElements().at(0).Size == sizeof(float) * 3);

    REQUIRE(layout.GetElements().at(1).Name == "color");
    REQUIRE(layout.GetElements().at(1).Size == sizeof(float) * 4);
}