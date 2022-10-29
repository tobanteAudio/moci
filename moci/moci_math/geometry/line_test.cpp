/**
 * @file line_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include <catch2/catch_approx.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include "moci_math/geometry/line.hpp"

TEST_CASE("moci_math/geometry: LineConstructDefault", "[geometry]")
{
    constexpr auto line = moci::Line<float>();
    REQUIRE(line.IsEmpty() == true);

    REQUIRE(line.GetMidPoint().GetX() == 0.0f);
    REQUIRE(line.GetMidPoint().GetY() == 0.0f);

    REQUIRE(line.GetStart().GetX() == 0.0f);
    REQUIRE(line.GetStart().GetY() == 0.0f);

    REQUIRE(line.GetEnd().GetX() == 0.0f);
    REQUIRE(line.GetEnd().GetY() == 0.0f);
}

TEST_CASE("moci_math/geometry: LineConstructFromPoints", "[geometry]")
{
    constexpr auto line = moci::Line<float>({0.0f, 0.0f}, {1.0f, 0.0f});
    REQUIRE(line.IsEmpty() == false);

    REQUIRE(line.GetMidPoint().GetX() == 0.5f);
    REQUIRE(line.GetMidPoint().GetY() == 0.0f);

    REQUIRE(line.GetStart().GetX() == 0.0f);
    REQUIRE(line.GetStart().GetY() == 0.0f);

    REQUIRE(line.GetEnd().GetX() == 1.0f);
    REQUIRE(line.GetEnd().GetY() == 0.0f);
}

TEST_CASE("moci_math/geometry: LineGetMidPoint", "[geometry]")
{
    constexpr auto line = moci::Line<float>({-1.0f, 2.0f}, {3.0f, -6.0f});

    REQUIRE(line.GetMidPoint().GetX() == 1.0f);
    REQUIRE(line.GetMidPoint().GetY() == -2.0f);
}

TEST_CASE("moci_math/geometry: LineGetSlope", "[geometry]")
{
    // empty
    constexpr auto l0 = moci::Line<float>();
    REQUIRE(l0.GetSlope().has_value() == false);

    constexpr auto l1 = moci::Line<float>({0.0f, 0.0f}, {1.0f, 0.0f});
    REQUIRE(l1.GetSlope().has_value() == true);
    REQUIRE(l1.GetSlope().value() == 0.0f);

    constexpr auto l2 = moci::Line<float>({0.0f, 0.0f}, {1.0f, 1.0f});
    REQUIRE(l2.GetSlope().has_value() == true);
    REQUIRE(l2.GetSlope().value() == 1.0f);

    constexpr auto l3 = moci::Line<float>({0.0f, 0.0f}, {-0.5f, -0.5f});
    REQUIRE(l3.GetSlope().has_value() == true);
    REQUIRE(l3.GetSlope().value() == 1.0f);

    constexpr auto l4 = moci::Line<float>({0.0f, 0.0f}, {0.5f, -0.5f});
    REQUIRE(l4.GetSlope().has_value() == true);
    REQUIRE(l4.GetSlope().value() == -1.0f);

    // slope would be inf
    constexpr auto l5 = moci::Line<float>({0.0f, 0.0f}, {0.0f, 1.0f});
    REQUIRE(l5.GetSlope().has_value() == false);
}

TEST_CASE("moci_math/geometry: LineGetAngleRadians", "[geometry]")
{
    constexpr auto l1 = moci::Line<int> {};
    REQUIRE(l1.GetAngleRadians() == 0);

    constexpr auto l2 = moci::Line<float> {{0.0f, 0.0f}, {1.0f, 1.0f}};
    REQUIRE(l2.GetAngleRadians() == Catch::Approx(0.7854f));

    constexpr auto l3 = moci::Line<float> {{0.0f, 0.0f}, {1.0f, -1.0f}};
    REQUIRE(l3.GetAngleRadians() == Catch::Approx(-0.7854f));
}

TEST_CASE("moci_math/geometry: LineGetAngleDegrees", "[geometry]")
{
    constexpr auto l1 = moci::Line<int> {};
    REQUIRE(l1.GetAngleDegrees() == 0);

    constexpr auto l2 = moci::Line<float> {{0.0f, 0.0f}, {1.0f, 1.0f}};
    REQUIRE(l2.GetAngleDegrees() == Catch::Approx(45.0f));

    constexpr auto l3 = moci::Line<float> {{0.0f, 0.0f}, {1.0f, -1.0f}};
    REQUIRE(l3.GetAngleDegrees() == Catch::Approx(-45.0f));
}

TEST_CASE("moci_math/geometry: LineGetLength", "[geometry]")
{
    constexpr auto l1 = moci::Line<int> {};
    REQUIRE(l1.GetLength() == 0);

    constexpr auto l2 = moci::Line<float> {{0.0f, 0.0f}, {1.0f, 0.0f}};
    REQUIRE(l2.GetLength() == Catch::Approx(1.0f));

    constexpr auto l3 = moci::Line<float> {{0.0f, 0.0f}, {0.0f, -1.0f}};
    REQUIRE(l3.GetLength() == Catch::Approx(1.0f));

    constexpr auto l4 = moci::Line<float> {{0.0f, 0.0f}, {10.0f, 0.0f}};
    REQUIRE(l4.GetLength() == Catch::Approx(10.0f));
}

TEST_CASE("moci_math/geometry: LineOperatorEqual", "[geometry]")
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

TEST_CASE("moci_math/geometry: LineOperatorNotEqual", "[geometry]")
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