#include "vk_device.hpp"

namespace mvk
{
VulkanDevice::VulkanDevice(VkPhysicalDevice device) : physicalDevice_ {device}
{

    queryDeviceProperties();
    quyeryQueueFamilyProperties();
    querySupportedExtensions();
}

/**
 * Default destructor
 *
 * @note Frees the logical device
 */
VulkanDevice::~VulkanDevice()
{
    if (commandPool != VK_NULL_HANDLE) { vkDestroyCommandPool(logicalDevice_, commandPool, nullptr); }
    if (logicalDevice_ != VK_NULL_HANDLE) { vkDestroyDevice(logicalDevice_, nullptr); }
}

void VulkanDevice::queryDeviceProperties()
{
    // Store Properties features, limits and properties of the physical device for later use
    // Device properties also contain limits and sparse properties
    vkGetPhysicalDeviceProperties(physicalDevice_, &properties_);
    // Features should be checked by the examples before using them
    vkGetPhysicalDeviceFeatures(physicalDevice_, &features);
    // Memory properties are used regularly for creating all kinds of buffers
    vkGetPhysicalDeviceMemoryProperties(physicalDevice_, &memoryProperties_);
}

void VulkanDevice::quyeryQueueFamilyProperties()
{
    // Queue family properties, used for setting up requested queues upon device creation
    auto queueFamilyCount = std::uint32_t {};
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueFamilyCount, nullptr);
    queueFamilyProperties_.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueFamilyCount, queueFamilyProperties_.data());
}

void VulkanDevice::querySupportedExtensions()
{
    // Get list of supported extensions
    std::uint32_t extCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice_, nullptr, &extCount, nullptr);
    if (extCount > 0)
    {
        auto extensions = std::vector<VkExtensionProperties>(extCount);
        if (vkEnumerateDeviceExtensionProperties(physicalDevice_, nullptr, &extCount, &extensions.front())
            == VK_SUCCESS)
        {
            for (auto ext : extensions) { supportedExtensions_.push_back(ext.extensionName); }
        }
    }
}
}  // namespace mvk