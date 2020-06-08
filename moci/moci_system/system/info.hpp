#pragma once

#include <string>

namespace moci
{
/**
 * @brief Query system & machine information.
 */
class SystemInfo
{
public:
    /**
     * @brief Deleted constructor. Static class.
     */
    SystemInfo() = delete;

    /**
     * @brief Returns the operating system name.
     */
    static std::string GetOSName();

    /**
     * @brief Returns the cpu vendor.
     */
    static std::string GetVendor();

    /**
     * @brief Returns the cpu model name.
     */
    static std::string GetCPUModel();

    /**
     * @brief Returns the number of cpu cores.
     */
    static int GetCPUCoreCount();

    /**
     * @brief Returns the number of cpu threads.
     */
    static int GetCPUThreadCount();

    /**
     * @brief Returns a string with all cpu features. Seperated by a literal space.
     */
    static std::string GetCPUFeatures();

private:
    class Pimpl;
};

}  // namespace moci
