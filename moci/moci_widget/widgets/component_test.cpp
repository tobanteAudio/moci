/**
 * @file component_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci_widget/widgets/component.hpp"
#include "moci_widget/widgets/style.hpp"

TEST_CASE("moci_widget: ComponentDefaultConstruct", "[ui]")
{
    moci::Component c {};
    REQUIRE(c.GetWidth() == 0);
    REQUIRE(c.GetHeight() == 0);
    REQUIRE(c.GetID() == "");
    REQUIRE(c.GetParent() == nullptr);
    REQUIRE(c.GetChildren().empty());
}

TEST_CASE("moci_widget: ComponentIDConstruct", "[ui]")
{
    moci::Component c {"test"};
    REQUIRE(c.GetWidth() == 0);
    REQUIRE(c.GetHeight() == 0);
    REQUIRE(c.GetID() == "test");
    REQUIRE(c.GetParent() == nullptr);
    REQUIRE(c.GetChildren().empty());
}

TEST_CASE("moci_widget: ComponentSetID", "[ui]")
{
    moci::Component c1 {};
    REQUIRE(c1.GetID() == "");
    c1.SetID("test");
    REQUIRE(c1.GetID() == "test");
}

TEST_CASE("moci_widget: ComponentSetParent", "[ui]")
{
    moci::Component c1 {};
    moci::Component c2 {};
    REQUIRE(c1.GetParent() == nullptr);
    REQUIRE(c2.GetParent() == nullptr);
    c1.SetParent(&c2);
    REQUIRE(c1.GetParent() == &c2);
}

TEST_CASE("moci_widget: ComponentGetRootComponent", "[ui]")
{
    moci::Component c1 {};
    moci::Component c2 {};
    moci::Component c3 {};
    c2.SetParent(&c1);
    c3.SetParent(&c2);
    REQUIRE(c3.GetRootComponent() == &c1);
}

TEST_CASE("moci_widget: ComponentAddChild", "[ui]")
{
    moci::Component c1 {};
    moci::Component c2 {};
    REQUIRE(c1.GetChildren().empty());
    c1.AddChild(&c2);
    REQUIRE(c1.GetChildren().size() == 1);
    REQUIRE(c2.GetParent() == &c1);
}

TEST_CASE("moci_widget: ComponentSetSize", "[ui]")
{
    moci::Component c {};
    c.SetSize(100, 50);
    REQUIRE(c.GetWidth() == 100);
    REQUIRE(c.GetHeight() == 50);
}

TEST_CASE("moci_widget: ComponentSetPositionXY", "[ui]")
{
    moci::Component c {};
    c.SetPosition(100, 50);
    REQUIRE(c.GetX() == 100);
    REQUIRE(c.GetY() == 50);
}

TEST_CASE("moci_widget: ComponentSetPositionPoint", "[ui]")
{
    moci::Component c {};
    c.SetPosition({100, 50});
    REQUIRE(c.GetX() == 100);
    REQUIRE(c.GetY() == 50);
}

TEST_CASE("moci_widget: ComponentSetBounds", "[ui]")
{
    moci::Component c {};
    c.SetBounds({100, 50, 10, 20});
    auto bounds = c.GetBounds();
    REQUIRE(bounds.GetX() == 100);
    REQUIRE(bounds.GetY() == 50);
    REQUIRE(bounds.GetWidth() == 10);
    REQUIRE(bounds.GetHeight() == 20);
}

TEST_CASE("moci_widget: ComponentStyle", "[ui]")
{
    moci::Component c {};

    REQUIRE(c.GetStyle() == nullptr);
    c.SetStyle(nullptr);
    REQUIRE(c.GetStyle() == nullptr);

    moci::Style style {};
    c.SetStyle(&style);
    REQUIRE(c.GetStyle() == &style);

    moci::Component child {};
    c.AddChild(&child);
    REQUIRE(child.GetStyle() == &style);
}