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
    vkDestroyCommandPool(logicalDevice_, commandPool_, nullptr);
    vkDestroyDevice(logicalDevice_, nullptr);
}

VkResult VulkanDevice::CreateLogicalDevice(VkPhysicalDeviceFeatures enabledFeatures,
                                           std::vector<const char*> enabledExtensions, void* pNextChain,
                                           bool useSwapChain, VkQueueFlags requestedQueueTypes)
{

    // Desired queues need to be requested upon logical device creation
    // Due to differing queue family configurations of Vulkan implementations this can be a bit tricky, especially
    // if the application requests different queue types
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos {};

    // Get queue family indices for the requested queue family types
    // Note that the indices may overlap depending on the implementation

    const float defaultQueuePriority(0.0f);

    // Graphics queue
    if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT)
    {
        queueFamilyIndices_.graphics = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
        VkDeviceQueueCreateInfo queueInfo {};
        queueInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = queueFamilyIndices_.graphics;
        queueInfo.queueCount       = 1;
        queueInfo.pQueuePriorities = &defaultQueuePriority;
        queueCreateInfos.push_back(queueInfo);
    }
    else
    {
        queueFamilyIndices_.graphics = VK_NULL_HANDLE;
    }

    // Dedicated compute queue
    if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT)
    {
        queueFamilyIndices_.compute = getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
        if (queueFamilyIndices_.compute != queueFamilyIndices_.graphics)
        {
            // If compute family index differs, we need an additional queue create info for the compute queue
            VkDeviceQueueCreateInfo queueInfo {};
            queueInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.queueFamilyIndex = queueFamilyIndices_.compute;
            queueInfo.queueCount       = 1;
            queueInfo.pQueuePriorities = &defaultQueuePriority;
            queueCreateInfos.push_back(queueInfo);
        }
    }
    else
    {
        // Else we use the same queue
        queueFamilyIndices_.compute = queueFamilyIndices_.graphics;
    }

    // Dedicated transfer queue
    if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT)
    {
        queueFamilyIndices_.transfer = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
        if ((queueFamilyIndices_.transfer != queueFamilyIndices_.graphics)
            && (queueFamilyIndices_.transfer != queueFamilyIndices_.compute))
        {
            // If compute family index differs, we need an additional queue create info for the compute queue
            VkDeviceQueueCreateInfo queueInfo {};
            queueInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.queueFamilyIndex = queueFamilyIndices_.transfer;
            queueInfo.queueCount       = 1;
            queueInfo.pQueuePriorities = &defaultQueuePriority;
            queueCreateInfos.push_back(queueInfo);
        }
    }
    else
    {
        // Else we use the same queue
        queueFamilyIndices_.transfer = queueFamilyIndices_.graphics;
    }

    // Create the logical device representation
    auto deviceExtensions = std::vector<const char*>(enabledExtensions);
    if (useSwapChain)
    {
        // If the device will be used for presenting to a display via a swapchain we need to request the swapchain
        // extension
        deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    VkDeviceCreateInfo deviceCreateInfo   = {};
    deviceCreateInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos    = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures     = &enabledFeatures;

    // If a pNext(Chain) has been passed, we need to add it to the device creation info
    VkPhysicalDeviceFeatures2 physicalDeviceFeatures2 {};
    if (pNextChain != nullptr)
    {
        physicalDeviceFeatures2.sType     = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        physicalDeviceFeatures2.features  = enabledFeatures;
        physicalDeviceFeatures2.pNext     = pNextChain;
        deviceCreateInfo.pEnabledFeatures = nullptr;
        deviceCreateInfo.pNext            = &physicalDeviceFeatures2;
    }

    // Enable the debug marker extension if it is present (likely meaning a debugging tool is present)
    if (extensionSupported(VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
    {
        deviceExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
        enableDebugMarkers_ = true;
    }

    if (deviceExtensions.size() > 0)
    {
        deviceCreateInfo.enabledExtensionCount   = (uint32_t)deviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    }

    VkResult result = vkCreateDevice(physicalDevice_, &deviceCreateInfo, nullptr, &logicalDevice_);

    if (result == VK_SUCCESS)
    {
        // Create a default command pool for graphics command buffers
        commandPool_ = createCommandPool(queueFamilyIndices_.graphics);
    }

    this->enabledFeatures_ = enabledFeatures;

    return result;
}

uint32_t VulkanDevice::getQueueFamilyIndex(VkQueueFlagBits queueFlags)
{
    // Dedicated queue for compute
    // Try to find a queue family index that supports compute but not graphics
    if (queueFlags & VK_QUEUE_COMPUTE_BIT)
    {
        for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties_.size()); i++)
        {
            if ((queueFamilyProperties_[i].queueFlags & queueFlags)
                && ((queueFamilyProperties_[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
            {
                return i;
                break;
            }
        }
    }

    // Dedicated queue for transfer
    // Try to find a queue family index that supports transfer but not graphics and compute
    if (queueFlags & VK_QUEUE_TRANSFER_BIT)
    {
        for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties_.size()); i++)
        {
            if ((queueFamilyProperties_[i].queueFlags & queueFlags)
                && ((queueFamilyProperties_[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)
                && ((queueFamilyProperties_[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
            {
                return i;
                break;
            }
        }
    }

    // For other queue types or if no separate compute queue is present, return the first one to support the requested
    // flags
    for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties_.size()); i++)
    {
        if (queueFamilyProperties_[i].queueFlags & queueFlags)
        {
            return i;
            break;
        }
    }

    throw std::runtime_error("Could not find a matching queue family index");
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