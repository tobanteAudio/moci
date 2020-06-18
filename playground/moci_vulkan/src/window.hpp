#pragma once

#include "vk/vk.hpp"

namespace mvk
{
class Window
{
public:
    explicit Window();
    ~Window();

    void PollEvents();
    bool ShouldClose();

private:
    constexpr static std::string_view APP_NAME = "Moci: Vulkan";

    void initGLFW();
    void createGLFWWindow();
    void createVulkanInstance();
    void createVulkanPhysicalDevice();
    void queryDeviceProperties();
    void printVulkanDeviceStats();

    // Native window handle
    GLFWwindow* nativeWindow_ = nullptr;

    // Vulkan instance, stores all per-application states
    VkInstance instance_ = VK_NULL_HANDLE;

    // Window surface to render to
    VkSurfaceKHR surface_ = VK_NULL_HANDLE;

    // Available devices
    std::vector<VkPhysicalDevice> physicalDevices_ {};

    // Selected GPU
    VkPhysicalDevice selectedDevice_                                 = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties selectedDeviceProperties_             = {};
    VkPhysicalDeviceFeatures selectedDeviceFeatures_                 = {};
    VkPhysicalDeviceMemoryProperties selectedDeviceMemoryProperties_ = {};
};
}  // namespace mvk