/**
 * @file rectangle_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci/geometry/rectangle.hpp"

TEST_CASE("moci/geometry: RectangleConstructionEmpty", "[geometry]")
{
    moci::Rectangle<float> r {};
    REQUIRE(r.IsEmpty() == true);
    REQUIRE(r.GetX() == 0.0f);
    REQUIRE(r.GetY() == 0.0f);
    REQUIRE(r.GetWidth() == 0.0f);
    REQUIRE(r.GetHeight() == 0.0f);
}

TEST_CASE("moci/geometry: RectangleConstructionWithValues", "[geometry]")
{
    constexpr moci::Rectangle<float> r(1.0f, 2.0f, 10.0f, 10.0f);
    REQUIRE(r.IsEmpty() == false);
    REQUIRE(r.GetX() == 1.0f);
    REQUIRE(r.GetY() == 2.0f);
    REQUIRE(r.GetWidth() == 10.0f);
    REQUIRE(r.GetHeight() == 10.0f);
}

TEST_CASE("moci/geometry: RectangleCopyConstruction", "[geometry]")
{
    auto r = moci::Rectangle<float>(1.0f, 2.0f, 10.0f, 10.0f);
    moci::Rectangle<float> const r2(r);
    REQUIRE(r.GetX() == r2.GetX());
    REQUIRE(r.GetY() == r2.GetY());
    REQUIRE(r.GetHeight() == r2.GetHeight());
    REQUIRE(r.GetWidth() == r2.GetWidth());
}

TEST_CASE("moci/geometry: RectangleCopyAssignment", "[geometry]")
{
    auto r1 = moci::Rectangle<float>(1.0f, 2.0f, 10.0f, 10.0f);
    auto r2 = r1;
    REQUIRE(r1.GetX() == r2.GetX());
    REQUIRE(r1.GetY() == r2.GetY());
}

TEST_CASE("moci/geometry: RectangleSetPositionXY", "[geometry]")
{
    auto r = moci::Rectangle<int> {};
    r.SetPosition(20, 10);
    REQUIRE(r.GetX() == 20);
    REQUIRE(r.GetY() == 10);
}

TEST_CASE("moci/geometry: RectangleSetPositionPoint", "[geometry]")
{
    auto r = moci::Rectangle<int> {};
    r.SetPosition(moci::Point<int>(10, 5));
    REQUIRE(r.GetX() == 10);
    REQUIRE(r.GetY() == 5);
}

TEST_CASE("moci/geometry: RectangleSetWidth", "[geometry]")
{
    auto r = moci::Rectangle<double> {};
    REQUIRE(r.IsEmpty() == true);
    r.SetWidth(20.0);
    REQUIRE(r.IsEmpty() == false);
    REQUIRE(r.GetWidth() == 20.0);
}

TEST_CASE("moci/geometry: RectangleSetHeight", "[geometry]")
{
    auto r = moci::Rectangle<double> {};
    REQUIRE(r.IsEmpty() == true);
    r.SetHeight(20.0);
    REQUIRE(r.IsEmpty() == false);
    REQUIRE(r.GetHeight() == 20.0);
}

TEST_CASE("moci/geometry: RectangleSetXY", "[geometry]")
{
    auto p = moci::Rectangle<int>();
    REQUIRE(p.GetX() == 0);
    REQUIRE(p.GetY() == 0);
    p.SetX(10);
    p.SetY(20);
    REQUIRE(p.GetX() == 10);
    REQUIRE(p.GetY() == 20);
}

TEST_CASE("moci/geometry: RectangleRemoveFromTop", "[geometry]")
{
    auto p = moci::Rectangle<int>(0, 0, 50, 100);
    REQUIRE(p.GetY() == 0);
    REQUIRE(p.GetWidth() == 50);
    REQUIRE(p.GetHeight() == 100);

    auto n = p.RemoveFromTop(10);
    REQUIRE(n.GetY() == 90);
    REQUIRE(n.GetWidth() == 50);
    REQUIRE(n.GetHeight() == 10);

    REQUIRE(p.GetY() == 0);
    REQUIRE(p.GetWidth() == 50);
    REQUIRE(p.GetHeight() == 90);
}

TEST_CASE("moci/geometry: RectangleRemoveFromBottom", "[geometry]")
{
    auto p = moci::Rectangle<int>(0, 0, 50, 100);
    REQUIRE(p.GetY() == 0);
    REQUIRE(p.GetWidth() == 50);
    REQUIRE(p.GetHeight() == 100);

    auto n = p.RemoveFromBottom(10);
    REQUIRE(n.GetY() == 0);
    REQUIRE(n.GetWidth() == 50);
    REQUIRE(n.GetHeight() == 10);

    REQUIRE(p.GetY() == 10);
    REQUIRE(p.GetWidth() == 50);
    REQUIRE(p.GetHeight() == 90);
}

TEST_CASE("moci/geometry: RectangleRemoveFromLeft", "[geometry]")
{
    auto p = moci::Rectangle<int>(0, 0, 50, 100);
    REQUIRE(p.GetX() == 0);
    REQUIRE(p.GetWidth() == 50);
    REQUIRE(p.GetHeight() == 100);

    auto n = p.RemoveFromLeft(10);
    REQUIRE(n.GetX() == 0);
    REQUIRE(n.GetWidth() == 10);
    REQUIRE(n.GetHeight() == 100);

    REQUIRE(p.GetX() == 10);
    REQUIRE(p.GetWidth() == 40);
    REQUIRE(p.GetHeight() == 100);
}

TEST_CASE("moci/geometry: RectangleRemoveFromRight", "[geometry]")
{
    auto p = moci::Rectangle<int>(0, 0, 50, 100);
    REQUIRE(p.GetX() == 0);
    REQUIRE(p.GetWidth() == 50);
    REQUIRE(p.GetHeight() == 100);

    auto n = p.RemoveFromRight(10);
    REQUIRE(n.GetX() == 40);
    REQUIRE(n.GetWidth() == 10);
    REQUIRE(n.GetHeight() == 100);

    REQUIRE(p.GetX() == 0);
    REQUIRE(p.GetWidth() == 40);
    REQUIRE(p.GetHeight() == 100);
}

TEST_CASE("moci/geometry: RectangleReduced", "[geometry]")
{
    auto p = moci::Rectangle<int>(0, 0, 50, 100);
    REQUIRE(p.GetX() == 0);
    REQUIRE(p.GetWidth() == 50);
    REQUIRE(p.GetHeight() == 100);

    auto n = p.Reduced(10);
    REQUIRE(n.GetX() == 10);
    REQUIRE(n.GetWidth() == 30);
    REQUIRE(n.GetHeight() == 80);
}

TEST_CASE("moci/geometry: RectangleReducedXY", "[geometry]")
{
    auto p = moci::Rectangle<int>(0, 0, 50, 100);
    REQUIRE(p.GetX() == 0);
    REQUIRE(p.GetWidth() == 50);
    REQUIRE(p.GetHeight() == 100);

    auto n = p.Reduced(10, 20);
    REQUIRE(n.GetX() == 10);
    REQUIRE(n.GetY() == 20);
    REQUIRE(n.GetWidth() == 30);
    REQUIRE(n.GetHeight() == 60);
}

TEST_CASE("moci/geometry: RectangleTo", "[geometry]")
{
    auto p = moci::Rectangle<double>(0.0, 0.0, 50.5, 100.1);
    auto n = p.To<int>();
    REQUIRE(n.GetX() == 0);
    REQUIRE(n.GetY() == 0);
    REQUIRE(n.GetWidth() == 50);
    REQUIRE(n.GetHeight() == 100);
}

TEST_CASE("moci/geometry: RectangleToInt", "[geometry]")
{
    auto p = moci::Rectangle<double>(0.0, 0.0, 50.5, 100.1);
    auto n = p.ToInt();
    REQUIRE(n.GetX() == 0);
    REQUIRE(n.GetY() == 0);
    REQUIRE(n.GetWidth() == 50);
    REQUIRE(n.GetHeight() == 100);
}

TEST_CASE("moci/geometry: RectangleToFloat", "[geometry]")
{
    auto p = moci::Rectangle<float>(0.0, 0.0, 50.5, 100.1);
    auto n = p.ToFloat();
    REQUIRE(n.GetX() == 0.0f);
    REQUIRE(n.GetY() == 0.0f);
    REQUIRE(n.GetWidth() == 50.5f);
    REQUIRE(n.GetHeight() == 100.1f);
}

TEST_CASE("moci/geometry: RectangleOperatorEqual", "[geometry]")
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

TEST_CASE("moci/geometry: RectangleOperatorNotEqual", "[geometry]")
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