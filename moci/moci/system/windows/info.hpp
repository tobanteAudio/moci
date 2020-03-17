#pragma once
#include "moci/core/preprocessor.hpp"

#if defined(MOCI_WINDOWS)

#include "moci/system/info.hpp"

#include <string>
#include <unordered_map>

namespace moci
{

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