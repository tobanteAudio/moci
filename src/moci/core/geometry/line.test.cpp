/**
 * @file line.test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include "moci/core/geometry/line.hpp"

TEST_CASE("math/geometry: LineConstructDefault", "[geometry]")
{
    constexpr auto line = moci::Line<float>();
    REQUIRE(line.isEmpty() == true);

    REQUIRE(line.getMidPoint().getX() == 0.0F);
    REQUIRE(line.getMidPoint().getY() == 0.0F);

    REQUIRE(line.getStart().getX() == 0.0F);
    REQUIRE(line.getStart().getY() == 0.0F);

    REQUIRE(line.getEnd().getX() == 0.0F);
    REQUIRE(line.getEnd().getY() == 0.0F);
}

TEST_CASE("math/geometry: LineConstructFromPoints", "[geometry]")
{
    constexpr auto line = moci::Line<float>({0.0F, 0.0F}, {1.0F, 0.0F});
    REQUIRE(line.isEmpty() == false);

    REQUIRE(line.getMidPoint().getX() == 0.5F);
    REQUIRE(line.getMidPoint().getY() == 0.0F);

    REQUIRE(line.getStart().getX() == 0.0F);
    REQUIRE(line.getStart().getY() == 0.0F);

    REQUIRE(line.getEnd().getX() == 1.0F);
    REQUIRE(line.getEnd().getY() == 0.0F);
}

TEST_CASE("math/geometry: LineGetMidPoint", "[geometry]")
{
    constexpr auto line = moci::Line<float>({-1.0F, 2.0F}, {3.0F, -6.0F});

    REQUIRE(line.getMidPoint().getX() == 1.0F);
    REQUIRE(line.getMidPoint().getY() == -2.0F);
}

TEST_CASE("math/geometry: LineGetSlope", "[geometry]")
{
    // empty
    constexpr auto l0 = moci::Line<float>();
    REQUIRE(l0.getSlope().has_value() == false);

    constexpr auto l1 = moci::Line<float>({0.0F, 0.0F}, {1.0F, 0.0F});
    REQUIRE(l1.getSlope().has_value() == true);
    REQUIRE(l1.getSlope().value() == 0.0F);

    constexpr auto l2 = moci::Line<float>({0.0F, 0.0F}, {1.0F, 1.0F});
    REQUIRE(l2.getSlope().has_value() == true);
    REQUIRE(l2.getSlope().value() == 1.0F);

    constexpr auto l3 = moci::Line<float>({0.0F, 0.0F}, {-0.5F, -0.5F});
    REQUIRE(l3.getSlope().has_value() == true);
    REQUIRE(l3.getSlope().value() == 1.0F);

    constexpr auto l4 = moci::Line<float>({0.0F, 0.0F}, {0.5F, -0.5F});
    REQUIRE(l4.getSlope().has_value() == true);
    REQUIRE(l4.getSlope().value() == -1.0F);

    // slope would be inf
    constexpr auto l5 = moci::Line<float>({0.0F, 0.0F}, {0.0F, 1.0F});
    REQUIRE(l5.getSlope().has_value() == false);
}

TEST_CASE("math/geometry: LineGetAngleRadians", "[geometry]")
{
    constexpr auto l1 = moci::Line<int> {};
    REQUIRE(l1.getAngleRadians() == 0);

    constexpr auto l2 = moci::Line<float> {{0.0F, 0.0F}, {1.0F, 1.0F}};
    REQUIRE(l2.getAngleRadians() == Catch::Approx(0.7854F));

    constexpr auto l3 = moci::Line<float> {{0.0F, 0.0F}, {1.0F, -1.0F}};
    REQUIRE(l3.getAngleRadians() == Catch::Approx(-0.7854F));
}

TEST_CASE("math/geometry: LineGetAngleDegrees", "[geometry]")
{
    constexpr auto l1 = moci::Line<int> {};
    REQUIRE(l1.getAngleDegrees() == 0);

    constexpr auto l2 = moci::Line<float> {{0.0F, 0.0F}, {1.0F, 1.0F}};
    REQUIRE(l2.getAngleDegrees() == Catch::Approx(45.0F));

    constexpr auto l3 = moci::Line<float> {{0.0F, 0.0F}, {1.0F, -1.0F}};
    REQUIRE(l3.getAngleDegrees() == Catch::Approx(-45.0F));
}

TEST_CASE("math/geometry: LineGetLength", "[geometry]")
{
    constexpr auto l1 = moci::Line<int> {};
    REQUIRE(l1.getLength() == 0);

    constexpr auto l2 = moci::Line<float> {{0.0F, 0.0F}, {1.0F, 0.0F}};
    REQUIRE(l2.getLength() == Catch::Approx(1.0F));

    constexpr auto l3 = moci::Line<float> {{0.0F, 0.0F}, {0.0F, -1.0F}};
    REQUIRE(l3.getLength() == Catch::Approx(1.0F));

    constexpr auto l4 = moci::Line<float> {{0.0F, 0.0F}, {10.0F, 0.0F}};
    REQUIRE(l4.getLength() == Catch::Approx(10.0F));
}

TEST_CASE("math/geometry: LineOperatorEqual", "[geometry]")
{
    {
        constexpr auto l1 = moci::Line<int> {};
        constexpr auto l2 = moci::Line<int> {};
        REQUIRE(l1 == l2);
    }
    {
        constexpr auto l1 = moci::Line<int> {};
        constexpr auto l2 = moci::Line<int>({1, 2}, {1, 1});
        REQUIRE((l1 == l2) == false);
    }
}

TEST_CASE("math/geometry: LineOperatorNotEqual", "[geometry]")
{
    {
        constexpr auto l1 = moci::Line<int> {};
        constexpr auto l2 = moci::Line<int> {};
        REQUIRE((l1 != l2) == false);
    }
    {
        constexpr auto l1 = moci::Line<int> {};
        constexpr auto l2 = moci::Line<int>({1, 2}, {1, 1});
        REQUIRE(l1 != l2);
    }
}