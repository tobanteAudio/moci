/**
 * @file key_event_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci_events/events/key_event.hpp"

using Catch::Matchers::Contains;

TEST_CASE("moci_events/events: KeyPressedEvent", "[app][events]")
{
    auto const event = moci::KeyPressedEvent(1, 1);
    REQUIRE(event.GetStaticType() == moci::EventType::KeyPressed);
    REQUIRE(event.GetName() == std::string_view("KeyPressed"));
    REQUIRE(event.GetKeyCode() == 1);
    REQUIRE(event.GetRepeatCount() == 1);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryInput) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryKeyboard) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryApplication) == false);
    REQUIRE_THAT(event.ToString(), Contains("KeyPressed"));
}

TEST_CASE("moci_events/events: KeyReleasedEvent", "[app][events]")
{
    auto const event = moci::KeyReleasedEvent(1);
    REQUIRE(event.GetStaticType() == moci::EventType::KeyReleased);
    REQUIRE(event.GetName() == std::string_view("KeyReleased"));
    REQUIRE(event.GetKeyCode() == 1);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryInput) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryKeyboard) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryApplication) == false);
    REQUIRE_THAT(event.ToString(), Contains("KeyReleased"));
}

TEST_CASE("moci_events/events: KeyTypedEvent", "[app][events]")
{
    auto const event = moci::KeyTypedEvent(1);
    REQUIRE(event.GetStaticType() == moci::EventType::KeyTyped);
    REQUIRE(event.GetName() == std::string_view("KeyTyped"));
    REQUIRE(event.GetKeyCode() == 1);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryInput) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryKeyboard) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryApplication) == false);
    REQUIRE_THAT(event.ToString(), Contains("KeyTyped"));
}