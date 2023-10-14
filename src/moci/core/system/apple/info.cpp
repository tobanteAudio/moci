#include "info.hpp"

#if defined(MOCI_MAC)

    #include <moci/core/strings.hpp>

    #include <algorithm>
    #include <fstream>
    #include <unordered_map>

    #include <sys/sysctl.h>
    #include <sys/types.h>
    #include <sys/utsname.h>

namespace moci {
std::string SystemInfo::Pimpl::getOSName()
{
    struct utsname uts;
    uname(&uts);
    return uts.sysname;
}

std::string SystemInfo::Pimpl::getVendor()
{
    char buffer[512];
    size_t size = sizeof(buffer);
    if (sysctlbyname("machdep.cpu.vendor", &buffer, &size, nullptr, 0) < 0) {
        perror("sysctl");
    }
    return std::string(buffer, size);
}

std::string SystemInfo::Pimpl::getCpuModel()
{
    char buffer[512];
    size_t size = sizeof(buffer);
    if (sysctlbyname("machdep.cpu.brand_string", &buffer, &size, nullptr, 0) < 0) {
        perror("sysctl");
    }
    return std::string(buffer, size);
}

int SystemInfo::Pimpl::getCpuCoreCount()
{
    int buffer;
    size_t size = sizeof(buffer);
    if (sysctlbyname("machdep.cpu.core_count", &buffer, &size, nullptr, 0) < 0) {
        perror("sysctl");
    }

    auto const coreCountStr = std::to_string(buffer);
    int coreCount{-1};

    try {
        coreCount = std::stoi(coreCountStr);
    } catch (std::invalid_argument const& e) {
    } catch (std::out_of_range const& e) {}
    return coreCount;
}

int SystemInfo::Pimpl::getCpuThreadCount()
{
    int buffer;
    size_t size = sizeof(buffer);
    if (sysctlbyname("machdep.cpu.thread_count", &buffer, &size, nullptr, 0) < 0) {
        perror("sysctl");
    }

    auto const coreCountStr = std::to_string(buffer);
    int coreCount{-1};

    try {
        coreCount = std::stoi(coreCountStr);
    } catch (std::invalid_argument const& e) {
    } catch (std::out_of_range const& e) {}
    return coreCount;
}

std::string SystemInfo::Pimpl::getCpuFeatures()
{
    char buffer[512];
    size_t size = sizeof(buffer);
    if (sysctlbyname("machdep.cpu.features", &buffer, &size, nullptr, 0) < 0) {
        perror("sysctl");
    }
    return std::string(buffer, size);
}

}  // namespace moci

#endif
