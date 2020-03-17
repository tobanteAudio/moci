/**
 * @file algorithm_test.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */
#include "catch2/catch.hpp"

#include "moci/core/algorithm.hpp"

TEST_CASE("moci/core: AlgorithmFindInVector", "[core]")
{
    auto v = std::vector<int> {1, 2, 3};

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