#include "moci_system/system/linux/info.hpp"

#if defined(MOCI_LINUX)

#include "moci_core/core/strings.hpp"

#include <algorithm>
#include <fstream>
#include <unordered_map>

#include <sys/utsname.h>

namespace moci
{

std::unordered_map<std::string, std::string> SystemInfoLinuxReadProcInfo()
{
    std::string line;
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open())
    {
        return {};
    }

    std::unordered_map<std::string, std::string> result {};
    while (getline(cpuinfo, line))
    {
        auto found = std::find(std::begin(line), std::end(line), ':');
        if (found != std::end(line))
        {
            auto key   = std::string(std::begin(line), found);
            auto value = std::string(++found, line.end());
            Strings::Trim(key);
            Strings::Trim(value);
            result.insert(std::make_pair(key, value));
        }
    }

    cpuinfo.close();

    return result;
}

std::string SystemInfo::Pimpl::GetOSName()
{
    utsname uts {};
    uname(&uts);
    return uts.sysname;
}

std::string SystemInfo::Pimpl::GetVendor()
{
    auto const procInfo = SystemInfoLinuxReadProcInfo();
    auto const vendorID = procInfo.find(std::string("vendor_id"));
    if (vendorID != std::end(procInfo))
    {
        return vendorID->second;
    }
    return "";
}

std::string SystemInfo::Pimpl::GetCPUModel()
{
    auto const procInfo = SystemInfoLinuxReadProcInfo();
    auto const vendorID = procInfo.find(std::string("model name"));
    if (vendorID != std::end(procInfo))
    {
        return vendorID->second;
    }
    return "";
}

int SystemInfo::Pimpl::GetCPUCoreCount()
{
    auto const procInfo = SystemInfoLinuxReadProcInfo();
    auto const vendorID = procInfo.find(std::string("cpu cores"));
    int coreCount {-1};
    if (vendorID != std::end(procInfo))
    {
        try
        {
            coreCount = std::stoi(vendorID->second);
        }
        catch (std::invalid_argument const& e)
        {
        }
        catch (std::out_of_range const& e)
        {
        }
        return coreCount;
    }

    return coreCount;
}

int SystemInfo::Pimpl::GetCPUThreadCount()
{
    auto const procInfo = SystemInfoLinuxReadProcInfo();
    auto const vendorID = procInfo.find(std::string("siblings"));
    int threadCount {-1};
    if (vendorID != std::end(procInfo))
    {
        try
        {
            threadCount = std::stoi(vendorID->second);
        }
        catch (std::invalid_argument const& e)
        {
        }
        catch (std::out_of_range const& e)
        {
        }
        return threadCount;
    }

    return threadCount;
}

std::string SystemInfo::Pimpl::GetCPUFeatures()
{
    auto const procInfo = SystemInfoLinuxReadProcInfo();
    auto const vendorID = procInfo.find(std::string("flags"));
    if (vendorID != std::end(procInfo))
    {
        return vendorID->second;
    }
    return "";
}
}  // namespace moci

#endif