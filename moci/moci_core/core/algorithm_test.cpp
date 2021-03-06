/**
 * @file algorithm_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci_core/core/algorithm.hpp"

TEST_CASE("moci_core: AlgorithmFindInVector", "[core]")
{
    auto v = moci::Vector<int> {1, 2, 3};

    {
        auto result = moci::FindInVector(v, 0);
        REQUIRE(result.first == false);
        REQUIRE(result.second == -1);
    }

    {
        auto result = moci::FindInVector(v, 1);
        REQUIRE(result.first == true);
        REQUIRE(result.second == 0);
    }
}