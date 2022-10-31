/**
 * @file buffer.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include <moci/render/buffer.hpp>

TEST_CASE("render/buffer: BufferElement", "[render]")
{
    moci::BufferElement element {moci::ShaderDataType::Float3, "test"};

    REQUIRE(element.Name == "test");
    REQUIRE(element.Size == sizeof(float) * 3);
    REQUIRE(element.Normalized == false);
    REQUIRE(element.getComponentCount() == 3);
}

TEST_CASE("render/buffer: BufferLayout", "[render]")
{
    moci::BufferLayout layout = {
        {moci::ShaderDataType::Float3, "position"},  //
        {moci::ShaderDataType::Float4, "color"},     //
    };

    REQUIRE(layout.getStride() == sizeof(float) * 7);

    REQUIRE(layout.getElements().at(0).Name == "position");
    REQUIRE(layout.getElements().at(0).Size == sizeof(float) * 3);

    REQUIRE(layout.getElements().at(1).Name == "color");
    REQUIRE(layout.getElements().at(1).Size == sizeof(float) * 4);
}