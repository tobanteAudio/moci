/**
 * @file component.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include <moci/widgets/component/component.hpp>
#include <moci/widgets/style/style.hpp>

TEST_CASE("widget: ComponentDefaultConstruct", "[ui]")
{
    moci::Component c {};
    REQUIRE(c.getWidth() == 0);
    REQUIRE(c.getHeight() == 0);
    REQUIRE(c.getId().empty());
    REQUIRE(c.getParent() == nullptr);
    REQUIRE(c.getChildren().empty());
}

TEST_CASE("widget: ComponentIDConstruct", "[ui]")
{
    moci::Component c {"test"};
    REQUIRE(c.getWidth() == 0);
    REQUIRE(c.getHeight() == 0);
    REQUIRE(c.getId() == "test");
    REQUIRE(c.getParent() == nullptr);
    REQUIRE(c.getChildren().empty());
}

TEST_CASE("widget: ComponentSetID", "[ui]")
{
    moci::Component c1 {};
    REQUIRE(c1.getId().empty());
    c1.setId("test");
    REQUIRE(c1.getId() == "test");
}

TEST_CASE("widget: ComponentSetParent", "[ui]")
{
    moci::Component c1 {};
    moci::Component c2 {};
    REQUIRE(c1.getParent() == nullptr);
    REQUIRE(c2.getParent() == nullptr);
    c1.setParent(&c2);
    REQUIRE(c1.getParent() == &c2);
}

TEST_CASE("widget: ComponentGetRootComponent", "[ui]")
{
    moci::Component c1 {};
    moci::Component c2 {};
    moci::Component c3 {};
    c2.setParent(&c1);
    c3.setParent(&c2);
    REQUIRE(c3.getRootComponent() == &c1);
}

TEST_CASE("widget: ComponentAddChild", "[ui]")
{
    moci::Component c1 {};
    moci::Component c2 {};
    REQUIRE(c1.getChildren().empty());
    c1.addChild(&c2);
    REQUIRE(c1.getChildren().size() == 1);
    REQUIRE(c2.getParent() == &c1);
}

TEST_CASE("widget: ComponentSetSize", "[ui]")
{
    moci::Component c {};
    c.setSize(100, 50);
    REQUIRE(c.getWidth() == 100);
    REQUIRE(c.getHeight() == 50);
}

TEST_CASE("widget: ComponentSetPositionXY", "[ui]")
{
    moci::Component c {};
    c.setPosition(100, 50);
    REQUIRE(c.getX() == 100);
    REQUIRE(c.getY() == 50);
}

TEST_CASE("widget: ComponentSetPositionPoint", "[ui]")
{
    moci::Component c {};
    c.setPosition({100, 50});
    REQUIRE(c.getX() == 100);
    REQUIRE(c.getY() == 50);
}

TEST_CASE("widget: ComponentSetBounds", "[ui]")
{
    moci::Component c {};
    c.setBounds({100, 50, 10, 20});
    auto bounds = c.getBounds();
    REQUIRE(bounds.getX() == 100);
    REQUIRE(bounds.getY() == 50);
    REQUIRE(bounds.getWidth() == 10);
    REQUIRE(bounds.getHeight() == 20);
}

TEST_CASE("widget: ComponentContains", "[ui]")
{
    moci::Component c {};
    c.setBounds({100, 50, 10, 20});
    REQUIRE(c.contains({0, 0}) == false);
    REQUIRE(c.contains({110, 49}) == false);
    REQUIRE(c.contains({110, 50}) == true);
}

TEST_CASE("widget: ComponentFindComponentAt", "[ui]")
{
    auto area   = moci::Rectangle<int> {0, 0, 100, 100};
    auto parent = moci::Component {};
    auto c1     = moci::Component {};
    auto c2     = moci::Component {};
    auto c3     = moci::Component {};

    parent.setBounds(area);
    parent.addChild(&c1);
    parent.addChild(&c2);
    parent.addChild(&c3);
    c1.setBounds(area.removeFromTop(20));
    c2.setBounds(area.removeFromTop(20));
    c3.setBounds(area.removeFromTop(20));

    REQUIRE(parent.findComponentAt({0, 0}) == &c1);
    REQUIRE(parent.findComponentAt({10, 25}) == &c2);
    REQUIRE(parent.findComponentAt({20, 45}) == &c3);
    REQUIRE(parent.findComponentAt({0, 99}) == &parent);
    REQUIRE(parent.findComponentAt({0, 180}) == nullptr);
}

TEST_CASE("widget: ComponentStyle", "[ui]")
{
    moci::Component c {};

    REQUIRE(c.getStyle() == nullptr);
    c.setStyle(nullptr);
    REQUIRE(c.getStyle() == nullptr);

    moci::Style style {};
    c.setStyle(&style);
    REQUIRE(c.getStyle() == &style);

    moci::Component child {};
    c.addChild(&child);
    REQUIRE(child.getStyle() == &style);
}
