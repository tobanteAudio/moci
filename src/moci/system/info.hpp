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
    static auto getOsName() -> std::string;

    /**
     * @brief Returns the cpu vendor.
     */
    static auto getVendor() -> std::string;

    /**
     * @brief Returns the cpu model name.
     */
    static auto getCpuModel() -> std::string;

    /**
     * @brief Returns the number of cpu cores.
     */
    static auto getCpuCoreCount() -> int;

    /**
     * @brief Returns the number of cpu threads.
     */
    static auto getCpuThreadCount() -> int;

    /**
     * @brief Returns a string with all cpu features. Seperated by a literal space.
     */
    static auto getCpuFeatures() -> std::string;

private:
    class Pimpl;
};

}  // namespace moci
