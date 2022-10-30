#pragma once
#include "moci/core/preprocessor.hpp"

#if defined(MOCI_MAC)

#include "moci/system/info.hpp"

namespace moci
{
class SystemInfo::Pimpl
{
public:
    static std::string getOSName();
    static std::string getVendor();
    static std::string getCpuModel();
    static int getCpuCoreCount();
    static int getCpuThreadCount();
    static std::string getCpuFeatures();
};

}  // namespace moci

#endif
