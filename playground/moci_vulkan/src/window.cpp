#include "window.hpp"

#include "vk/vk.hpp"

namespace mvk
{
Window::Window()
{
    initGLFW();
    createVulkanInstance();
    createGLFWWindow();
    createVulkanPhysicalDevice();
    createVulkanLogicalDevice();
    createVulkanQueue();
    createVulkanSemaphores();
}

Window::~Window()
{
    vkDestroySemaphore(logicalDevice_, semaphores_.imageAvailable, nullptr);
    vkDestroySemaphore(logicalDevice_, semaphores_.renderDone, nullptr);

    // Needs to be explicit, since the order matters.
    vulkanDevice_.reset(nullptr);

    vkDestroySurfaceKHR(instance_, surface_, nullptr);
    vkDestroyInstance(instance_, nullptr);

    glfwDestroyWindow(nativeWindow_);
    glfwTerminate();
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

    // Create window surface, looks a lot like a Vulkan function ( and not GLFW
    // function ) This is a one function solution for all operating systems. No
    // need to hassle with the OS specifics. For windows this would be
    // vkCreateWin32SurfaceKHR() or on linux XCB window library this would be
    // vkCreateXcbSurfaceKHR()
    if (VK_SUCCESS != glfwCreateWindowSurface(instance_, nativeWindow_, nullptr, &surface_))
    {
        glfwTerminate();
        throw std::runtime_error("couldn't create surface, exit");
    }
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
