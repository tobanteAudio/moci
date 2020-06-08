#include "application.hpp"

int main()
{
    th::Application app {};
    if (auto err = app.Startup(); err != EXIT_SUCCESS)
    {
        std::printf("Error in startup: %d\n", err);
    }

    app.Run();
    app.Shutdown();

    return EXIT_SUCCESS;
}