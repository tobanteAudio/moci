/**
 * @file point_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci/geometry/point.hpp"

TEST_CASE("moci/geometry: PointConstructionEmpty", "[geometry]")
{
    auto p = moci::Point<float>();
    REQUIRE(p.GetX() == 0.0f);
    REQUIRE(p.GetY() == 0.0f);
}

TEST_CASE("moci/geometry: PointConstructionWithValues", "[geometry]")
{
    constexpr auto p = moci::Point<float>(1.0f, 2.0f);
    REQUIRE(p.GetX() == 1.0f);
    REQUIRE(p.GetY() == 2.0f);
}

TEST_CASE("moci/geometry: PointCopyConstruction", "[geometry]")
{
    constexpr auto p1 = moci::Point<float>(1.0f, 2.0f);
    moci::Point<float> const p2(p1);
    REQUIRE(p1.GetX() == p2.GetX());
    REQUIRE(p1.GetY() == p2.GetY());
}

TEST_CASE("moci/geometry: PointCopyAssignment", "[geometry]")
{
    auto p1 = moci::Point<float>(1.0f, 2.0f);
    auto p2 = p1;
    REQUIRE(p1.GetX() == p2.GetX());
    REQUIRE(p1.GetY() == p2.GetY());
}

TEST_CASE("moci/geometry: PointIsOrigin", "[geometry]")
{
    auto p = moci::Point<int>();
    REQUIRE(p.IsOrigin() == true);
    p.SetY(20);
    REQUIRE(p.IsOrigin() == false);
}

TEST_CASE("moci/geometry: PointSetXY", "[geometry]")
{
    auto p = moci::Point<int>();
    REQUIRE(p.GetX() == 0);
    REQUIRE(p.GetY() == 0);
    p.SetX(10);
    p.SetY(20);
    REQUIRE(p.GetX() == 10);
    REQUIRE(p.GetY() == 20);
}

TEST_CASE("moci/geometry: PointOperatorEqual", "[geometry]")
{
    {
        constexpr auto p1 = moci::Point<int>();
        constexpr auto p2 = moci::Point<int>();
        REQUIRE(p1 == p2);
    }
    {
        constexpr auto p1 = moci::Point<int>();
        constexpr auto p2 = moci::Point<int>(1, 2);
        REQUIRE((p1 == p2) == false);
    }
}

TEST_CASE("moci/geometry: PointOperatorNotEqual", "[geometry]")
{
    {
        constexpr auto p1 = moci::Point<int>();
        constexpr auto p2 = moci::Point<int>();
        REQUIRE((p1 != p2) == false);
    }
    {
        constexpr auto p1 = moci::Point<int>();
        constexpr auto p2 = moci::Point<int>(1, 2);
        REQUIRE(p1 != p2);
    }
}