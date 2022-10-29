/**
 * @file component_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include "moci_widget/component/component.hpp"
#include "moci_widget/style/style.hpp"

TEST_CASE("moci_widget: ComponentDefaultConstruct", "[ui]")
{
    moci::Component c {};
    REQUIRE(c.GetWidth() == 0);
    REQUIRE(c.GetHeight() == 0);
    REQUIRE(c.GetID().empty());
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
    REQUIRE(c1.GetID().empty());
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

TEST_CASE("moci_widget: ComponentContains", "[ui]")
{
    moci::Component c {};
    c.SetBounds({100, 50, 10, 20});
    REQUIRE(c.Contains({0, 0}) == false);
    REQUIRE(c.Contains({110, 49}) == false);
    REQUIRE(c.Contains({110, 50}) == true);
}

TEST_CASE("moci_widget: ComponentFindComponentAt", "[ui]")
{
    auto area   = moci::Rectangle<int> {0, 0, 100, 100};
    auto parent = moci::Component {};
    auto c1     = moci::Component {};
    auto c2     = moci::Component {};
    auto c3     = moci::Component {};

    parent.SetBounds(area);
    parent.AddChild(&c1);
    parent.AddChild(&c2);
    parent.AddChild(&c3);
    c1.SetBounds(area.RemoveFromTop(20));
    c2.SetBounds(area.RemoveFromTop(20));
    c3.SetBounds(area.RemoveFromTop(20));

    REQUIRE(parent.FindComponentAt({0, 0}) == &c1);
    REQUIRE(parent.FindComponentAt({10, 25}) == &c2);
    REQUIRE(parent.FindComponentAt({20, 45}) == &c3);
    REQUIRE(parent.FindComponentAt({0, 99}) == &parent);
    REQUIRE(parent.FindComponentAt({0, 180}) == nullptr);
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