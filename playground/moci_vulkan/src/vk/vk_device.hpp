#pragma once

#include "vk.hpp"

namespace mvk
{
class VulkanDevice
{
public:
    /** @brief Contains queue family indices */
    struct QueueFamilyIndex
    {
        std::uint32_t graphics;
        std::uint32_t compute;
        std::uint32_t transfer;
    };

    VulkanDevice(VkPhysicalDevice device);

    ~VulkanDevice();

    /**  @brief Typecast to VkDevice */
    [[nodiscard]] operator VkDevice() const noexcept { return logicalDevice_; };

    /**
     * Create the logical device based on the assigned physical device, also gets default queue family indices
     *
     * @param enabledFeatures Can be used to enable certain features upon device creation
     * @param pNextChain Optional chain of pointer to extension structures
     * @param useSwapChain Set to false for headless rendering to omit the swapchain device extensions
     * @param requestedQueueTypes Bit flags specifying the queue types to be requested from the device
     *
     * @return VkResult of the device creation call
     */
    VkResult CreateLogicalDevice(VkPhysicalDeviceFeatures enabledFeatures, std::vector<const char*> enabledExtensions,
                                 void* pNextChain, bool useSwapChain = true,
                                 VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);

    QueueFamilyIndex const& GetQueueFamilyIndex() { return queueFamilyIndices_; }

    /**
     * Create a command pool for allocation command buffers from
     *
     * @param queueFamilyIndex Family index of the queue to create the command pool for
     * @param createFlags (Optional) Command pool creation flags (Defaults to
     * VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
     *
     * @note Command buffers allocated from the created pool can only be submitted to a queue with the same family index
     *
     * @return A handle to the created command buffer
     */
    VkCommandPool createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags
                                                               = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
    {
        auto cmdPoolInfo             = VkCommandPoolCreateInfo {};
        cmdPoolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
        cmdPoolInfo.flags            = createFlags;

        auto cmdPool = VkCommandPool {};
        VULKAN_CALL(vkCreateCommandPool(logicalDevice_, &cmdPoolInfo, nullptr, &cmdPool));
        return cmdPool;
    }

    /**
     * Get the index of a queue family that supports the requested queue flags
     *
     * @param queueFlags Queue flags to find a queue family index for
     *
     * @return Index of the queue family index that matches the flags
     *
     * @throw Throws an exception if no queue family index could be found that supports the requested flags
     */
    uint32_t getQueueFamilyIndex(VkQueueFlagBits queueFlags);

    /**
     * Check if an extension is supported by the (physical device)
     *
     * @param extension Name of the extension to check
     *
     * @return True if the extension is supported (present in the list read at device creation time)
     */
    bool extensionSupported(std::string extension)
    {
        return (std::find(supportedExtensions_.begin(), supportedExtensions_.end(), extension)
                != supportedExtensions_.end());
    }

    /**
     * Select the best-fit depth format for this device from a list of possible depth (and stencil) formats
     *
     * @param checkSamplingSupport Check if the format can be sampled from (e.g. for shader reads)
     *
     * @return The depth format that best fits for the current device
     *
     * @throw Throws an exception if no depth format fits the requirements
     */
    VkFormat getSupportedDepthFormat(bool checkSamplingSupport)
    {
        // All depth formats may be optional, so we need to find a suitable depth format to use
        std::vector<VkFormat> depthFormats
            = {VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT,
               VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D16_UNORM};
        for (auto& format : depthFormats)
        {
            VkFormatProperties formatProperties;
            vkGetPhysicalDeviceFormatProperties(physicalDevice_, format, &formatProperties);
            // Format must support depth stencil attachment for optimal tiling
            if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
            {
                if (checkSamplingSupport)
                {
                    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)) { continue; }
                }
                return format;
            }
        }
        throw std::runtime_error("Could not find a matching depth format");
    }

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
    VkCommandPool commandPool_ = VK_NULL_HANDLE;

    /** @brief Set to true when the debug marker extension is detected */
    bool enableDebugMarkers_ = false;

    QueueFamilyIndex queueFamilyIndices_ = {};
};
}  // namespace mvk
