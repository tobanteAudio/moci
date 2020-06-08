#pragma once

#include <cstdio>
#include <iostream>

// You tell GLFW to use vulkan by defining GLFW_INCLUDE_VULKAN, GLFW
// automatically looks for vulkan header in <vulkan/vulkan.h> if not included
// already.
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

inline std::string errorString(VkResult errorCode)
{
    switch (errorCode)
    {
#define STR(r)                                                                                                         \
    case VK_##r: return #r
        STR(NOT_READY);
        STR(TIMEOUT);
        STR(EVENT_SET);
        STR(EVENT_RESET);
        STR(INCOMPLETE);
        STR(ERROR_OUT_OF_HOST_MEMORY);
        STR(ERROR_OUT_OF_DEVICE_MEMORY);
        STR(ERROR_INITIALIZATION_FAILED);
        STR(ERROR_DEVICE_LOST);
        STR(ERROR_MEMORY_MAP_FAILED);
        STR(ERROR_LAYER_NOT_PRESENT);
        STR(ERROR_EXTENSION_NOT_PRESENT);
        STR(ERROR_FEATURE_NOT_PRESENT);
        STR(ERROR_INCOMPATIBLE_DRIVER);
        STR(ERROR_TOO_MANY_OBJECTS);
        STR(ERROR_FORMAT_NOT_SUPPORTED);
        STR(ERROR_SURFACE_LOST_KHR);
        STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
        STR(SUBOPTIMAL_KHR);
        STR(ERROR_OUT_OF_DATE_KHR);
        STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
        STR(ERROR_VALIDATION_FAILED_EXT);
        STR(ERROR_INVALID_SHADER_NV);
#undef STR
        default: return "UNKNOWN_ERROR";
    }
}

#define VULKAN_CALL(f)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        VkResult res = (f);                                                                                            \
        if (res != VK_SUCCESS)                                                                                         \
        {                                                                                                              \
            std::printf("Fatal: VkResult is: \"%s\" in %s at line %d\n", errorString(res).c_str(), __FILE__,           \
                        __LINE__);                                                                                     \
        }                                                                                                              \
    } while (false);

namespace th
{
inline auto PrintVulkanDeviceStats(VkPhysicalDevice const& device) -> void
{
    auto properties = VkPhysicalDeviceProperties {};
    vkGetPhysicalDeviceProperties(device, &properties);
    std::printf("Name: %s\n", properties.deviceName);
    auto const version = properties.apiVersion;
    std::printf("API: %d.%d.%d\n", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));
    std::printf("Type: %d\n", properties.deviceType);
    std::printf("Discrete Queue Prios: %d\n", properties.limits.discreteQueuePriorities);

    auto features = VkPhysicalDeviceFeatures {};
    vkGetPhysicalDeviceFeatures(device, &features);
    std::printf("Geometry Shader: %d\n", features.geometryShader);
    std::printf("Tessellation Shader: %d\n", features.tessellationShader);

    auto memProperties = VkPhysicalDeviceMemoryProperties {};
    vkGetPhysicalDeviceMemoryProperties(device, &memProperties);
    std::printf("Heap Count: %d\n", memProperties.memoryHeapCount);

    std::puts("\n");
}

}  // namespace th
