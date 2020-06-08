#include "moci_system/system/windows/info.hpp"

#if defined(MOCI_WINDOWS)

#include <windows.h>

namespace moci
{
std::string SystemInfo::Pimpl::GetOSName() { return "Windows"; }

std::string SystemInfo::Pimpl::GetVendor()
{
    // auto const procInfo = SystemInfoLinuxReadProcInfo();
    // auto const vendorID = procInfo.find(std::string("vendor_id"));
    // if (vendorID != std::end(procInfo))
    // {
    //     return vendorID->second;
    // }
    return "";
}

std::string SystemInfo::Pimpl::GetCPUModel()
{
    // auto const procInfo = SystemInfoLinuxReadProcInfo();
    // auto const vendorID = procInfo.find(std::string("model name"));
    // if (vendorID != std::end(procInfo))
    // {
    //     return vendorID->second;
    // }
    return "";
}

int SystemInfo::Pimpl::GetCPUCoreCount()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    auto const numCPU = sysinfo.dwNumberOfProcessors;
    return numCPU;
}

int SystemInfo::Pimpl::GetCPUThreadCount()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    auto const numCPU = sysinfo.dwNumberOfProcessors;
    return numCPU;
}

std::string SystemInfo::Pimpl::GetCPUFeatures()
{
    // auto const procInfo = SystemInfoLinuxReadProcInfo();
    // auto const vendorID = procInfo.find(std::string("flags"));
    // if (vendorID != std::end(procInfo))
    // {
    //     return vendorID->second;
    // }
    return "";
}
}  // namespace moci

#endif