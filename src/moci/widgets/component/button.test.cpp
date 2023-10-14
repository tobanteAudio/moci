/**
 * @file button.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include <moci/widgets/component/button.hpp>
#include <moci/widgets/style/style.hpp>

TEST_CASE("widget: ButtonConstruct", "[ui]")
{
    moci::Button const button{"test"};
    REQUIRE(button.getText() == "test");
    REQUIRE(button.getTextView() == "test");
}

TEST_CASE("widget: ButtonSetText", "[ui]")
{
    moci::Button button{""};
    REQUIRE(button.getText().empty());
    REQUIRE(button.getTextView() == "");
    button.SetText("test143");
    REQUIRE(button.getText() == "test143");
    REQUIRE(button.getTextView() == "test143");
}

TEST_CASE("widget: ButtonSetTextColor", "[ui]")
{
    moci::Button button{""};

    auto c1 = button.getTextColor();
    REQUIRE(c1.red() == 0.0f);
    REQUIRE(c1.green() == 0.0f);
    REQUIRE(c1.blue() == 0.0f);
    REQUIRE(c1.alpha() == 0.0f);

    button.SetTextColor(moci::Colors::Black);

    auto c2 = button.getTextColor();
    REQUIRE(c2.red() == 0.0f);
    REQUIRE(c2.green() == 0.0f);
    REQUIRE(c2.blue() == 0.0f);
    REQUIRE(c2.alpha() == 1.0f);
}

TEST_CASE("widget: ButtonSetSpecs", "[ui]")
{
    moci::Button button{""};

    {
        auto specs = button.getSpecs();
        REQUIRE(specs.callbacks.clicked == nullptr);
        REQUIRE(specs.isToggle == false);
    }

    auto newSpecs              = moci::ButtonSpecs{};
    newSpecs.callbacks.clicked = [](auto state) { moci::IgnoreUnused(state); };
    newSpecs.isToggle          = true;
    button.SetSpecs(newSpecs);

    {
        auto specs = button.getSpecs();
        REQUIRE(specs.callbacks.clicked != nullptr);
        REQUIRE(specs.isToggle == true);
    }
}

TEST_CASE("widget: ButtonSetState", "[ui]")
{
    moci::Button button{""};
    REQUIRE(button.getState() == moci::ButtonState::Normal);

    button.SetState(moci::ButtonState::Down);
    REQUIRE(button.getState() == moci::ButtonState::Down);
}

TEST_CASE("widget: ButtonCallbackStateChanged", "[ui]")
{
    auto wasCalled               = false;
    auto specs                   = moci::ButtonSpecs{};
    specs.callbacks.stateChanged = [&](auto state) {
        REQUIRE(state == moci::ButtonState::Down);
        wasCalled = true;
    };

    auto slider = moci::Button{"test", specs};
    REQUIRE(slider.getState() == moci::ButtonState::Normal);
    slider.SetState(moci::ButtonState::Down);
    REQUIRE(slider.getState() == moci::ButtonState::Down);
    REQUIRE(wasCalled == true);
}

TEST_CASE("widget: ButtonCallbackStateChangedCallbackNotSet", "[ui]")
{
    auto wasCalled = false;
    auto specs     = moci::ButtonSpecs{};
    auto slider    = moci::Button{"test", specs};
    REQUIRE(slider.getState() == moci::ButtonState::Normal);
    slider.SetState(moci::ButtonState::Down);
    REQUIRE(slider.getState() == moci::ButtonState::Down);
    REQUIRE(wasCalled == false);
}
