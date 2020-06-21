#pragma once

#include "vk/vk.hpp"
#include "vk/vk_device.hpp"
#include "vk/vk_swap_chain.hpp"

namespace mvk
{
class Window
{
public:
    explicit Window();
    ~Window();

    void Prepare();

    void PollEvents();
    bool ShouldClose();

private:
    constexpr static std::string_view APP_NAME = "Moci: Vulkan";

    // Init
    void initGLFW();
    void createGLFWWindow();
    void createVulkanInstance();
    void createVulkanPhysicalDevice();
    void createVulkanLogicalDevice();
    void createVulkanQueue();
    void createVulkanSemaphores();
    void createVulkanSubmitInfo();
    void createVulkanCommandPool();
    void queryDeviceProperties();
    void printVulkanDeviceStats();

    struct Semaphores
    {
        VkSemaphore imageAvailable = VK_NULL_HANDLE;
        VkSemaphore renderDone     = VK_NULL_HANDLE;
    };

    // Native window handle
    GLFWwindow* nativeWindow_ = nullptr;

    // Vulkan instance, stores all per-application states
    VkInstance instance_ = VK_NULL_HANDLE;

    // Available devices
    std::vector<VkPhysicalDevice> physicalDevices_ {};

    // Selected GPU
    VkPhysicalDevice selectedDevice_                                 = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties selectedDeviceProperties_             = {};
    VkPhysicalDeviceFeatures selectedDeviceFeatures_                 = {};
    VkPhysicalDeviceMemoryProperties selectedDeviceMemoryProperties_ = {};

    // LogicalDevice
    std::unique_ptr<mvk::VulkanDevice> vulkanDevice_ = nullptr;
    VkDevice logicalDevice_                          = VK_NULL_HANDLE;

    // Handle to the device graphics queue that command buffers are submitted to
    VkQueue queue_;

    // Swap Chain
    std::unique_ptr<VulkanSwapChain> swapChain_ = {};

    // Depth buffer format (selected during Vulkan initialization)
    VkFormat depthFormat_;

    // Command buffer pool
    VkCommandPool cmdPool_;

    Semaphores semaphores_ = {};

    /** @brief Pipeline stages used to wait at for graphics queue submissions */
    VkPipelineStageFlags submitPipelineStages_ = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    // Contains command buffers and semaphores to be presented to the queue
    VkSubmitInfo submitInfo_;
};
}  // namespace mvk