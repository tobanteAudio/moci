#pragma once

#include "vk.hpp"

namespace mvk
{
struct SwapChainBuffer
{
    VkImage image;
    VkImageView view;
};

class VulkanSwapChain
{
public:
    VulkanSwapChain();
    ~VulkanSwapChain();

    VulkanSwapChain(VulkanSwapChain&) = delete;
    VulkanSwapChain& operator=(VulkanSwapChain&) = delete;

    void Connect(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device);

    void InitSurface(GLFWwindow* window);

    std::uint32_t GetQueueNodeIndex() const noexcept { return queueNodeIndex_; }

private:
    VkInstance instance_             = VK_NULL_HANDLE;
    VkDevice device_                 = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
    VkSurfaceKHR surface_            = VK_NULL_HANDLE;

//    VkFormat colorFormat_                 = {};
//    VkColorSpaceKHR colorSpace_           = {};
//    VkSwapchainKHR swapChain_             = VK_NULL_HANDLE;
//    std::uint32_t imageCount_             = 0;
//    std::vector<VkImage> images_          = {};
//    std::vector<SwapChainBuffer> buffers_ = {};
    std::uint32_t queueNodeIndex_         = UINT32_MAX;
};
}  // namespace mvk