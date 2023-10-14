#pragma once
#include <moci/core/preprocessor.hpp>

#if defined(MOCI_WINDOWS)

    #include <moci/core/system/info.hpp>

    #include <string>
    #include <unordered_map>

namespace moci {

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
