/**
 * @file key_event.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>

#include <moci/events/types/key_event.hpp>

// using Catch::Matchers::Contains;

TEST_CASE("events: KeyPressedEvent", "[key][events]")
{
    auto const event = moci::KeyPressedEvent(moci::Key::Escape, 1);
    REQUIRE(event.getStaticType() == moci::EventType::KeyPressed);
    REQUIRE(event.getName() == std::string_view("KeyPressed"));
    REQUIRE(event.getKeyCode() == moci::Key::Escape);
    REQUIRE(event.getRepeatCount() == 1);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryInput) == true);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryKeyboard) == true);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryApplication) == false);
    // REQUIRE_THAT(event.ToString(), Contains("KeyPressed"));
}

TEST_CASE("events: KeyReleasedEvent", "[key][events]")
{
    auto const event = moci::KeyReleasedEvent(moci::Key::A);
    REQUIRE(event.getStaticType() == moci::EventType::KeyReleased);
    REQUIRE(event.getName() == std::string_view("KeyReleased"));
    REQUIRE(event.getKeyCode() == moci::Key::A);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryInput) == true);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryKeyboard) == true);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryApplication) == false);
    // REQUIRE_THAT(event.ToString(), Contains("KeyReleased"));
}

TEST_CASE("events: KeyTypedEvent", "[key][events]")
{
    auto const event = moci::KeyTypedEvent(moci::Key::Y);
    REQUIRE(event.getStaticType() == moci::EventType::KeyTyped);
    REQUIRE(event.getName() == std::string_view("KeyTyped"));
    REQUIRE(event.getKeyCode() == moci::Key::Y);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryInput) == true);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryKeyboard) == true);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryApplication) == false);
    // REQUIRE_THAT(event.ToString(), Contains("KeyTyped"));
}