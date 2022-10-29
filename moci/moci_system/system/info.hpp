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
    static auto GetOSName() -> std::string;

    /**
     * @brief Returns the cpu vendor.
     */
    static auto GetVendor() -> std::string;

    /**
     * @brief Returns the cpu model name.
     */
    static auto GetCPUModel() -> std::string;

    /**
     * @brief Returns the number of cpu cores.
     */
    static auto GetCPUCoreCount() -> int;

    /**
     * @brief Returns the number of cpu threads.
     */
    static auto GetCPUThreadCount() -> int;

    /**
     * @brief Returns a string with all cpu features. Seperated by a literal space.
     */
    static auto GetCPUFeatures() -> std::string;

private:
    class Pimpl;
};

}  // namespace moci
