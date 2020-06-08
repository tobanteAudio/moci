#include "application.hpp"

namespace th
{
int Application::Startup()
{
    if (glfwInit() == GLFW_FALSE)
    {
        return EXIT_FAILURE;
    }

    if (glfwVulkanSupported() == GLFW_FALSE)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    createAppInfo();
    printLayers();
    printExtensions();
    createInstance();

    if (auto err = createGLFWWindow(); err != EXIT_SUCCESS)
    {
        return err;
    }

    if (auto err = createPhysicalDevices(); err != EXIT_SUCCESS)
    {
        return err;
    }

    createQueue();
    checkSurfaceSupport();
    createSwapChain();
    createImageViews();
    createShaders();
    createRenderPass();
    createPipeline();
    createFrameBuffers();
    createCommandPool();
    createCommandBuffers();
    createVertexBuffer();
    recordCommandBuffers();
    createSemaphores();

    return EXIT_SUCCESS;
}

void Application::Run()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        auto imageIndex = uint32_t {0};
        VULKAN_CALL(vkAcquireNextImageKHR(device, swapChain, std::numeric_limits<uint64_t>::max(),
                                          semaphoreImageAvailable, VK_NULL_HANDLE, &imageIndex));

        auto waitStageMask = std::array<VkPipelineStageFlags, 1> {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        auto submitInfo    = VkSubmitInfo {};
        submitInfo.sType   = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext   = nullptr;
        submitInfo.waitSemaphoreCount   = 1;
        submitInfo.pWaitSemaphores      = &semaphoreImageAvailable;
        submitInfo.pWaitDstStageMask    = waitStageMask.data();
        submitInfo.commandBufferCount   = 1;
        submitInfo.pCommandBuffers      = &commandBuffers.at(imageIndex);
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores    = &semaphoreRenderDone;

        VULKAN_CALL(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));

        auto presentInfo               = VkPresentInfoKHR {};
        presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext              = nullptr;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores    = &semaphoreRenderDone;
        presentInfo.swapchainCount     = 1;
        presentInfo.pSwapchains        = &swapChain;
        presentInfo.pImageIndices      = &imageIndex;
        presentInfo.pResults           = nullptr;

        auto result = vkQueuePresentKHR(queue, &presentInfo);
        // if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || windowData_.framebufferResized)
        // {
        //     windowData_.framebufferResized = false;
        //     RecreateSwapChain();
        // }
        // else if (result != VK_SUCCESS)
        // {
        //     throw std::runtime_error("failed to present swap chain image!");
        // }
    }
}
void Application::Shutdown()
{
    VULKAN_CALL(vkDeviceWaitIdle(device));

    vkFreeMemory(device, vertexBufferMemory_, nullptr);
    vkDestroyBuffer(device, vertexBuffer_, nullptr);
    vkDestroySemaphore(device, semaphoreImageAvailable, nullptr);
    vkDestroySemaphore(device, semaphoreRenderDone, nullptr);
    destroySwapChain();
    vkDestroySwapchainKHR(device, swapChain, nullptr);
    vkDestroyDevice(device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::RecreateSwapChain()
{
    int width  = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }

    VULKAN_CALL(vkDeviceWaitIdle(device));

    destroySwapChain();

    auto* const oldSwapChain = swapChain;

    auto surfaceCapabilities = VkSurfaceCapabilitiesKHR {};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevices[0], surface, &surfaceCapabilities);

    if (width > surfaceCapabilities.maxImageExtent.width)
    {
        width = surfaceCapabilities.maxImageExtent.width;
    }
    if (height > surfaceCapabilities.maxImageExtent.height)
    {
        height = surfaceCapabilities.maxImageExtent.height;
    }

    createSwapChain();
    createImageViews();
    createShaders();
    createRenderPass();
    createPipeline();
    createFrameBuffers();
    createCommandPool();
    createVertexBuffer();
    createCommandBuffers();
    recordCommandBuffers();

    vkDestroySwapchainKHR(device, oldSwapChain, nullptr);
}
}  // namespace th