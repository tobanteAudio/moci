#pragma once

#include "window.hpp"

#include <memory>

namespace mvk
{
class Application
{
public:
    Application();
    void Run();

private:
    std::unique_ptr<Window> window_ = nullptr;
    bool isRunning_                 = true;
};
}  // namespace mvk