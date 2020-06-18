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
    std::unique_ptr<Window> window_;
};
}  // namespace mvk