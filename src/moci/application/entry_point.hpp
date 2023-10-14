#pragma once

#if defined(MOCI_WINDOWS) || defined(MOCI_MAC) || defined(MOCI_LINUX)

#include <moci/core/preprocessor.hpp>

#include <cstdlib>
#include <memory>

extern moci::Application* moci::createApplication();

int main(int argc, char** argv)
{
    moci::ignoreUnused(argc);
    moci::ignoreUnused(argv);

    moci::Log::init();
    MOCI_CORE_INFO("Initialized Log!");

    auto app = std::unique_ptr<moci::Application>(moci::createApplication());
    app->run();

    return EXIT_SUCCESS;
}

#endif