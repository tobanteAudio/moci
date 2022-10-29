#pragma once
#include "moci/core/preprocessor.hpp"

#if defined(MOCI_LINUX)

#include "moci/system/info.hpp"

#include <string>
#include <unordered_map>

namespace moci
{
auto systemInfoLinuxReadProcInfo() -> std::unordered_map<std::string, std::string>;

class SystemInfo::Pimpl
{
public:
    static auto getOsName() -> std::string;
    static auto getVendor() -> std::string;
    static auto getCpuModel() -> std::string;
    static auto getCpuCoreCount() -> int;
    static auto getCpuThreadCount() -> int;
    static auto getCpuFeatures() -> std::string;
};

}  // namespace moci

#endif