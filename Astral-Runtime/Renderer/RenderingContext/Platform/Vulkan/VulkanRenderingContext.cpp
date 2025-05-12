/**
* @file VulkanRenderingContext.cpp
* @author Andrew Fagan
* @date 3/4/2025
*/

#include "VulkanRenderingContext.h"

#include <array>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "Core/CoreMacroDefinitions.h"
#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Error.h"
#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Loggers.h"


namespace Graphics {

    VulkanRenderingContext::VulkanRenderingContext(GLFWwindow* window) :
        m_Instance(VK_NULL_HANDLE),
        m_DebugMessenger(VK_NULL_HANDLE),
        m_WindowSurface(VK_NULL_HANDLE),
        m_Window(window),
        m_QueueFamilyIndex(-1), // Wraps around to max uint32 to start with an invalid index
        m_Device(VK_NULL_HANDLE),
        m_Swapchain(VK_NULL_HANDLE),
        m_CommandPool(VK_NULL_HANDLE),
        m_NumberOfSwapchainImages(0)
    {
    }


    void VulkanRenderingContext::Init()
    {
        PROFILE_SCOPE("Vulkan Rendering Context Initialization");

        if (!glfwVulkanSupported()) { ASTRAL_ERROR("GLFW can't find Vulkan!"); }

        CreateInstance();
        CreateDebugMessageCallback();
        CreateWindowSurface();
        m_PhysicalDevices.Init(m_Instance, m_WindowSurface);
        m_QueueFamilyIndex = m_PhysicalDevices.SelectDevice(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT, true);
        CreateDevice();
        CreateSwapchain();
        CreateCommandBufferPool();

        // TEMP
        m_CommandBuffers.resize(m_NumberOfSwapchainImages);
        CreateCommandBuffers(m_CommandBuffers.size(), m_CommandBuffers.data());
    }


    void VulkanRenderingContext::Shutdown()
    {
        PROFILE_SCOPE("Vulkan Rendering Context Shutdown");

        DestroyCommandBufferPool();
        DestroySwapchain();
        DestroyDevice();
        DestroyWindowSurface();
        DestroyDebugMessageCallback();
        DestroyInstance();
    }


    std::string_view VulkanRenderingContext::GetGraphicsProcessorName()
    {
        return m_PhysicalDevices.SelectedDevice().deviceProperties.deviceName;
    }


    std::string_view VulkanRenderingContext::GetGPUVendor()
    {
        return "";//m_PhysicalDevices.SelectedDevice().deviceProperties.vendorID;
    }


    std::string_view VulkanRenderingContext::GetRenderingAPI()
    {
        static uint32 vulkanAPIVersion = m_PhysicalDevices.SelectedDevice().deviceProperties.apiVersion;
        static char buffer[39] = "Vulkan ";
        snprintf(buffer + 7, sizeof(buffer) - 7, "%d.%d.%d", VK_API_VERSION_MAJOR(vulkanAPIVersion),
                                                                       VK_API_VERSION_MINOR(vulkanAPIVersion),
                                                                       VK_API_VERSION_PATCH(vulkanAPIVersion));
        return buffer;
    }


    void VulkanRenderingContext::CreateInstance()
    {
        PROFILE_SCOPE("Creating Vulkan Instance")

        std::array<const char*, 1> layers = {
            "VK_LAYER_KHRONOS_validation"
        };

        std::array<const char*, 4> extensions = {
            VK_KHR_SURFACE_EXTENSION_NAME,
#if ASTRAL_PLATFORM_WINDOWS
            "VK_KHR_win32_surface",
#elif ASTRAL_PLATFORM_MACOS
            "VK_EXT_metal_surface",
#elif ASTRAL_PLATFORM_LINUX
            "VK_KHR_xcb_surface",
#endif
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
            VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
        };

        VkApplicationInfo appInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "VulkanPlayground",
            .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
            .pEngineName = "Astral Engine",
            .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
            .apiVersion = VK_API_VERSION_1_0
        };

        VkInstanceCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
            .pApplicationInfo = &appInfo,
            .enabledLayerCount = (uint32)layers.size(),
            .ppEnabledLayerNames = layers.data(),
            .enabledExtensionCount = (uint32)extensions.size(),
            .ppEnabledExtensionNames = extensions.data()
        };

        VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
        ASSERT(result == VK_SUCCESS, "Vulkan failed to create the instance! Error code: " << result);
    }


    void VulkanRenderingContext::DestroyInstance()
    {
        vkDestroyInstance(m_Instance, nullptr);
    }


    void VulkanRenderingContext::CreateDebugMessageCallback()
    {
        VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext = nullptr,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = &VulkanRenderingContext::DebugCallback,
            .pUserData = nullptr
        };

        PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessenger = VK_NULL_HANDLE;
        vkCreateDebugUtilsMessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
        ASSERT(vkCreateDebugUtilsMessenger, "Vulkan failed to find address of 'vkCreateDebugUtilsMessengerEXT'")

        VkResult result = vkCreateDebugUtilsMessenger(m_Instance, &messengerCreateInfo, nullptr, &m_DebugMessenger);
        ASSERT(result == VK_SUCCESS, "Vulkan failed to create the DebugUtilsMessenger")
    }


    void VulkanRenderingContext::DestroyDebugMessageCallback()
    {
        PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = VK_NULL_HANDLE;
        vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
        ASSERT(vkDestroyDebugUtilsMessengerEXT, "Vulkan failed to find address of 'vkDestroyDebugUtilsMessengerEXT'")

        vkDestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
    }


    void VulkanRenderingContext::CreateWindowSurface()
    {
        VkResult result = glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &m_WindowSurface);
        ASSERT(result == VK_SUCCESS, "Vulkan Rendering Context: Failed to create window surface! Error Code: " << result);
    }


    void VulkanRenderingContext::DestroyWindowSurface()
    {
        PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = VK_NULL_HANDLE;
        vkDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)vkGetInstanceProcAddr(m_Instance, "vkDestroySurfaceKHR");
        ASSERT(vkDestroySurfaceKHR, "Vulkan failed to find address of function 'vkDestroySurfaceKHR'")

        vkDestroySurfaceKHR(m_Instance, m_WindowSurface, nullptr);
    }


    void VulkanRenderingContext::CreateDevice()
    {
        float priorities[] = { 1.0f };

        VkDeviceQueueCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = m_QueueFamilyIndex,
            .queueCount = 1,
            .pQueuePriorities = &priorities[0]
        };

        std::vector<const char*> devExts = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME
        };

        VkPhysicalDeviceFeatures deviceFeatures = { 0 };

        if (m_PhysicalDevices.SelectedDevice().features.geometryShader == VK_FALSE)
            { LOG("Vulkan: Geometry Shader is not supported!") }
        else
            { deviceFeatures.geometryShader = VK_TRUE; }

        if (m_PhysicalDevices.SelectedDevice().features.tessellationShader == VK_FALSE)
            { LOG("Vulkan: Tessellation Shader is not supported!") }
        else
            { deviceFeatures.tessellationShader = VK_TRUE; }

        VkDeviceCreateInfo deviceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &info,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = (uint32)devExts.size(),
            .ppEnabledExtensionNames = devExts.data(),
            .pEnabledFeatures = &deviceFeatures
        };

        VkResult result = vkCreateDevice(m_PhysicalDevices.SelectedDevice().physicalDevice, &deviceCreateInfo, nullptr, &m_Device);
        ASSERT(result == VK_SUCCESS, "Vulkan device failed on creation!");
    }


    void VulkanRenderingContext::DestroyDevice()
    {
        vkDestroyDevice(m_Device, nullptr);
    }


    uint32 ChooseNumSwapchainImages(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        uint32 requestedNumberOfImage = capabilities.minImageCount + 1;
        uint32 finalNumberOfImages = 0;

        if (capabilities.maxImageCount > 0 && requestedNumberOfImage > capabilities.maxImageCount)
            { finalNumberOfImages = capabilities.maxImageCount; }
        else
            { finalNumberOfImages = requestedNumberOfImage; }

        return finalNumberOfImages;
    }


    VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes)
    {
        for (const VkPresentModeKHR& mode : presentModes)
        {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return mode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR; // Always supported
    }


    VkSurfaceFormatKHR ChooseSurfaceFormatAndColorSpace(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        for (const VkSurfaceFormatKHR& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                {
                    return availableFormat;
                }
        }

        return availableFormats[0];
    }

    VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
                                VkImageViewType viewType, uint32 layerCount, uint32 mipLevels)
    {
        VkImageViewCreateInfo viewInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .image = image,
            .viewType = viewType,
            .format = format,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask = aspectFlags,
                .baseMipLevel = 0,
                .levelCount = mipLevels,
                .baseArrayLayer = 0,
                .layerCount = 1,
            }
        };

        VkImageView imageView;
        VkResult result = vkCreateImageView(device, &viewInfo, nullptr, &imageView);
        ASSERT(result == VK_SUCCESS, "Vulkan failed to create image view!");
        return imageView;
    }


    void VulkanRenderingContext::CreateSwapchain()
    {
        const VkSurfaceCapabilitiesKHR surfaceCapabilities = m_PhysicalDevices.SelectedDevice().surfaceCapabilities;

        m_NumberOfSwapchainImages = ChooseNumSwapchainImages(surfaceCapabilities);


        const std::vector<VkPresentModeKHR>& presentModes = m_PhysicalDevices.SelectedDevice().presentModes;

        VkPresentModeKHR presentMode = ChoosePresentMode(presentModes);
        VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormatAndColorSpace(m_PhysicalDevices.SelectedDevice().surfaceFormats);

        VkSwapchainCreateInfoKHR swapChainCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .surface = m_WindowSurface,
            .minImageCount = m_NumberOfSwapchainImages,
            .imageFormat = surfaceFormat.format,
            .imageColorSpace = surfaceFormat.colorSpace,
            .imageExtent = surfaceCapabilities.currentExtent,
            .imageArrayLayers = 1,
            .imageUsage = (VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT),
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = &m_QueueFamilyIndex,
            .preTransform = surfaceCapabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = presentMode,
            .clipped = VK_TRUE
        };

        VkResult result = vkCreateSwapchainKHR(m_Device, &swapChainCreateInfo, nullptr, &m_Swapchain);
        ASSERT(result == VK_SUCCESS, "Vulkan swapchain failed to create!");

        uint32 numberOfSwapchainImages = 0;
        result = vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &numberOfSwapchainImages, nullptr);
        ASSERT(result == VK_SUCCESS, "Vulkan swapchain failed to get number of images!");

        m_Images.resize(numberOfSwapchainImages);
        m_ImageViews.resize(numberOfSwapchainImages);

        result = vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &numberOfSwapchainImages, m_Images.data());
        ASSERT(result == VK_SUCCESS, "Vulkan swapchain failed to get images!");

        int32 layerCount = 1;
        int mipLevels = 1;

        for (uint32 i = 0; i < numberOfSwapchainImages; i++)
        {
            m_ImageViews[i] = CreateImageView(m_Device, m_Images[i], surfaceFormat.format,
                            VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_2D, layerCount, mipLevels);
        }

    }


    void VulkanRenderingContext::DestroySwapchain()
    {
        for (const VkImageView& imageView : m_ImageViews)
        {
            vkDestroyImageView(m_Device, imageView, nullptr);
        }

        vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
    }


    void VulkanRenderingContext::CreateCommandBufferPool()
    {
        VkCommandPoolCreateInfo commandPoolCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = m_QueueFamilyIndex
        };

        VkResult result = vkCreateCommandPool(m_Device, &commandPoolCreateInfo, nullptr, &m_CommandPool);
        ASSERT(result == VK_SUCCESS, "Vulkan command pool failed to create!");
    }


    void VulkanRenderingContext::DestroyCommandBufferPool()
    {
        vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
    }


    void VulkanRenderingContext::CreateCommandBuffers(uint32 count, VkCommandBuffer* commandBuffers)
    {
        VkCommandBufferAllocateInfo commandBufferAllocate = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = m_CommandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = count,
        };

        VkResult result = vkAllocateCommandBuffers(m_Device, &commandBufferAllocate, commandBuffers);
        ASSERT(result == VK_SUCCESS, "Vulkan command buffer(s) failed to allocate!");
    }


    void VulkanRenderingContext::FreeCommandBuffers(uint32 count, VkCommandBuffer* commandBuffers)
    {
        for (uint32 i = 0; i < count; i++)
        {
            vkFreeCommandBuffers(m_Device, m_CommandPool, count, commandBuffers);
        }
    }


    const char* GetDebugMessageSeverity(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverityFlag)
    {
        switch (messageSeverityFlag)
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: return "Verbose";
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: return "Info";
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: return "Warning";
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: return "Error";
            default: ASTRAL_ERROR("Invalid severity code: " << messageSeverityFlag);
        }
    }


    const char* GetDebugMessageType(VkDebugUtilsMessageTypeFlagsEXT messageTypeFlag)
    {
        switch (messageTypeFlag)
        {
            case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: return "General";
            case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: return "Validation";
            case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: return "Performance";
            default: ASTRAL_ERROR("Invalid flag type: " << messageTypeFlag);
        }
    }


    VkBool32 VulkanRenderingContext::DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        TRACE("Vulkan Debug Message:");
        LOG(pCallbackData->pMessage);
        LOG("   Severity: " << GetDebugMessageSeverity(severity));
        LOG("   Type: " << GetDebugMessageType(type));
        LOG("   Objects:");

        for (uint32 i = 0; i < pCallbackData->objectCount; i++)
        {
            LOG("     " << pCallbackData->pObjects[i].objectHandle);
        }


        return VK_FALSE;
    }

}

