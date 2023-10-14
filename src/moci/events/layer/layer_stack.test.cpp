/**
 * @file layer_stack.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "layer_stack.hpp"

#include <catch2/catch_all.hpp>

#include <iterator>
#include <memory>
#include <string>
#include <utility>

namespace {
class TestLayer : public moci::Layer
{
public:
    explicit TestLayer(std::string n) : moci::Layer(std::move(n)) {}

    ~TestLayer() override = default;
};
}  // namespace

TEST_CASE("events/layer: Layer", "[events][layer]")
{
    std::unique_ptr<moci::Layer> layer(new TestLayer("test"));
    REQUIRE(layer->getName() == "test");
}

TEST_CASE("events/layer: LayerStackConstructor", "[events][layer]")
{
    auto stack = moci::LayerStack();
    REQUIRE(std::distance(stack.begin(), stack.end()) == 0);
}

TEST_CASE("events/layer: LayerStackPushLayer", "[events][layer]")
{
    auto stack = moci::LayerStack();
    REQUIRE(stack.begin() == stack.end());
    stack.pushLayer(std::make_unique<TestLayer>("test"));
    // REQUIRE(std::distance(stack.begin(), stack.end()) == 1);
}
