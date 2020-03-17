#pragma once

#if defined(MOCI_WINDOWS) || defined(MOCI_MAC) || defined(MOCI_LINUX)

#include <cstdlib>

#include <memory>

extern moci::Application* moci::CreateApplication();

int main(int argc, char** argv)
{
    moci::Log::Init();
    MOCI_CORE_INFO("Initialized Log!");

    auto app = std::unique_ptr<moci::Application>(moci::CreateApplication());
    app->Run();

    return EXIT_SUCCESS;
}

#endif