/**
 * @file application_event_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>

#include "moci/events/types/application_event.hpp"
#include "moci/events/types/event.hpp"
#include "moci/events/types/key_event.hpp"
#include "moci/events/types/mouse_event.hpp"

// using Catch::Matchers::Contains;

TEST_CASE("events/types: WindowResizeEvent", "[types][events]")
{
    auto const event = moci::WindowResizeEvent(10, 20);
    REQUIRE(event.GetStaticType() == moci::EventType::WindowResize);
    REQUIRE(event.GetName() == std::string_view("WindowResize"));
    REQUIRE(event.getWidth() == 10);
    REQUIRE(event.getHeight() == 20);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryApplication) == true);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryInput) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryKeyboard) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    // REQUIRE_THAT(event.ToString(), Contains("WindowResize"));
}

TEST_CASE("events/types: WindowCloseEvent", "[types][events]")
{
    auto const event = moci::WindowCloseEvent();
    REQUIRE(event.GetStaticType() == moci::EventType::WindowClose);
    REQUIRE(event.GetName() == std::string_view("WindowClose"));
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryApplication) == true);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryInput) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryKeyboard) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    // REQUIRE_THAT(event.ToString(), Contains("WindowClose"));
}

TEST_CASE("events/types: AppRenderEvent", "[types][events]")
{
    auto const event = moci::AppRenderEvent();
    REQUIRE(event.GetStaticType() == moci::EventType::AppRender);
    REQUIRE(event.GetName() == std::string_view("AppRender"));
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryApplication) == true);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryInput) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryKeyboard) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    // REQUIRE_THAT(event.ToString(), Contains("AppRender"));
}

TEST_CASE("events/types: AppUpdateEvent", "[types][events]")
{
    auto const event = moci::AppUpdateEvent();
    REQUIRE(event.GetStaticType() == moci::EventType::AppUpdate);
    REQUIRE(event.GetName() == std::string_view("AppUpdate"));
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryApplication) == true);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryInput) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryKeyboard) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    // REQUIRE_THAT(event.ToString(), Contains("AppUpdate"));
}

TEST_CASE("events/types: AppTickEvent", "[types][events]")
{
    auto const event = moci::AppTickEvent();
    REQUIRE(event.GetStaticType() == moci::EventType::AppTick);
    REQUIRE(event.GetName() == std::string_view("AppTick"));
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryApplication) == true);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryInput) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryKeyboard) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouse) == false);
    REQUIRE(event.isInCategory(moci::EventCategory::EventCategoryMouseButton) == false);
    // REQUIRE_THAT(event.ToString(), Contains("AppTick"));
}