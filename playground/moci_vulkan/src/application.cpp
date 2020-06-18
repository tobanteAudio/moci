#include "application.hpp"

namespace mvk
{
Application::Application() { window_ = std::make_unique<Window>(); }
void Application::Run()
{
    while (isRunning_) { ; }
}
}  // namespace mvk