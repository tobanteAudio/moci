/**
 * @file line_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include "moci/math/geometry/line.hpp"

TEST_CASE("math/geometry: LineConstructDefault", "[geometry]")
{
    constexpr auto line = moci::Line<float>();
    REQUIRE(line.IsEmpty() == true);

    REQUIRE(line.GetMidPoint().GetX() == 0.0F);
    REQUIRE(line.GetMidPoint().GetY() == 0.0F);

    REQUIRE(line.GetStart().GetX() == 0.0F);
    REQUIRE(line.GetStart().GetY() == 0.0F);

    REQUIRE(line.GetEnd().GetX() == 0.0F);
    REQUIRE(line.GetEnd().GetY() == 0.0F);
}

TEST_CASE("math/geometry: LineConstructFromPoints", "[geometry]")
{
    constexpr auto line = moci::Line<float>({0.0F, 0.0F}, {1.0F, 0.0F});
    REQUIRE(line.IsEmpty() == false);

    REQUIRE(line.GetMidPoint().GetX() == 0.5F);
    REQUIRE(line.GetMidPoint().GetY() == 0.0F);

    REQUIRE(line.GetStart().GetX() == 0.0F);
    REQUIRE(line.GetStart().GetY() == 0.0F);

    REQUIRE(line.GetEnd().GetX() == 1.0F);
    REQUIRE(line.GetEnd().GetY() == 0.0F);
}

TEST_CASE("math/geometry: LineGetMidPoint", "[geometry]")
{
    constexpr auto line = moci::Line<float>({-1.0F, 2.0F}, {3.0F, -6.0F});

    REQUIRE(line.GetMidPoint().GetX() == 1.0F);
    REQUIRE(line.GetMidPoint().GetY() == -2.0F);
}

TEST_CASE("math/geometry: LineGetSlope", "[geometry]")
{
    // empty
    constexpr auto l0 = moci::Line<float>();
    REQUIRE(l0.GetSlope().has_value() == false);

    constexpr auto l1 = moci::Line<float>({0.0F, 0.0F}, {1.0F, 0.0F});
    REQUIRE(l1.GetSlope().has_value() == true);
    REQUIRE(l1.GetSlope().value() == 0.0F);

    constexpr auto l2 = moci::Line<float>({0.0F, 0.0F}, {1.0F, 1.0F});
    REQUIRE(l2.GetSlope().has_value() == true);
    REQUIRE(l2.GetSlope().value() == 1.0F);

    constexpr auto l3 = moci::Line<float>({0.0F, 0.0F}, {-0.5F, -0.5F});
    REQUIRE(l3.GetSlope().has_value() == true);
    REQUIRE(l3.GetSlope().value() == 1.0F);

    constexpr auto l4 = moci::Line<float>({0.0F, 0.0F}, {0.5F, -0.5F});
    REQUIRE(l4.GetSlope().has_value() == true);
    REQUIRE(l4.GetSlope().value() == -1.0F);

    // slope would be inf
    constexpr auto l5 = moci::Line<float>({0.0F, 0.0F}, {0.0F, 1.0F});
    REQUIRE(l5.GetSlope().has_value() == false);
}

TEST_CASE("math/geometry: LineGetAngleRadians", "[geometry]")
{
    constexpr auto l1 = moci::Line<int> {};
    REQUIRE(l1.GetAngleRadians() == 0);

    constexpr auto l2 = moci::Line<float> {{0.0F, 0.0F}, {1.0F, 1.0F}};
    REQUIRE(l2.GetAngleRadians() == Catch::Approx(0.7854F));

    constexpr auto l3 = moci::Line<float> {{0.0F, 0.0F}, {1.0F, -1.0F}};
    REQUIRE(l3.GetAngleRadians() == Catch::Approx(-0.7854F));
}

TEST_CASE("math/geometry: LineGetAngleDegrees", "[geometry]")
{
    constexpr auto l1 = moci::Line<int> {};
    REQUIRE(l1.GetAngleDegrees() == 0);

    constexpr auto l2 = moci::Line<float> {{0.0F, 0.0F}, {1.0F, 1.0F}};
    REQUIRE(l2.GetAngleDegrees() == Catch::Approx(45.0F));

    constexpr auto l3 = moci::Line<float> {{0.0F, 0.0F}, {1.0F, -1.0F}};
    REQUIRE(l3.GetAngleDegrees() == Catch::Approx(-45.0F));
}

TEST_CASE("math/geometry: LineGetLength", "[geometry]")
{
    constexpr auto l1 = moci::Line<int> {};
    REQUIRE(l1.GetLength() == 0);

    constexpr auto l2 = moci::Line<float> {{0.0F, 0.0F}, {1.0F, 0.0F}};
    REQUIRE(l2.GetLength() == Catch::Approx(1.0F));

    constexpr auto l3 = moci::Line<float> {{0.0F, 0.0F}, {0.0F, -1.0F}};
    REQUIRE(l3.GetLength() == Catch::Approx(1.0F));

    constexpr auto l4 = moci::Line<float> {{0.0F, 0.0F}, {10.0F, 0.0F}};
    REQUIRE(l4.GetLength() == Catch::Approx(10.0F));
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