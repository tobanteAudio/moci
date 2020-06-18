#pragma once

#include "vk/vk.hpp"

namespace mvk
{
class Window
{
public:
    explicit Window();
    ~Window();

private:
    constexpr static std::string_view APP_NAME = "Moci: Vulkan";

    void initGLFW();
    void createGLFWWindow();
    void createVulkanInstance();

    // Native window handle
    GLFWwindow* nativeWindow_ = nullptr;

    // Vulkan instance, stores all per-application states
    VkInstance instance_ = VK_NULL_HANDLE;

    VkSurfaceKHR surface_ = VK_NULL_HANDLE;
};
}  // namespace mvk