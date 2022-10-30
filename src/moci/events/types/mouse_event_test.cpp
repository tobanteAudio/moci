/**
 * @file mouse_event_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>

#include "moci/events/types/mouse_event.hpp"

// using Catch::Matchers::Contains;

TEST_CASE("events: MouseMovedEvent", "[mouse][events]")
{
    auto const event = moci::MouseMovedEvent(10.0F, 20.0F);
    REQUIRE(event.getStaticType() == moci::EventType::MouseMoved);
    REQUIRE(event.getName() == std::string_view("MouseMoved"));
    REQUIRE(event.getX() == 10.0F);
    REQUIRE(event.getY() == 20.0F);
    REQUIRE(event.isInCategory(moci::EventCategoryInput) == true);
    REQUIRE(event.isInCategory(moci::EventCategoryMouse) == true);
    // REQUIRE_THAT(event.ToString(), Contains("MouseMoved"));
}

TEST_CASE("events: MouseScrolledEvent", "[mouse][events]")
{
    auto const event = moci::MouseScrolledEvent(10.0F, 20.0F);
    REQUIRE(event.getStaticType() == moci::EventType::MouseScrolled);
    REQUIRE(event.getName() == std::string_view("MouseScrolled"));
    REQUIRE(event.getXOffset() == 10.0F);
    REQUIRE(event.getYOffset() == 20.0F);
    REQUIRE(event.isInCategory(moci::EventCategoryInput) == true);
    REQUIRE(event.isInCategory(moci::EventCategoryMouse) == true);
    // REQUIRE_THAT(event.ToString(), Contains("MouseScrolled"));
}

TEST_CASE("events: MouseButtonPressedEvent", "[mouse][events]")
{
    auto const event = moci::MouseButtonPressedEvent(moci::MouseCode::Button2);
    REQUIRE(event.getStaticType() == moci::EventType::MouseButtonPressed);
    REQUIRE(event.getName() == std::string_view("MouseButtonPressed"));
    REQUIRE(event.getMouseButton() == moci::MouseCode::Button2);
    REQUIRE(event.isInCategory(moci::EventCategoryInput) == true);
    REQUIRE(event.isInCategory(moci::EventCategoryMouse) == true);
    // REQUIRE_THAT(event.ToString(), Contains("MouseButtonPressed"));
}

TEST_CASE("events: MouseButtonReleasedEvent", "[mouse][events]")
{
    auto const event = moci::MouseButtonReleasedEvent(moci::MouseCode::ButtonLeft);
    REQUIRE(event.getStaticType() == moci::EventType::MouseButtonReleased);
    REQUIRE(event.getName() == std::string_view("MouseButtonReleased"));
    REQUIRE(event.getMouseButton() == moci::MouseCode::ButtonLeft);
    REQUIRE(event.isInCategory(moci::EventCategoryInput) == true);
    REQUIRE(event.isInCategory(moci::EventCategoryMouse) == true);
    // REQUIRE_THAT(event.ToString(), Contains("MouseButtonReleased"));
}