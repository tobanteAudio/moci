/**
 * @file key_event_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>

#include "moci/events/types/key_event.hpp"

// using Catch::Matchers::Contains;

TEST_CASE("events: KeyPressedEvent", "[key][events]")
{
    auto const event = moci::KeyPressedEvent(moci::Key::Escape, 1);
    REQUIRE(event.GetStaticType() == moci::EventType::KeyPressed);
    REQUIRE(event.GetName() == std::string_view("KeyPressed"));
    REQUIRE(event.GetKeyCode() == moci::Key::Escape);
    REQUIRE(event.GetRepeatCount() == 1);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryInput) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryKeyboard) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryApplication) == false);
    // REQUIRE_THAT(event.ToString(), Contains("KeyPressed"));
}

TEST_CASE("events: KeyReleasedEvent", "[key][events]")
{
    auto const event = moci::KeyReleasedEvent(moci::Key::A);
    REQUIRE(event.GetStaticType() == moci::EventType::KeyReleased);
    REQUIRE(event.GetName() == std::string_view("KeyReleased"));
    REQUIRE(event.GetKeyCode() == moci::Key::A);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryInput) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryKeyboard) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryApplication) == false);
    // REQUIRE_THAT(event.ToString(), Contains("KeyReleased"));
}

TEST_CASE("events: KeyTypedEvent", "[key][events]")
{
    auto const event = moci::KeyTypedEvent(moci::Key::Y);
    REQUIRE(event.GetStaticType() == moci::EventType::KeyTyped);
    REQUIRE(event.GetName() == std::string_view("KeyTyped"));
    REQUIRE(event.GetKeyCode() == moci::Key::Y);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryInput) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryKeyboard) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryApplication) == false);
    // REQUIRE_THAT(event.ToString(), Contains("KeyTyped"));
}