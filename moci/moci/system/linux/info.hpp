#pragma once
#include "moci_core/core/preprocessor.hpp"

#if defined(MOCI_LINUX)

#include "moci/system/info.hpp"

#include <string>
#include <unordered_map>

namespace moci
{
std::unordered_map<std::string, std::string> SystemInfoLinuxReadProcInfo();

class SystemInfo::Pimpl
{
public:
    static std::string GetOSName();
    static std::string GetVendor();
    static std::string GetCPUModel();
    static int GetCPUCoreCount();
    static int GetCPUThreadCount();
    static std::string GetCPUFeatures();
};

}  // namespace moci

#endif