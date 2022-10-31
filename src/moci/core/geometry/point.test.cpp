/**
 * @file point.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_all.hpp>

#include <moci/core/geometry/point.hpp>

TEST_CASE("math/geometry: PointConstructionEmpty", "[geometry]")
{
    auto p = moci::Point<float>();
    REQUIRE(p.getX() == 0.0F);
    REQUIRE(p.getY() == 0.0F);
}

TEST_CASE("math/geometry: PointConstructionWithValues", "[geometry]")
{
    constexpr auto p = moci::Point<float>(1.0F, 2.0F);
    REQUIRE(p.getX() == 1.0F);
    REQUIRE(p.getY() == 2.0F);
}

TEST_CASE("math/geometry: PointCopyConstruction", "[geometry]")
{
    constexpr auto p1 = moci::Point<float>(1.0F, 2.0F);
    moci::Point<float> const p2(p1);
    REQUIRE(p1.getX() == p2.getX());
    REQUIRE(p1.getY() == p2.getY());
}

TEST_CASE("math/geometry: PointCopyAssignment", "[geometry]")
{
    auto p1 = moci::Point<float>(1.0F, 2.0F);
    auto p2 = p1;
    REQUIRE(p1.getX() == p2.getX());
    REQUIRE(p1.getY() == p2.getY());
}

TEST_CASE("math/geometry: PointIsOrigin", "[geometry]")
{
    auto p = moci::Point<int>();
    REQUIRE(p.isOrigin() == true);
    p.setY(20);
    REQUIRE(p.isOrigin() == false);
}

TEST_CASE("math/geometry: PointSetXY", "[geometry]")
{
    auto p = moci::Point<int>();
    REQUIRE(p.getX() == 0);
    REQUIRE(p.getY() == 0);
    p.setX(10);
    p.setY(20);
    REQUIRE(p.getX() == 10);
    REQUIRE(p.getY() == 20);
}

TEST_CASE("math/geometry: PointOperatorEqual", "[geometry]")
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

TEST_CASE("math/geometry: PointOperatorNotEqual", "[geometry]")
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