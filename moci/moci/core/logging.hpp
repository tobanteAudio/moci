#pragma once

#include "moci/core/preprocessor.hpp"
#include "moci/core/memory.hpp"

#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

namespace moci
{

class Log
{
public:
    static void Init();

    inline static auto GetCoreLogger() -> Ref<spdlog::logger>& { return s_CoreLogger; }
    inline static auto GetClientLogger() -> Ref<spdlog::logger>& { return s_ClientLogger; }

private:
    static Ref<spdlog::logger> s_CoreLogger;
    static Ref<spdlog::logger> s_ClientLogger;
};

}  // namespace moci

// Core log macros
#define MOCI_CORE_TRACE(...) ::moci::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MOCI_CORE_INFO(...) ::moci::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MOCI_CORE_WARN(...) ::moci::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MOCI_CORE_ERROR(...) ::moci::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MOCI_CORE_CRITICAL(...) ::moci::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define MOCI_TRACE(...) ::moci::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MOCI_INFO(...) ::moci::Log::GetClientLogger()->info(__VA_ARGS__)
#define MOCI_WARN(...) ::moci::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MOCI_ERROR(...) ::moci::Log::GetClientLogger()->error(__VA_ARGS__)
#define MOCI_CRITICAL(...) ::moci::Log::GetClientLogger()->critical(__VA_ARGS__)