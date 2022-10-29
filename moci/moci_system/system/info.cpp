#include "moci_system/system/info.hpp"

#include "moci/core/preprocessor.hpp"

#if defined(MOCI_LINUX)
#include "moci_system/system/linux/info.hpp"
#elif defined(MOCI_MAC)
#include "moci_system/system/apple/info.hpp"
#elif defined(MOCI_WINDOWS)
#include "moci_system/system/windows/info.hpp"
#endif

namespace moci
{

auto SystemInfo::GetOSName() -> std::string { return Pimpl::GetOSName(); }
auto SystemInfo::GetVendor() -> std::string { return Pimpl::GetVendor(); }
auto SystemInfo::GetCPUModel() -> std::string { return Pimpl::GetCPUModel(); }
auto SystemInfo::GetCPUCoreCount() -> int { return Pimpl::GetCPUCoreCount(); }
auto SystemInfo::GetCPUThreadCount() -> int { return Pimpl::GetCPUThreadCount(); }
auto SystemInfo::GetCPUFeatures() -> std::string { return Pimpl::GetCPUFeatures(); }

}  // namespace moci
