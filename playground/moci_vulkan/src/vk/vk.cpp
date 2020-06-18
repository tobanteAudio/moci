#include "vk.hpp"

namespace mvk
{
auto PrintVulkanDeviceStats(VkPhysicalDevice const& device) -> void
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

}  // namespace mvk
