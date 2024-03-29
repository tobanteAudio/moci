#include "info.hpp"

#if defined(MOCI_LINUX)

    #include <moci/core/strings.hpp>

    #include <algorithm>
    #include <fstream>
    #include <unordered_map>

    #include <sys/utsname.h>

namespace moci {

auto systemInfoLinuxReadProcInfo() -> std::unordered_map<std::string, std::string>
{
    std::string line;
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open()) {
        return {};
    }

    std::unordered_map<std::string, std::string> result{};
    while (getline(cpuinfo, line)) {
        auto found = std::find(std::begin(line), std::end(line), ':');
        if (found != std::end(line)) {
            auto key   = std::string(std::begin(line), found);
            auto value = std::string(++found, line.end());
            Strings::trim(key);
            Strings::trim(value);
            result.insert(std::make_pair(key, value));
        }
    }

    cpuinfo.close();

    return result;
}

auto SystemInfo::Pimpl::getOSName() -> std::string
{
    utsname uts{};
    uname(&uts);
    return uts.sysname;
}

auto SystemInfo::Pimpl::getVendor() -> std::string
{
    auto const procInfo = systemInfoLinuxReadProcInfo();
    auto const vendorID = procInfo.find(std::string("vendor_id"));
    if (vendorID != std::end(procInfo)) {
        return vendorID->second;
    }
    return "";
}

auto SystemInfo::Pimpl::getCpuModel() -> std::string
{
    auto const procInfo = systemInfoLinuxReadProcInfo();
    auto const vendorID = procInfo.find(std::string("model name"));
    if (vendorID != std::end(procInfo)) {
        return vendorID->second;
    }
    return "";
}

auto SystemInfo::Pimpl::getCpuCoreCount() -> int
{
    auto const procInfo = systemInfoLinuxReadProcInfo();
    auto const vendorID = procInfo.find(std::string("cpu cores"));
    int coreCount{-1};
    if (vendorID != std::end(procInfo)) {
        try {
            coreCount = std::stoi(vendorID->second);
        } catch (std::invalid_argument const& e) {
        } catch (std::out_of_range const& e) {}
        return coreCount;
    }

    return coreCount;
}

auto SystemInfo::Pimpl::getCpuThreadCount() -> int
{
    auto const procInfo = systemInfoLinuxReadProcInfo();
    auto const vendorID = procInfo.find(std::string("siblings"));
    int threadCount{-1};
    if (vendorID != std::end(procInfo)) {
        try {
            threadCount = std::stoi(vendorID->second);
        } catch (std::invalid_argument const& e) {
        } catch (std::out_of_range const& e) {}
        return threadCount;
    }

    return threadCount;
}

auto SystemInfo::Pimpl::getCpuFeatures() -> std::string
{
    auto const procInfo = systemInfoLinuxReadProcInfo();
    auto const vendorID = procInfo.find(std::string("flags"));
    if (vendorID != std::end(procInfo)) {
        return vendorID->second;
    }
    return "";
}
}  // namespace moci

#endif
