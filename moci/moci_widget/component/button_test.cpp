/**
 * @file button_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci_widget/component/button.hpp"
#include "moci_widget/style/style.hpp"

TEST_CASE("moci_widget: ButtonConstruct", "[ui]")
{
    moci::Button const button {"test"};
    REQUIRE(button.GetText() == "test");
    REQUIRE(button.GetTextView() == "test");
}

TEST_CASE("moci_widget: ButtonSetText", "[ui]")
{
    moci::Button button {""};
    REQUIRE(button.GetText().empty());
    REQUIRE(button.GetTextView() == "");
    button.SetText("test143");
    REQUIRE(button.GetText() == "test143");
    REQUIRE(button.GetTextView() == "test143");
}

TEST_CASE("moci_widget: ButtonSetTextColor", "[ui]")
{
    moci::Button button {""};

    auto c1 = button.GetTextColor();
    REQUIRE(c1.GetRed() == 0.0f);
    REQUIRE(c1.GetGreen() == 0.0f);
    REQUIRE(c1.GetBlue() == 0.0f);
    REQUIRE(c1.GetAlpha() == 0.0f);

    button.SetTextColor(moci::Colors::Black);

    auto c2 = button.GetTextColor();
    REQUIRE(c2.GetRed() == 0.0f);
    REQUIRE(c2.GetGreen() == 0.0f);
    REQUIRE(c2.GetBlue() == 0.0f);
    REQUIRE(c2.GetAlpha() == 1.0f);
}

TEST_CASE("moci_widget: ButtonSetSpecs", "[ui]")
{
    moci::Button button {""};

    {
        auto specs = button.GetSpecs();
        REQUIRE(specs.callbacks.onClick == nullptr);
        REQUIRE(specs.isToggle == false);
    }

    auto newSpecs              = moci::ButtonSpecs {};
    newSpecs.callbacks.onClick = [](auto state) { moci::IgnoreUnused(state); };
    newSpecs.isToggle          = true;
    button.SetSpecs(newSpecs);

    {
        auto specs = button.GetSpecs();
        REQUIRE(specs.callbacks.onClick != nullptr);
        REQUIRE(specs.isToggle == true);
    }
}

TEST_CASE("moci_widget: ButtonSetState", "[ui]")
{
    moci::Button button {""};
    REQUIRE(button.GetState() == moci::ButtonState::Normal);

    button.SetState(moci::ButtonState::Down);
    REQUIRE(button.GetState() == moci::ButtonState::Down);
}

TEST_CASE("moci_widget: ButtonCallbackStateChanged", "[ui]")
{
    auto wasCalled                = false;
    auto specs                    = moci::ButtonSpecs {};
    specs.callbacks.onStateChange = [&](auto state) {
        REQUIRE(state == moci::ButtonState::Down);
        wasCalled = true;
    };

    auto slider = moci::Button {"test", specs};
    REQUIRE(slider.GetState() == moci::ButtonState::Normal);
    slider.SetState(moci::ButtonState::Down);
    REQUIRE(slider.GetState() == moci::ButtonState::Down);
    REQUIRE(wasCalled == true);
}

TEST_CASE("moci_widget: ButtonCallbackStateChangedCallbackNotSet", "[ui]")
{
    auto wasCalled = false;
    auto specs     = moci::ButtonSpecs {};
    auto slider    = moci::Button {"test", specs};
    REQUIRE(slider.GetState() == moci::ButtonState::Normal);
    slider.SetState(moci::ButtonState::Down);
    REQUIRE(slider.GetState() == moci::ButtonState::Down);
    REQUIRE(wasCalled == false);
}
