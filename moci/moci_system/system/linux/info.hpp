#pragma once
#include "moci_core/core/preprocessor.hpp"

#if defined(MOCI_LINUX)

#include "moci_system/system/info.hpp"

#include <string>
#include <unordered_map>

namespace moci
{
auto SystemInfoLinuxReadProcInfo() -> std::unordered_map<std::string, std::string>;

class SystemInfo::Pimpl
{
public:
    static auto GetOSName() -> std::string;
    static auto GetVendor() -> std::string;
    static auto GetCPUModel() -> std::string;
    static auto GetCPUCoreCount() -> int;
    static auto GetCPUThreadCount() -> int;
    static auto GetCPUFeatures() -> std::string;
};

}  // namespace moci

#endif