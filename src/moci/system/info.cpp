#include "moci/system/info.hpp"

#include "moci/core/preprocessor.hpp"

#if defined(MOCI_LINUX)
#include "moci/system/linux/info.hpp"
#elif defined(MOCI_MAC)
#include "moci/system/apple/info.hpp"
#elif defined(MOCI_WINDOWS)
#include "moci/system/windows/info.hpp"
#endif

namespace moci
{

auto SystemInfo::getOSName() -> std::string { return Pimpl::getOSName(); }
auto SystemInfo::getVendor() -> std::string { return Pimpl::getVendor(); }
auto SystemInfo::getCpuModel() -> std::string { return Pimpl::getCpuModel(); }
auto SystemInfo::getCpuCoreCount() -> int { return Pimpl::getCpuCoreCount(); }
auto SystemInfo::getCpuThreadCount() -> int { return Pimpl::getCpuThreadCount(); }
auto SystemInfo::getCpuFeatures() -> std::string { return Pimpl::getCpuFeatures(); }

}  // namespace moci
