/**
 * @file mouse_event_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci_events/events/mouse_event.hpp"

using Catch::Matchers::Contains;

TEST_CASE("moci_events/events: MouseMovedEvent", "[app][events]")
{
    auto const event = moci::MouseMovedEvent(10.0f, 20.0f);
    REQUIRE(event.GetStaticType() == moci::EventType::MouseMoved);
    REQUIRE(event.GetName() == std::string_view("MouseMoved"));
    REQUIRE(event.GetX() == 10.0f);
    REQUIRE(event.GetY() == 20.0f);
    REQUIRE(event.IsInCategory(moci::EventCategoryInput) == true);
    REQUIRE(event.IsInCategory(moci::EventCategoryMouse) == true);
    REQUIRE_THAT(event.ToString(), Contains("MouseMoved"));
}

TEST_CASE("moci_events/events: MouseScrolledEvent", "[app][events]")
{
    auto const event = moci::MouseScrolledEvent(10.0f, 20.0f);
    REQUIRE(event.GetStaticType() == moci::EventType::MouseScrolled);
    REQUIRE(event.GetName() == std::string_view("MouseScrolled"));
    REQUIRE(event.GetXOffset() == 10.0f);
    REQUIRE(event.GetYOffset() == 20.0f);
    REQUIRE(event.IsInCategory(moci::EventCategoryInput) == true);
    REQUIRE(event.IsInCategory(moci::EventCategoryMouse) == true);
    REQUIRE_THAT(event.ToString(), Contains("MouseScrolled"));
}

TEST_CASE("moci_events/events: MouseButtonPressedEvent", "[app][events]")
{
    auto const event = moci::MouseButtonPressedEvent(2);
    REQUIRE(event.GetStaticType() == moci::EventType::MouseButtonPressed);
    REQUIRE(event.GetName() == std::string_view("MouseButtonPressed"));
    REQUIRE(event.GetMouseButton() == 2);
    REQUIRE(event.IsInCategory(moci::EventCategoryInput) == true);
    REQUIRE(event.IsInCategory(moci::EventCategoryMouse) == true);
    REQUIRE_THAT(event.ToString(), Contains("MouseButtonPressed"));
}

TEST_CASE("moci_events/events: MouseButtonReleasedEvent", "[app][events]")
{
    auto const event = moci::MouseButtonReleasedEvent(2);
    REQUIRE(event.GetStaticType() == moci::EventType::MouseButtonReleased);
    REQUIRE(event.GetName() == std::string_view("MouseButtonReleased"));
    REQUIRE(event.GetMouseButton() == 2);
    REQUIRE(event.IsInCategory(moci::EventCategoryInput) == true);
    REQUIRE(event.IsInCategory(moci::EventCategoryMouse) == true);
    REQUIRE_THAT(event.ToString(), Contains("MouseButtonReleased"));
}