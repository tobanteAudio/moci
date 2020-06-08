#include <cstdlib>

#include <array>
#include <limits>
#include <vector>

#include "util.hpp"
#include "vk.hpp"

constexpr auto DEFAULT_WIDTH  = 1280;
constexpr auto DEFAULT_HEIGHT = 720;
constexpr auto IMAGE_FORMAT   = VK_FORMAT_B8G8R8A8_UNORM;

class Application
{
public:
    int Startup()
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
        recordCommandBuffers();
        createSemaphores();

        return EXIT_SUCCESS;
    }

    void Run()
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
            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || windowData_.framebufferResized)
            {
                windowData_.framebufferResized = false;
                RecreateSwapChain();
            }
            else if (result != VK_SUCCESS)
            {
                throw std::runtime_error("failed to present swap chain image!");
            }
        }
    }
    void Shutdown()
    {
        VULKAN_CALL(vkDeviceWaitIdle(device));
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

    void RecreateSwapChain()
    {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        VULKAN_CALL(vkDeviceWaitIdle(device));

        destroySwapChain();

        auto const oldSwapChain = swapChain;

        createSwapChain();
        createImageViews();
        createShaders();
        createRenderPass();
        createPipeline();
        createFrameBuffers();
        createCommandPool();
        createCommandBuffers();
        recordCommandBuffers();

        vkDestroySwapchainKHR(device, oldSwapChain, nullptr);
    }

private:
    void destroySwapChain()
    {
        vkFreeCommandBuffers(device, commandPool, commandBuffers.size(), commandBuffers.data());
        vkDestroyCommandPool(device, commandPool, nullptr);

        for (auto const& framebuffer : framebuffers)
        {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        vkDestroyPipeline(device, pipeline, nullptr);
        vkDestroyRenderPass(device, renderPass, nullptr);
        vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        vkDestroyShaderModule(device, vertexShaderModule, nullptr);
        vkDestroyShaderModule(device, fragmentShaderModule, nullptr);

        for (auto const& imageView : imageViews)
        {
            vkDestroyImageView(device, imageView, nullptr);
        }
    }

    void createAppInfo()
    {
        appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pNext              = nullptr;
        appInfo.apiVersion         = VK_API_VERSION_1_2;
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.pApplicationName   = "GLFW with Vulkan";
        appInfo.engineVersion      = VK_MAKE_VERSION(0, 0, 1);
        appInfo.pEngineName        = "GLFW with Vulkan";
    }

    int createGLFWWindow()
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, appInfo.pApplicationName, nullptr, nullptr);

        glfwSetWindowUserPointer(window, this);
        glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
            auto& data               = *static_cast<Application*>(glfwGetWindowUserPointer(window));
            auto surfaceCapabilities = VkSurfaceCapabilitiesKHR {};
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(data.physicalDevices[0], data.surface, &surfaceCapabilities);

            if (width > surfaceCapabilities.maxImageExtent.width) width = surfaceCapabilities.maxImageExtent.width;
            if (height > surfaceCapabilities.maxImageExtent.height) height = surfaceCapabilities.maxImageExtent.height;

            if (width == 0 || height == 0) return;

            data.windowData_.width              = width;
            data.windowData_.height             = height;
            data.windowData_.framebufferResized = true;
        });

        glfwGetFramebufferSize(window, &windowData_.width, &windowData_.height);

        // Create window surface, looks a lot like a Vulkan function ( and not GLFW
        // function ) This is a one function solution for all operating systems. No
        // need to hassle with the OS specifics. For windows this would be
        // vkCreateWin32SurfaceKHR() or on linux XCB window library this would be
        // vkCreateXcbSurfaceKHR()
        if (VK_SUCCESS != glfwCreateWindowSurface(instance, window, nullptr, &surface))
        {
            std::puts("couldn't create surface, exit\n");
            glfwTerminate();
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    int createPhysicalDevices()
    {
        auto physicalDeviceCount = uint32_t {0};
        VULKAN_CALL(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr));

        physicalDevices.resize(physicalDeviceCount);
        VULKAN_CALL(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()));

        for (auto const& device : physicalDevices)
        {
            th::PrintVulkanDeviceStats(device);
        }

        auto queueFamilyCount = uint32_t {0};
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &queueFamilyCount, nullptr);
        auto familyProperties = std::vector<VkQueueFamilyProperties> {};
        familyProperties.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &queueFamilyCount, familyProperties.data());

        for (uint32_t i = 0; i < queueFamilyCount; ++i)
        {
            if (familyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                queueFamilyIdx = i;
            }
        }

        if (queueFamilyIdx == UINT32_MAX)
        {
            // queue family not found
            vkDestroyInstance(instance, nullptr);
            glfwTerminate();
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    void createInstance()
    {
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

        VULKAN_CALL(vkCreateInstance(&instanceInfo, nullptr, &instance));
    }

    void createShaders()
    {
        auto createShader = [&](auto const& binary, VkShaderModule* shaderModule) {
            auto shaderInfo     = VkShaderModuleCreateInfo {};
            shaderInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
            shaderInfo.pNext    = nullptr;
            shaderInfo.flags    = 0;
            shaderInfo.codeSize = binary.size();
            shaderInfo.pCode    = reinterpret_cast<uint32_t const*>(binary.data());

            VULKAN_CALL(vkCreateShaderModule(device, &shaderInfo, nullptr, shaderModule));
        };

        auto vertexShaderBinary = th::ReadFile("playground/glfw_vulkan/shader/vertex.vert.spv");
        createShader(vertexShaderBinary, &vertexShaderModule);
        auto vertexShaderStageInfo                = VkPipelineShaderStageCreateInfo {};
        vertexShaderStageInfo.sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertexShaderStageInfo.pNext               = nullptr;
        vertexShaderStageInfo.flags               = 0;
        vertexShaderStageInfo.stage               = VK_SHADER_STAGE_VERTEX_BIT;
        vertexShaderStageInfo.module              = vertexShaderModule;
        vertexShaderStageInfo.pName               = "main";
        vertexShaderStageInfo.pSpecializationInfo = nullptr;

        auto fragmentShaderBinary = th::ReadFile("playground/glfw_vulkan/shader/fragment.frag.spv");
        createShader(fragmentShaderBinary, &fragmentShaderModule);
        auto fragmentShaderStageInfo                = VkPipelineShaderStageCreateInfo {};
        fragmentShaderStageInfo.sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragmentShaderStageInfo.pNext               = nullptr;
        fragmentShaderStageInfo.flags               = 0;
        fragmentShaderStageInfo.stage               = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragmentShaderStageInfo.module              = fragmentShaderModule;
        fragmentShaderStageInfo.pName               = "main";
        fragmentShaderStageInfo.pSpecializationInfo = nullptr;

        shaderStages[0] = vertexShaderStageInfo;
        shaderStages[1] = fragmentShaderStageInfo;

        vertexInputInfo.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.pNext                           = nullptr;
        vertexInputInfo.flags                           = 0;
        vertexInputInfo.vertexBindingDescriptionCount   = 0;
        vertexInputInfo.pVertexBindingDescriptions      = nullptr;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions    = nullptr;

        inputAssemnblyInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemnblyInfo.pNext                  = nullptr;
        inputAssemnblyInfo.flags                  = 0;
        inputAssemnblyInfo.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssemnblyInfo.primitiveRestartEnable = VK_FALSE;
    }

    void createQueue()
    {
        // Create Vulkan device
        float const priorities[]         = {1.0f};
        auto queueCreateInfo             = VkDeviceQueueCreateInfo {};
        queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueCount       = 1;
        queueCreateInfo.queueFamilyIndex = queueFamilyIdx;
        queueCreateInfo.pQueuePriorities = priorities;

        auto deviceExtensions                    = std::vector<char const*> {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        auto deviceFeature                       = VkPhysicalDeviceFeatures {};
        auto deviceCreateInfo                    = VkDeviceCreateInfo {};
        deviceCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext                   = nullptr;
        deviceCreateInfo.flags                   = 0;
        deviceCreateInfo.queueCreateInfoCount    = 1;
        deviceCreateInfo.pQueueCreateInfos       = &queueCreateInfo;
        deviceCreateInfo.enabledLayerCount       = 0;
        deviceCreateInfo.ppEnabledLayerNames     = nullptr;
        deviceCreateInfo.enabledExtensionCount   = deviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        deviceCreateInfo.pEnabledFeatures        = &deviceFeature;

        VULKAN_CALL(vkCreateDevice(physicalDevices[0], &deviceCreateInfo, nullptr, &device));

        vkGetDeviceQueue(device, queueFamilyIdx, 0, &queue);
    }

    void checkSurfaceSupport()
    {
        auto surfaceSupport = VkBool32 {VK_FALSE};
        VULKAN_CALL(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevices[0], queueFamilyIdx, surface, &surfaceSupport));
    }

    void createSwapChain()
    {
        auto swapChainInfo            = VkSwapchainCreateInfoKHR {};
        swapChainInfo.sType           = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapChainInfo.pNext           = nullptr;
        swapChainInfo.flags           = 0;
        swapChainInfo.surface         = surface;
        swapChainInfo.minImageCount   = 2;                                  // TODO
        swapChainInfo.imageFormat     = IMAGE_FORMAT;                       // TODO
        swapChainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;  // TODO
        swapChainInfo.imageExtent
            = VkExtent2D {static_cast<uint32_t>(windowData_.width), static_cast<uint32_t>(windowData_.height)};
        swapChainInfo.imageArrayLayers      = 1;
        swapChainInfo.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapChainInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;  // TODO
        swapChainInfo.queueFamilyIndexCount = 0;
        swapChainInfo.pQueueFamilyIndices   = nullptr;
        swapChainInfo.preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        swapChainInfo.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapChainInfo.presentMode           = VK_PRESENT_MODE_FIFO_KHR;  // TODO
        swapChainInfo.clipped               = VK_TRUE;
        swapChainInfo.oldSwapchain          = swapChain;

        VULKAN_CALL(vkCreateSwapchainKHR(device, &swapChainInfo, nullptr, &swapChain));

        auto swapChainImageCount = uint32_t {0};
        VULKAN_CALL(vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, nullptr));

        swapChainImages.resize(swapChainImageCount);
        VULKAN_CALL(vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, swapChainImages.data()));
        std::printf("Images in swap chain: %d\n", swapChainImageCount);
    }

    void createImageViews()
    {
        imageViews.resize(swapChainImages.size());
        for (auto i = 0; i < swapChainImages.size(); i++)
        {
            auto imageViewInfo                            = VkImageViewCreateInfo {};
            imageViewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewInfo.pNext                           = nullptr;
            imageViewInfo.flags                           = 0;
            imageViewInfo.image                           = swapChainImages[i];
            imageViewInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
            imageViewInfo.format                          = IMAGE_FORMAT;
            imageViewInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
            imageViewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewInfo.subresourceRange.baseMipLevel   = 0;
            imageViewInfo.subresourceRange.levelCount     = 1;
            imageViewInfo.subresourceRange.baseArrayLayer = 0;
            imageViewInfo.subresourceRange.layerCount     = 1;

            VULKAN_CALL(vkCreateImageView(device, &imageViewInfo, nullptr, &imageViews[i]));
        }
    }

    void createRenderPass()
    {
        auto attachmentDescription           = VkAttachmentDescription {};
        attachmentDescription.flags          = 0;
        attachmentDescription.format         = IMAGE_FORMAT;
        attachmentDescription.samples        = VK_SAMPLE_COUNT_1_BIT;
        attachmentDescription.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentDescription.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        attachmentDescription.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentDescription.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentDescription.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        auto attachmentRef       = VkAttachmentReference {};
        attachmentRef.attachment = 0;
        attachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        auto subpassDescription                    = VkSubpassDescription {};
        subpassDescription.flags                   = 0;
        subpassDescription.pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDescription.inputAttachmentCount    = 0;
        subpassDescription.pInputAttachments       = nullptr;
        subpassDescription.colorAttachmentCount    = 1;
        subpassDescription.pColorAttachments       = &attachmentRef;
        subpassDescription.pResolveAttachments     = nullptr;
        subpassDescription.pDepthStencilAttachment = nullptr;
        subpassDescription.preserveAttachmentCount = 0;
        subpassDescription.pPreserveAttachments    = nullptr;

        auto subpassDependency            = VkSubpassDependency {};
        subpassDependency.srcSubpass      = VK_SUBPASS_EXTERNAL;
        subpassDependency.dstSubpass      = 0;
        subpassDependency.srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.srcAccessMask   = 0;
        subpassDependency.dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency.dependencyFlags = 0;

        auto renderPassInfo            = VkRenderPassCreateInfo {};
        renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.pNext           = nullptr;
        renderPassInfo.flags           = 0;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments    = &attachmentDescription;
        renderPassInfo.subpassCount    = 1;
        renderPassInfo.pSubpasses      = &subpassDescription;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies   = &subpassDependency;

        VULKAN_CALL(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass));
    }

    void createPipeline()
    {
        auto viewport     = VkViewport {};
        viewport.x        = 0.0f;
        viewport.y        = 0.0f;
        viewport.width    = windowData_.width;
        viewport.height   = windowData_.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        auto scissor   = VkRect2D {};
        scissor.offset = {0, 0};
        scissor.extent = {static_cast<uint32_t>(windowData_.width), static_cast<uint32_t>(windowData_.height)};

        auto viewportState          = VkPipelineViewportStateCreateInfo {};
        viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.pNext         = nullptr;
        viewportState.flags         = 0;
        viewportState.viewportCount = 1;
        viewportState.pViewports    = &viewport;
        viewportState.scissorCount  = 1;
        viewportState.pScissors     = &scissor;

        auto rasterizationInfo                    = VkPipelineRasterizationStateCreateInfo {};
        rasterizationInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationInfo.pNext                   = nullptr;
        rasterizationInfo.flags                   = 0;
        rasterizationInfo.depthClampEnable        = VK_FALSE;
        rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizationInfo.polygonMode             = VK_POLYGON_MODE_FILL;
        rasterizationInfo.cullMode                = VK_CULL_MODE_BACK_BIT;
        rasterizationInfo.frontFace               = VK_FRONT_FACE_CLOCKWISE;
        rasterizationInfo.depthBiasEnable         = VK_FALSE;
        rasterizationInfo.depthBiasConstantFactor = 0.0f;
        rasterizationInfo.depthBiasClamp          = 0.0f;
        rasterizationInfo.depthBiasSlopeFactor    = 0.0f;
        rasterizationInfo.lineWidth               = 1.0f;

        auto multisampleInfo                  = VkPipelineMultisampleStateCreateInfo {};
        multisampleInfo.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampleInfo.pNext                 = nullptr;
        multisampleInfo.flags                 = 0;
        multisampleInfo.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
        multisampleInfo.sampleShadingEnable   = VK_FALSE;
        multisampleInfo.minSampleShading      = 1.0f;
        multisampleInfo.pSampleMask           = nullptr;
        multisampleInfo.alphaToCoverageEnable = VK_FALSE;
        multisampleInfo.alphaToOneEnable      = VK_FALSE;

        auto colorBlendAttachment                = VkPipelineColorBlendAttachmentState {};
        colorBlendAttachment.blendEnable         = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp        = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp        = VK_BLEND_OP_ADD;
        colorBlendAttachment.colorWriteMask      =  //
            VK_COLOR_COMPONENT_R_BIT                //
            | VK_COLOR_COMPONENT_G_BIT              //
            | VK_COLOR_COMPONENT_B_BIT              //
            | VK_COLOR_COMPONENT_A_BIT;

        auto colorBlendInfo              = VkPipelineColorBlendStateCreateInfo {};
        colorBlendInfo.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendInfo.pNext             = nullptr;
        colorBlendInfo.flags             = 0;
        colorBlendInfo.logicOpEnable     = VK_FALSE;
        colorBlendInfo.logicOp           = VK_LOGIC_OP_NO_OP;
        colorBlendInfo.attachmentCount   = 1;
        colorBlendInfo.pAttachments      = &colorBlendAttachment;
        colorBlendInfo.blendConstants[0] = 0.0f;
        colorBlendInfo.blendConstants[1] = 0.0f;
        colorBlendInfo.blendConstants[2] = 0.0f;
        colorBlendInfo.blendConstants[3] = 0.0f;

        auto pipelineLayoutInfo                   = VkPipelineLayoutCreateInfo {};
        pipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pNext                  = nullptr;
        pipelineLayoutInfo.flags                  = 0;
        pipelineLayoutInfo.setLayoutCount         = 0;
        pipelineLayoutInfo.pSetLayouts            = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges    = nullptr;

        VULKAN_CALL(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout));

        auto pipelineInfo                = VkGraphicsPipelineCreateInfo {};
        pipelineInfo.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.pNext               = nullptr;
        pipelineInfo.flags               = 0;
        pipelineInfo.stageCount          = shaderStages.size();
        pipelineInfo.pStages             = shaderStages.data();
        pipelineInfo.pVertexInputState   = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssemnblyInfo;
        pipelineInfo.pTessellationState  = nullptr;
        pipelineInfo.pViewportState      = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizationInfo;
        pipelineInfo.pMultisampleState   = &multisampleInfo;
        pipelineInfo.pDepthStencilState  = nullptr;
        pipelineInfo.pColorBlendState    = &colorBlendInfo;
        pipelineInfo.pDynamicState       = nullptr;
        pipelineInfo.layout              = pipelineLayout;
        pipelineInfo.renderPass          = renderPass;
        pipelineInfo.subpass             = 0;
        pipelineInfo.basePipelineHandle  = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex   = -1;

        VULKAN_CALL(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline));
    }

    void createFrameBuffers()
    {
        auto idx = size_t {0};
        framebuffers.resize(imageViews.size());
        for (auto const& image : imageViews)
        {
            auto frameBufferInfo            = VkFramebufferCreateInfo {};
            frameBufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            frameBufferInfo.pNext           = nullptr;
            frameBufferInfo.flags           = 0;
            frameBufferInfo.renderPass      = renderPass;
            frameBufferInfo.attachmentCount = 1;
            frameBufferInfo.pAttachments    = &image;
            frameBufferInfo.width           = windowData_.width;
            frameBufferInfo.height          = windowData_.height;
            frameBufferInfo.layers          = 1;

            VULKAN_CALL(vkCreateFramebuffer(device, &frameBufferInfo, nullptr, &framebuffers[idx]));
            idx++;
        }
    }

    void createCommandPool()
    {
        auto commandPoolInfo             = VkCommandPoolCreateInfo {};
        commandPoolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolInfo.pNext            = nullptr;
        commandPoolInfo.flags            = 0;
        commandPoolInfo.queueFamilyIndex = queueFamilyIdx;

        VULKAN_CALL(vkCreateCommandPool(device, &commandPoolInfo, nullptr, &commandPool));
    }

    void createCommandBuffers()
    {
        auto commandBufferAllocInfo               = VkCommandBufferAllocateInfo {};
        commandBufferAllocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        commandBufferAllocInfo.pNext              = nullptr;
        commandBufferAllocInfo.commandPool        = commandPool;
        commandBufferAllocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        commandBufferAllocInfo.commandBufferCount = swapChainImages.size();

        commandBuffers.resize(swapChainImages.size());
        VULKAN_CALL(vkAllocateCommandBuffers(device, &commandBufferAllocInfo, commandBuffers.data()));

        auto commandBufferBeginInfo             = VkCommandBufferBeginInfo {};
        commandBufferBeginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.pNext            = nullptr;
        commandBufferBeginInfo.flags            = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;
    }
    void recordCommandBuffers()
    {
        auto commandBufferBeginInfo             = VkCommandBufferBeginInfo {};
        commandBufferBeginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.pNext            = nullptr;
        commandBufferBeginInfo.flags            = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;

        auto idx = size_t {0};
        for (auto& commandBuffer : commandBuffers)
        {
            VULKAN_CALL(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo));

            auto clearColor                       = VkClearValue {0.1f, 0.1f, 0.1f, 1.0f};
            auto renderPassBeginInfo              = VkRenderPassBeginInfo {};
            renderPassBeginInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassBeginInfo.pNext             = nullptr;
            renderPassBeginInfo.renderPass        = renderPass;
            renderPassBeginInfo.framebuffer       = framebuffers[idx++];
            renderPassBeginInfo.renderArea.offset = {0, 0};
            renderPassBeginInfo.renderArea.extent
                = {static_cast<uint32_t>(windowData_.width), static_cast<uint32_t>(windowData_.height)};
            renderPassBeginInfo.clearValueCount = 1;
            renderPassBeginInfo.pClearValues    = &clearColor;

            vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
            vkCmdDraw(commandBuffer, 3, 1, 0, 0);
            vkCmdEndRenderPass(commandBuffer);

            VULKAN_CALL(vkEndCommandBuffer(commandBuffer));
        }
    }

    void createSemaphores()
    {
        auto semaphoreInfo  = VkSemaphoreCreateInfo {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreInfo.pNext = nullptr;
        semaphoreInfo.flags = 0;

        VULKAN_CALL(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphoreImageAvailable));
        VULKAN_CALL(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &semaphoreRenderDone));
    }

    void printLayers()
    {
        auto layerCount = uint32_t {0};
        VULKAN_CALL(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));
        auto layers = std::vector<VkLayerProperties> {};
        layers.resize(layerCount);
        VULKAN_CALL(vkEnumerateInstanceLayerProperties(&layerCount, layers.data()));
        std::printf("Instance layer count: %d\n", layerCount);
        for (auto const& layer : layers)
        {
            std::printf("%s\n", layer.layerName);
        }
        std::puts("\n");
    }

    void printExtensions()
    {
        auto extensionCount = uint32_t {0};
        VULKAN_CALL(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr));
        auto extensions = std::vector<VkExtensionProperties> {};
        extensions.resize(extensionCount);
        VULKAN_CALL(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data()));
        std::printf("Extension count: %d\n", extensionCount);
        for (auto const& extension : extensions)
        {
            std::printf("%s\n", extension.extensionName);
        }
        std::puts("\n");
    }

