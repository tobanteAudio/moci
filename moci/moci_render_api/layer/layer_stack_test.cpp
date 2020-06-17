/**
 * @file layer_stack_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci_render_api/moci_render_api.hpp"

#include <iterator>
#include <memory>
#include <string>
#include <utility>

namespace
{
class TestLayer : public moci::Layer
{
public:
    TestLayer(std::string n) : moci::Layer(std::move(n)) { }
    ~TestLayer() override = default;
};
}  // namespace

TEST_CASE("moci_render_api: Layer", "[render]")
{
    moci::Scope<moci::Layer> layer(new TestLayer("test"));
    REQUIRE(layer->GetName() == "test");
}

TEST_CASE("moci_render_api: LayerStackConstructor", "[render]")
{
    auto stack = moci::LayerStack();
    REQUIRE(std::distance(stack.begin(), stack.end()) == 0);
}

TEST_CASE("moci_render_api: LayerStackPushLayer", "[render]")
{
    auto stack = moci::LayerStack();
    REQUIRE(stack.begin() == stack.end());
    stack.PushLayer(moci::MakeScope<TestLayer>("test"));
    // REQUIRE(std::distance(stack.begin(), stack.end()) == 1);
}