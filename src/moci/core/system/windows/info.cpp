#include "info.hpp"

#if defined(MOCI_WINDOWS)

    #include <windows.h>

namespace moci {
std::string SystemInfo::Pimpl::getOSName() { return "Windows"; }

std::string SystemInfo::Pimpl::getVendor()
{
    // auto const procInfo = SystemInfoLinuxReadProcInfo();
    // auto const vendorID = procInfo.find(std::string("vendor_id"));
    // if (vendorID != std::end(procInfo))
    // {
    //     return vendorID->second;
    // }
    return "";
}

std::string SystemInfo::Pimpl::getCpuModel()
{
    // auto const procInfo = SystemInfoLinuxReadProcInfo();
    // auto const vendorID = procInfo.find(std::string("model name"));
    // if (vendorID != std::end(procInfo))
    // {
    //     return vendorID->second;
    // }
    return "";
}

int SystemInfo::Pimpl::getCpuCoreCount()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    auto const numCPU = sysinfo.dwNumberOfProcessors;
    return numCPU;
}

int SystemInfo::Pimpl::getCpuThreadCount()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    auto const numCPU = sysinfo.dwNumberOfProcessors;
    return numCPU;
}

std::string SystemInfo::Pimpl::getCpuFeatures()
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
