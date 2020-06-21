#include "vk_swap_chain.hpp"

namespace mvk
{
VulkanSwapChain::VulkanSwapChain() { }
VulkanSwapChain::~VulkanSwapChain() { vkDestroySurfaceKHR(instance_, surface_, nullptr); }

void VulkanSwapChain::Connect(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device)
{
    instance_       = instance;
    physicalDevice_ = physicalDevice;
    device_         = device;
}

void VulkanSwapChain::InitSurface(GLFWwindow* window)
{
    if (VK_SUCCESS != glfwCreateWindowSurface(instance_, window, nullptr, &surface_))
    { throw std::runtime_error("couldn't create surface, exit"); }

    // Get available queue family properties
    uint32_t queueCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueCount, NULL);
    assert(queueCount >= 1);

    std::vector<VkQueueFamilyProperties> queueProps(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueCount, queueProps.data());

    auto queueFamilyCount = uint32_t {0};
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueFamilyCount, nullptr);
    auto familyProperties = std::vector<VkQueueFamilyProperties> {};
    familyProperties.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueFamilyCount, familyProperties.data());

    for (uint32_t i = 0; i < queueFamilyCount; ++i)
    {
        if (familyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) { queueNodeIndex_ = i; }
    }

    if (queueNodeIndex_ == UINT32_MAX)
    { throw std::runtime_error("Could not find a graphics and/or presenting queue!"); }

    // // Iterate over each queue to learn whether it supports presenting:
    // // Find a queue with present support
    // // Will be used to present the swap chain images to the windowing system
    // std::vector<VkBool32> supportsPresent(queueCount);
    // for (uint32_t i = 0; i < queueCount; i++)
    // { fpGetPhysicalDeviceSurfaceSupportKHR(physicalDevice_, i, surface, &supportsPresent[i]); }

    // // Search for a graphics and a present queue in the array of queue
    // // families, try to find one that supports both
    // uint32_t graphicsQueueNodeIndex = UINT32_MAX;
    // uint32_t presentQueueNodeIndex  = UINT32_MAX;
    // for (uint32_t i = 0; i < queueCount; i++)
    // {
    //     if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
    //     {
    //         if (graphicsQueueNodeIndex == UINT32_MAX) { graphicsQueueNodeIndex = i; }

    //         if (supportsPresent[i] == VK_TRUE)
    //         {
    //             graphicsQueueNodeIndex = i;
    //             presentQueueNodeIndex  = i;
    //             break;
    //         }
    //     }
    // }
    // if (presentQueueNodeIndex == UINT32_MAX)
    // {
    //     // If there's no queue that supports both present and graphics
    //     // try to find a separate present queue
    //     for (uint32_t i = 0; i < queueCount; ++i)
    //     {
    //         if (supportsPresent[i] == VK_TRUE)
    //         {
    //             presentQueueNodeIndex = i;
    //             break;
    //         }
    //     }
    // }

    // // Exit if either a graphics or a presenting queue hasn't been found
    // if (graphicsQueueNodeIndex == UINT32_MAX || presentQueueNodeIndex == UINT32_MAX)
    // { throw std::runtime_error("Could not find a graphics and/or presenting queue!"); }

    // // todo : Add support for separate graphics and presenting queue
    // if (graphicsQueueNodeIndex != presentQueueNodeIndex)
    // { throw std::runtime_error("Separate graphics and presenting queues are not supported yet!"); }

    // queueNodeIndex_ = graphicsQueueNodeIndex;

    // // Get list of supported surface formats
    // uint32_t formatCount;
    // VULKAN_CALL(GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice_, surface_, &formatCount, NULL));
    // assert(formatCount > 0);

    // std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
    // VULKAN_CALL(GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice_, surface_, &formatCount, surfaceFormats.data()));

    // // If the surface format list only includes one entry with VK_FORMAT_UNDEFINED,
    // // there is no preferered format, so we assume VK_FORMAT_B8G8R8A8_UNORM
    // if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
    // {
    //     colorFormat_ = VK_FORMAT_B8G8R8A8_UNORM;
    //     colorSpace_  = surfaceFormats[0].colorSpace;
    // }
    // else
    // {
    //     // iterate over the list of available surface format and
    //     // check for the presence of VK_FORMAT_B8G8R8A8_UNORM
    //     bool found_B8G8R8A8_UNORM = false;
    //     for (auto&& surfaceFormat : surfaceFormats)
    //     {
    //         if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
    //         {
    //             colorFormat_         = surfaceFormat.format;
    //             colorSpace_          = surfaceFormat.colorSpace;
    //             found_B8G8R8A8_UNORM = true;
    //             break;
    //         }
    //     }

    //     // in case VK_FORMAT_B8G8R8A8_UNORM is not available
    //     // select the first available color format
    //     if (!found_B8G8R8A8_UNORM)
    //     {
    //         colorFormat_ = surfaceFormats[0].format;
    //         colorSpace_  = surfaceFormats[0].colorSpace;
    //     }
    // }
}
}  // namespace mvk