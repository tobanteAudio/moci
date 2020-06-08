#include "moci_system/system/info.hpp"

#include "moci_core/core/preprocessor.hpp"

#if defined(MOCI_LINUX)
#include "moci_system/system/linux/info.hpp"
#elif defined(MOCI_MAC)
#include "moci_system/system/apple/info.hpp"
#elif defined(MOCI_WINDOWS)
#include "moci_system/system/windows/info.hpp"
#endif

namespace moci
{

std::string SystemInfo::GetOSName() { return Pimpl::GetOSName(); }
std::string SystemInfo::GetVendor() { return Pimpl::GetVendor(); }
std::string SystemInfo::GetCPUModel() { return Pimpl::GetCPUModel(); }
int SystemInfo::GetCPUCoreCount() { return Pimpl::GetCPUCoreCount(); }
int SystemInfo::GetCPUThreadCount() { return Pimpl::GetCPUThreadCount(); }
std::string SystemInfo::GetCPUFeatures() { return Pimpl::GetCPUFeatures(); }

}  // namespace moci
