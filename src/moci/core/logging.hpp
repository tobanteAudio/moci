#pragma once

#include <moci/core/preprocessor.hpp>

#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

#include <memory>

namespace moci {

class Log
{
public:
    static void init();

    inline static auto getCoreLogger() -> std::shared_ptr<spdlog::logger>&
    {
        return sCoreLogger;
    }

    inline static auto getClientLogger() -> std::shared_ptr<spdlog::logger>&
    {
        return sClientLogger;
    }

private:
    static std::shared_ptr<spdlog::logger> sCoreLogger;
    static std::shared_ptr<spdlog::logger> sClientLogger;
};

}  // namespace moci

// Core log macros
#define MOCI_CORE_TRACE(...)    ::moci::Log::getCoreLogger()->trace(__VA_ARGS__)
#define MOCI_CORE_INFO(...)     ::moci::Log::getCoreLogger()->info(__VA_ARGS__)
#define MOCI_CORE_WARN(...)     ::moci::Log::getCoreLogger()->warn(__VA_ARGS__)
#define MOCI_CORE_ERROR(...)    ::moci::Log::getCoreLogger()->error(__VA_ARGS__)
#define MOCI_CORE_CRITICAL(...) ::moci::Log::getCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define MOCI_TRACE(...)    ::moci::Log::getClientLogger()->trace(__VA_ARGS__)
#define MOCI_INFO(...)     ::moci::Log::getClientLogger()->info(__VA_ARGS__)
#define MOCI_WARN(...)     ::moci::Log::getClientLogger()->warn(__VA_ARGS__)
#define MOCI_ERROR(...)    ::moci::Log::getClientLogger()->error(__VA_ARGS__)
#define MOCI_CRITICAL(...) ::moci::Log::getClientLogger()->critical(__VA_ARGS__)
