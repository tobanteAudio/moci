/**
 * @file test_main.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */

#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>

#include "moci_core/moci_core.hpp"

auto main(int argc, char* argv[]) -> int
{
    // global setup...
    moci::Log::Init();

    // tests ...
    int result = Catch::Session().run(argc, argv);

    // global clean-up...
    return result;
}
