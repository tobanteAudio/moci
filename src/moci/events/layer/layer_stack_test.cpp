/**
 * @file layer_stack_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "moci/events/layer/layer_stack.hpp"

#include <iterator>
#include <memory>
#include <string>
#include <utility>

namespace
{
class TestLayer : public moci::Layer
{
public:
    explicit TestLayer(std::string n) : moci::Layer(std::move(n)) { }
    ~TestLayer() override = default;
};
}  // namespace

TEST_CASE("events/layer: Layer", "[events][layer]")
{
    moci::Scope<moci::Layer> layer(new TestLayer("test"));
    REQUIRE(layer->GetName() == "test");
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
    stack.PushLayer(moci::MakeScope<TestLayer>("test"));
    // REQUIRE(std::distance(stack.begin(), stack.end()) == 1);
}