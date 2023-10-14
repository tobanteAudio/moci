/**
 * @file rectangle.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <moci/core/geometry/rectangle.hpp>

TEST_CASE("math/geometry: RectangleConstructionEmpty", "[geometry]")
{
    moci::Rectangle<float> r {};
    REQUIRE(r.isEmpty() == true);
    REQUIRE(r.getX() == 0.0F);
    REQUIRE(r.getY() == 0.0F);
    REQUIRE(r.getWidth() == 0.0F);
    REQUIRE(r.getHeight() == 0.0F);
}

TEST_CASE("math/geometry: RectangleConstructionWithValues", "[geometry]")
{
    constexpr moci::Rectangle<float> r(1.0F, 2.0F, 10.0F, 10.0F);
    REQUIRE(r.isEmpty() == false);
    REQUIRE(r.getX() == 1.0F);
    REQUIRE(r.getY() == 2.0F);
    REQUIRE(r.getWidth() == 10.0F);
    REQUIRE(r.getHeight() == 10.0F);
}

TEST_CASE("math/geometry: RectangleCopyConstruction", "[geometry]")
{
    auto r = moci::Rectangle<float>(1.0F, 2.0F, 10.0F, 10.0F);
    moci::Rectangle<float> const r2(r);
    REQUIRE(r.getX() == r2.getX());
    REQUIRE(r.getY() == r2.getY());
    REQUIRE(r.getHeight() == r2.getHeight());
    REQUIRE(r.getWidth() == r2.getWidth());
}

TEST_CASE("math/geometry: RectangleCopyAssignment", "[geometry]")
{
    auto r1 = moci::Rectangle<float>(1.0F, 2.0F, 10.0F, 10.0F);
    auto r2 = r1;
    REQUIRE(r1.getX() == r2.getX());
    REQUIRE(r1.getY() == r2.getY());
}

TEST_CASE("math/geometry: RectangleSetPositionXY", "[geometry]")
{
    auto r = moci::Rectangle<int> {};
    r.setPosition(20, 10);
    REQUIRE(r.getX() == 20);
    REQUIRE(r.getY() == 10);
}

TEST_CASE("math/geometry: RectangleSetPositionPoint", "[geometry]")
{
    auto r = moci::Rectangle<int> {};
    r.setPosition(moci::Point<int>(10, 5));
    REQUIRE(r.getX() == 10);
    REQUIRE(r.getY() == 5);
}

TEST_CASE("math/geometry: RectangleSetWidth", "[geometry]")
{
    auto r = moci::Rectangle<double> {};
    REQUIRE(r.isEmpty() == true);
    r.setWidth(20.0);
    REQUIRE(r.isEmpty() == false);
    REQUIRE(r.getWidth() == 20.0);
}

TEST_CASE("math/geometry: RectangleSetHeight", "[geometry]")
{
    auto r = moci::Rectangle<double> {};
    REQUIRE(r.isEmpty() == true);
    r.setHeight(20.0);
    REQUIRE(r.isEmpty() == false);
    REQUIRE(r.getHeight() == 20.0);
}

TEST_CASE("math/geometry: RectangleSetXY", "[geometry]")
{
    auto p = moci::Rectangle<int>();
    REQUIRE(p.getX() == 0);
    REQUIRE(p.getY() == 0);
    p.setX(10);
    p.setY(20);
    REQUIRE(p.getX() == 10);
    REQUIRE(p.getY() == 20);
}

TEST_CASE("math/geometry: RectangleGetRight", "[geometry]")
{
    auto empty = moci::Rectangle<int> {};
    REQUIRE(empty.getRight() == 0);

    auto square = moci::Rectangle<int> {0, 0, 100, 100};
    REQUIRE(square.getRight() == 100);
}

TEST_CASE("math/geometry: RectangleGetBottom", "[geometry]")
{
    auto empty = moci::Rectangle<int> {};
    REQUIRE(empty.getBottom() == 0);

    auto square = moci::Rectangle<int> {0, 0, 100, 100};
    REQUIRE(square.getBottom() == 100);
}

TEST_CASE("math/geometry: RectangleGetAspectRatio", "[geometry]")
{
    auto empty = moci::Rectangle<int> {};
    REQUIRE(empty.getAspectRatio() == 0);

    auto square = moci::Rectangle<int> {0, 0, 100, 100};
    REQUIRE(square.getAspectRatio() == 1);

    auto display = moci::Rectangle<float> {0.0F, 0.0F, 1920.0F, 1080.0F};
    REQUIRE(display.getAspectRatio() == Catch::Approx(16.0F / 9.0F));
}

TEST_CASE("math/geometry: RectangleRemoveFromTop", "[geometry]")
{
    auto p = moci::Rectangle<int>(0, 0, 50, 100);
    REQUIRE(p.getY() == 0);
    REQUIRE(p.getWidth() == 50);
    REQUIRE(p.getHeight() == 100);

    auto n = p.removeFromTop(10);
    REQUIRE(n.getY() == 0);
    REQUIRE(n.getWidth() == 50);
    REQUIRE(n.getHeight() == 10);

    REQUIRE(p.getY() == 10);
    REQUIRE(p.getWidth() == 50);
    REQUIRE(p.getHeight() == 90);
}

TEST_CASE("math/geometry: RectangleRemoveFromBottom", "[geometry]")
{
    auto p = moci::Rectangle<int>(0, 0, 50, 100);

    auto n = p.removeFromBottom(10);
    REQUIRE(n.getY() == 90);
    REQUIRE(n.getWidth() == 50);
    REQUIRE(n.getHeight() == 10);

    REQUIRE(p.getY() == 0);
    REQUIRE(p.getWidth() == 50);
    REQUIRE(p.getHeight() == 90);
}

TEST_CASE("math/geometry: RectangleRemoveFromLeft", "[geometry]")
{
    auto p = moci::Rectangle<int>(0, 0, 50, 100);

    auto n = p.removeFromLeft(10);
    REQUIRE(n.getX() == 0);
    REQUIRE(n.getY() == 0);
    REQUIRE(n.getWidth() == 10);
    REQUIRE(n.getHeight() == 100);

    REQUIRE(p.getX() == 10);
    REQUIRE(p.getY() == 0);
    REQUIRE(p.getWidth() == 40);
    REQUIRE(p.getHeight() == 100);
}

TEST_CASE("math/geometry: RectangleRemoveFromRight", "[geometry]")
{
    auto p = moci::Rectangle<int>(0, 0, 50, 100);
    REQUIRE(p.getX() == 0);
    REQUIRE(p.getWidth() == 50);
    REQUIRE(p.getHeight() == 100);

    auto n = p.removeFromRight(10);
    REQUIRE(n.getX() == 40);
    REQUIRE(n.getWidth() == 10);
    REQUIRE(n.getHeight() == 100);

    REQUIRE(p.getX() == 0);
    REQUIRE(p.getWidth() == 40);
    REQUIRE(p.getHeight() == 100);
}

TEST_CASE("math/geometry: RectangleReduced", "[geometry]")
{
    auto p = moci::Rectangle<int>(0, 0, 50, 100);
    REQUIRE(p.getX() == 0);
    REQUIRE(p.getWidth() == 50);
    REQUIRE(p.getHeight() == 100);

    auto n = p.reduced(10);
    REQUIRE(n.getX() == 10);
    REQUIRE(n.getWidth() == 30);
    REQUIRE(n.getHeight() == 80);
}

TEST_CASE("math/geometry: RectangleReducedXY", "[geometry]")
{
    auto p = moci::Rectangle<int>(0, 0, 50, 100);
    REQUIRE(p.getX() == 0);
    REQUIRE(p.getWidth() == 50);
    REQUIRE(p.getHeight() == 100);

    auto n = p.reduced(10, 20);
    REQUIRE(n.getX() == 10);
    REQUIRE(n.getY() == 20);
    REQUIRE(n.getWidth() == 30);
    REQUIRE(n.getHeight() == 60);
}

TEST_CASE("math/geometry: RectangleTo", "[geometry]")
{
    auto p = moci::Rectangle<double>(0.0, 0.0, 50.5, 100.1);
    auto n = p.to<int>();
    REQUIRE(n.getX() == 0);
    REQUIRE(n.getY() == 0);
    REQUIRE(n.getWidth() == 50);
    REQUIRE(n.getHeight() == 100);
}

TEST_CASE("math/geometry: RectangleToInt", "[geometry]")
{
    auto p = moci::Rectangle<double>(0.0, 0.0, 50.5, 100.1);
    auto n = p.toInt();
    REQUIRE(n.getX() == 0);
    REQUIRE(n.getY() == 0);
    REQUIRE(n.getWidth() == 50);
    REQUIRE(n.getHeight() == 100);
}

TEST_CASE("math/geometry: RectangleToFloat", "[geometry]")
{
    auto p = moci::Rectangle<float>(0.0F, 0.0F, 50.5F, 100.1F);
    auto n = p.toFloat();
    REQUIRE(n.getX() == 0.0F);
    REQUIRE(n.getY() == 0.0F);
    REQUIRE(n.getWidth() == 50.5F);
    REQUIRE(n.getHeight() == 100.1F);
}

TEST_CASE("math/geometry: RectangleOperatorEqual", "[geometry]")
{
    {
        constexpr auto r1 = moci::Rectangle<int> {};
        constexpr auto r2 = moci::Rectangle<int> {};
        REQUIRE(r1 == r2);
    }
    {
        constexpr auto r1 = moci::Rectangle<int> {};
        constexpr auto r2 = moci::Rectangle<int>({1, 2}, 1, 1);
        REQUIRE((r1 == r2) == false);
    }
}

TEST_CASE("math/geometry: RectangleOperatorNotEqual", "[geometry]")
{
    {
        constexpr auto r1 = moci::Rectangle<int> {};
        constexpr auto r2 = moci::Rectangle<int> {};
        REQUIRE((r1 != r2) == false);
    }
    {
        constexpr auto r1 = moci::Rectangle<int> {};
        constexpr auto r2 = moci::Rectangle<int>({1, 2}, 1, 1);
        REQUIRE(r1 != r2);
    }
}
