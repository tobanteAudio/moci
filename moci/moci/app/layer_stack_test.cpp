/**
 * @file layer_stack_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci/app/layer_stack.hpp"

#include <iterator>
#include <memory>
#include <string>
#include <utility>

namespace
{
class TestLayer : public moci::Layer
{
public:
    TestLayer(std::string n) : moci::Layer(std::move(n)) {}
    ~TestLayer() override = default;
};
}  // namespace

TEST_CASE("moci/app: Layer", "[app]")
{
    std::unique_ptr<moci::Layer> layer(new TestLayer("test"));
    REQUIRE(layer->GetName() == "test");
}

TEST_CASE("moci/app: LayerStackConstructor", "[app]")
{
    auto stack = moci::LayerStack();
    REQUIRE(std::distance(stack.begin(), stack.end()) == 0);
}

TEST_CASE("moci/app: LayerStackPushLayer", "[app]")
{
    auto stack = moci::LayerStack();
    REQUIRE(stack.begin() == stack.end());
    stack.PushLayer(new TestLayer("test"));
    REQUIRE(std::distance(stack.begin(), stack.end()) == 1);
}