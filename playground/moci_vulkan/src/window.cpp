#include "window.hpp"

#include "vk/vk.hpp"

namespace mvk
{
Window::Window():swapChain_{std::make_unique<VulkanSwapChain>()}
{
    initGLFW();
    createVulkanInstance();
    createGLFWWindow();
    createVulkanPhysicalDevice();
    createVulkanLogicalDevice();
    createVulkanQueue();
    swapChain_->Connect(instance_, selectedDevice_, logicalDevice_);
    createVulkanSemaphores();
    createVulkanSubmitInfo();
}

Window::~Window()
{
    vkDestroyCommandPool(logicalDevice_, cmdPool_, nullptr);

    vkDestroySemaphore(logicalDevice_, semaphores_.imageAvailable, nullptr);
    vkDestroySemaphore(logicalDevice_, semaphores_.renderDone, nullptr);

    // Needs to be explicit, since the order matters.
    swapChain_.reset(nullptr);
    vulkanDevice_.reset(nullptr);

    vkDestroyInstance(instance_, nullptr);

    glfwDestroyWindow(nativeWindow_);
    glfwTerminate();
}

void Window::Prepare()
{
    swapChain_->InitSurface(nativeWindow_);
    createVulkanCommandPool();
}

void Window::PollEvents() { glfwPollEvents(); }
bool Window::ShouldClose() { return glfwWindowShouldClose(nativeWindow_); }

void Window::initGLFW()
{
    if (glfwInit() == GLFW_FALSE) { throw std::runtime_error("Could not init GLFW"); }

    if (glfwVulkanSupported() == GLFW_FALSE)
    {
        glfwTerminate();
        throw std::runtime_error("GLFW vulkan support is false");
    }
}

void Window::createGLFWWindow()
{

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    nativeWindow_ = glfwCreateWindow(1280, 720, APP_NAME.data(), nullptr, nullptr);

    // glfwSetWindowUserPointer(nativeWindow_, this);
    // glfwSetWindowSizeCallback(nativeWindow_, [](GLFWwindow* window, int width, int height) {});
    // glfwGetFramebufferSize(nativeWindow_, &windowData_.width, &windowData_.height);
}
void Window::createVulkanInstance()
{
    VkApplicationInfo appInfo  = {};
    appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext              = nullptr;
    appInfo.apiVersion         = VK_API_VERSION_1_2;
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pApplicationName   = "MoCi: Vulkan";
    appInfo.engineVersion      = VK_MAKE_VERSION(0, 1, 0);
    appInfo.pEngineName        = "MoCi: Vulkan";

    auto validationLayers = std::vector<char const*> {
        "VK_LAYER_KHRONOS_validation",
    };

    auto glfwExtensionCount = uint32_t {0};
    auto glfwExtensions     = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    auto instanceInfo                    = VkInstanceCreateInfo {};
    instanceInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo        = &appInfo;
    instanceInfo.pNext                   = nullptr;
    instanceInfo.flags                   = 0;
    instanceInfo.enabledLayerCount       = validationLayers.size();
    instanceInfo.ppEnabledLayerNames     = validationLayers.data();
    instanceInfo.enabledExtensionCount   = glfwExtensionCount;
    instanceInfo.ppEnabledExtensionNames = glfwExtensions;

    VULKAN_CALL(vkCreateInstance(&instanceInfo, nullptr, &instance_));
}

void Window::createVulkanPhysicalDevice()
{
    auto gpuCount = uint32_t {0};
    VULKAN_CALL(vkEnumeratePhysicalDevices(instance_, &gpuCount, nullptr));
    physicalDevices_.resize(gpuCount);
    VULKAN_CALL(vkEnumeratePhysicalDevices(instance_, &gpuCount, physicalDevices_.data()));

    // Select default GPU, aka index 0.
    selectedDevice_ = physicalDevices_.at(0);

    queryDeviceProperties();
    printVulkanDeviceStats();
}

void Window::createVulkanLogicalDevice()
{
    // Vulkan device creation
    // This is handled by a separate class that gets a logical device representation
    // and encapsulates functions related to a device
    vulkanDevice_ = std::make_unique<mvk::VulkanDevice>(selectedDevice_);
    VULKAN_CALL(vulkanDevice_->CreateLogicalDevice(selectedDeviceFeatures_, {}, nullptr));
    logicalDevice_ = static_cast<VkDevice>(*vulkanDevice_);
}

void Window::createVulkanQueue()
{
    // Get a graphics queue from the device
    auto const& index = vulkanDevice_->GetQueueFamilyIndex();
    vkGetDeviceQueue(logicalDevice_, index.graphics, 0, &queue_);
}

void Window::createVulkanSemaphores()
{
    auto semaphoreInfo  = VkSemaphoreCreateInfo {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.pNext = nullptr;
    semaphoreInfo.flags = 0;

    // Create a semaphore used to synchronize image presentation
    // Ensures that the image is displayed before we start submitting new commands to the queu
    VULKAN_CALL(vkCreateSemaphore(logicalDevice_, &semaphoreInfo, nullptr, &semaphores_.imageAvailable));
    // Create a semaphore used to synchronize command submission
    // Ensures that the image is not presented until all commands have been sumbitted and executed
    VULKAN_CALL(vkCreateSemaphore(logicalDevice_, &semaphoreInfo, nullptr, &semaphores_.renderDone));
}

void Window::createVulkanSubmitInfo()
{
    // Set up submit info structure
    // Semaphores will stay the same during application lifetime
    // Command buffer submission info is set by each example
    submitInfo_.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo_.pWaitDstStageMask    = &submitPipelineStages_;
    submitInfo_.waitSemaphoreCount   = 1;
    submitInfo_.pWaitSemaphores      = &semaphores_.imageAvailable;
    submitInfo_.signalSemaphoreCount = 1;
    submitInfo_.pSignalSemaphores    = &semaphores_.renderDone;
}

void Window::createVulkanCommandPool()
{
    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.queueFamilyIndex        = swapChain_->GetQueueNodeIndex();
    cmdPoolInfo.flags                   = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VULKAN_CALL(vkCreateCommandPool(logicalDevice_, &cmdPoolInfo, nullptr, &cmdPool_));
}

void Window::queryDeviceProperties()
{
    // Store properties (including limits), features and memory properties of the phyiscal device (so that examples can
    // check against them)
    vkGetPhysicalDeviceProperties(selectedDevice_, &selectedDeviceProperties_);
    vkGetPhysicalDeviceFeatures(selectedDevice_, &selectedDeviceFeatures_);
    vkGetPhysicalDeviceMemoryProperties(selectedDevice_, &selectedDeviceMemoryProperties_);
}
void Window::printVulkanDeviceStats()
{
    std::printf("Name: %s\n", selectedDeviceProperties_.deviceName);
    auto const version = selectedDeviceProperties_.apiVersion;
    std::printf("API: %d.%d.%d\n", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));
    std::printf("Type: %d\n", selectedDeviceProperties_.deviceType);
    std::printf("Discrete Queue Prios: %d\n", selectedDeviceProperties_.limits.discreteQueuePriorities);

    std::printf("Geometry Shader: %d\n", selectedDeviceFeatures_.geometryShader);
    std::printf("Tessellation Shader: %d\n", selectedDeviceFeatures_.tessellationShader);

    std::printf("Heap Count: %d\n", selectedDeviceMemoryProperties_.memoryHeapCount);
    std::puts("\n");
}
}  // namespace mvk
