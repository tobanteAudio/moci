#include "application.hpp"

namespace mvk
{
Application::Application()
{
    window_ = std::make_unique<Window>();
    window_->Prepare();
}
void Application::Run()
{
    while (!window_->ShouldClose()) { window_->PollEvents(); }
}
}  // namespace mvk