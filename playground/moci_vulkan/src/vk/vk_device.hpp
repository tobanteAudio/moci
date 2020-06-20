#pragma once

#include "vk.hpp"

namespace mvk
{
class VulkanDevice
{
public:
    /**  @brief Typecast to VkDevice */
    operator VkDevice() { return logicalDevice_; };

    VulkanDevice(VkPhysicalDevice device);

    /**
     * Default destructor
     *
     * @note Frees the logical device
     */
    ~VulkanDevice();

private:
    void queryDeviceProperties();

    void quyeryQueueFamilyProperties();

    void querySupportedExtensions();

    /** @brief Physical device representation */
    VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;

    /** @brief Logical device representation (application's view of the device) */
    VkDevice logicalDevice_ = VK_NULL_HANDLE;

    /** @brief Properties of the physical device including limits that the application can check against */
    VkPhysicalDeviceProperties properties_ = {};

    /** @brief Features of the physical device that an application can use to check if a feature is supported */
    VkPhysicalDeviceFeatures features = {};

    /** @brief Features that have been enabled for use on the physical device */
    VkPhysicalDeviceFeatures enabledFeatures_ = {};

    /** @brief Memory types and heaps of the physical device */
    VkPhysicalDeviceMemoryProperties memoryProperties_ = {};

    /** @brief Queue family properties of the physical device */
    std::vector<VkQueueFamilyProperties> queueFamilyProperties_ = {};

    /** @brief List of extensions supported by the device */
    std::vector<std::string> supportedExtensions_ = {};

    /** @brief Default command pool for the graphics queue family index */
    VkCommandPool commandPool = VK_NULL_HANDLE;

    /** @brief Set to true when the debug marker extension is detected */
    bool enableDebugMarkers_ = false;

    /** @brief Contains queue family indices */
    struct QueueFamilyIndex
    {
        std::uint32_t graphics;
        std::uint32_t compute;
        std::uint32_t transfer;
    };

    QueueFamilyIndex queueFamilyIndices_ = {};
};
}  // namespace mvk
