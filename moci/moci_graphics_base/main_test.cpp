/**
 * @file test_main.cpp
 * @copyright Copyright 2019-2020 tobanteAudio.
 */

#define CATCH_CONFIG_RUNNER
#include "catch2/catch.hpp"

#include "moci_core/moci_core.hpp"
#include "moci_graphics_base/moci_graphics_base.hpp"

int main(int argc, char* argv[])
{
    // global setup...
    moci::Log::Init();

    // tests ...
    int result = Catch::Session().run(argc, argv);

    // global clean-up...
    return result;
}
