/**
 * @file application_event_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci_events/moci_events.hpp"

using Catch::Matchers::Contains;

TEST_CASE("moci_events/types: WindowResizeEvent", "[types][events]")
{
    auto const event = moci::WindowResizeEvent(10, 20);
    REQUIRE(event.GetStaticType() == moci::EventType::WindowResize);
    REQUIRE(event.GetName() == std::string_view("WindowResize"));
    REQUIRE(event.GetWidth() == 10);
    REQUIRE(event.GetHeight() == 20);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryApplication) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryInput) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryKeyboard) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    REQUIRE_THAT(event.ToString(), Contains("WindowResize"));
}

TEST_CASE("moci_events/types: WindowCloseEvent", "[types][events]")
{
    auto const event = moci::WindowCloseEvent();
    REQUIRE(event.GetStaticType() == moci::EventType::WindowClose);
    REQUIRE(event.GetName() == std::string_view("WindowClose"));
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryApplication) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryInput) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryKeyboard) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    REQUIRE_THAT(event.ToString(), Contains("WindowClose"));
}

TEST_CASE("moci_events/types: AppRenderEvent", "[types][events]")
{
    auto const event = moci::AppRenderEvent();
    REQUIRE(event.GetStaticType() == moci::EventType::AppRender);
    REQUIRE(event.GetName() == std::string_view("AppRender"));
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryApplication) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryInput) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryKeyboard) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    REQUIRE_THAT(event.ToString(), Contains("AppRender"));
}

TEST_CASE("moci_events/types: AppUpdateEvent", "[types][events]")
{
    auto const event = moci::AppUpdateEvent();
    REQUIRE(event.GetStaticType() == moci::EventType::AppUpdate);
    REQUIRE(event.GetName() == std::string_view("AppUpdate"));
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryApplication) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryInput) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryKeyboard) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    REQUIRE_THAT(event.ToString(), Contains("AppUpdate"));
}

TEST_CASE("moci_events/types: AppTickEvent", "[types][events]")
{
    auto const event = moci::AppTickEvent();
    REQUIRE(event.GetStaticType() == moci::EventType::AppTick);
    REQUIRE(event.GetName() == std::string_view("AppTick"));
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryApplication) == true);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryInput) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryKeyboard) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.IsInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    REQUIRE_THAT(event.ToString(), Contains("AppTick"));
}