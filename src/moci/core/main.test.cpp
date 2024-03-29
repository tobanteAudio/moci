/**
 * @file test_main.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */

#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>

#include <moci/core/logging.hpp>

auto main(int argc, char* argv[]) -> int
{
    // global setup...
    moci::Log::init();

    // tests ...
    int const result = Catch::Session().run(argc, argv);

    // global clean-up...
    return result;
}