public:
    struct WindowData
    {
        int width               = 0;
        int height              = 0;
        bool framebufferResized = false;
    };

    WindowData windowData_                        = {};
    std::vector<VkPhysicalDevice> physicalDevices = {};

private:
    GLFWwindow* window                                          = nullptr;
    VkSurfaceKHR surface                                        = VK_NULL_HANDLE;
    VkApplicationInfo appInfo                                   = {};
    VkInstance instance                                         = VK_NULL_HANDLE;
    uint32_t queueFamilyIdx                                     = UINT32_MAX;
    VkDevice device                                             = VK_NULL_HANDLE;
    VkQueue queue                                               = {};
    std::vector<VkImage> swapChainImages                        = {};
    VkSwapchainKHR swapChain                                    = VK_NULL_HANDLE;
    std::vector<VkImageView> imageViews                         = {};
    VkRenderPass renderPass                                     = VK_NULL_HANDLE;
    VkPipeline pipeline                                         = VK_NULL_HANDLE;
    std::vector<VkFramebuffer> framebuffers                     = {};
    VkCommandPool commandPool                                   = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commandBuffers                 = {};
    VkPipelineLayout pipelineLayout                             = VK_NULL_HANDLE;
    VkSemaphore semaphoreImageAvailable                         = VK_NULL_HANDLE;
    VkSemaphore semaphoreRenderDone                             = VK_NULL_HANDLE;
    VkShaderModule vertexShaderModule                           = VK_NULL_HANDLE;
    VkShaderModule fragmentShaderModule                         = VK_NULL_HANDLE;
    VkPipelineVertexInputStateCreateInfo vertexInputInfo        = {};
    VkPipelineInputAssemblyStateCreateInfo inputAssemnblyInfo   = {};
    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {};
};

int main()
{
    Application app {};
    if (auto err = app.Startup(); err != EXIT_SUCCESS)
    {
        std::printf("Error in startup: %d\n", err);
    }

    app.Run();
    app.Shutdown();

    return EXIT_SUCCESS;
}