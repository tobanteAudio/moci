#include "window.hpp"

#include "vk/vk.hpp"

namespace mvk
{
Window::Window()
{
    initGLFW();
    createVulkanInstance();
    createGLFWWindow();
}

Window::~Window()
{
    glfwDestroyWindow(nativeWindow_);
    glfwTerminate();

    vkDestroySurfaceKHR(instance_, surface_, nullptr);
    vkDestroyInstance(instance_, nullptr);
}

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

}  // namespace mvk
