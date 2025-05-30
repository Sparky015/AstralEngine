/**
* @file VulkanRendererContext.cpp
* @author Andrew Fagan
* @date 3/4/2025
*/

#include "VulkanRendererContext.h"

#include "Core/CoreMacroDefinitions.h"
#include "Debug/Instrumentation/ScopeProfiler.h"
#include "Debug/Utilities/Error.h"
#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Loggers.h"

#ifdef ASTRAL_VULKAN_AVAILABLE
#endif
#include "Debug/ImGui/ImGuiDependencies/imgui_impl_vulkan.h"

#include <array>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace Astral {

    VulkanRenderingContext::VulkanRenderingContext(GLFWwindow* window) :
        m_Instance(VK_NULL_HANDLE),
        m_DebugMessenger(VK_NULL_HANDLE),
        m_Window(window),
        m_WindowSurface(VK_NULL_HANDLE),
        m_QueueFamilyIndex(-1), // Wraps around to max uint32 to start with an invalid index
        m_Device(VK_NULL_HANDLE)
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
        m_PhysicalDevices.SelectDevice(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT, true);
        m_QueueFamilyIndex = m_PhysicalDevices.SelectedQueueFamily();

        CreateDevice();
    }


    void VulkanRenderingContext::Shutdown()
    {
        PROFILE_SCOPE("Vulkan Rendering Context Shutdown");

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
        switch (m_PhysicalDevices.SelectedDevice().deviceProperties.vendorID)
        {
            case 0x1002: return "AMD";
            case 0x10DE: return "NVIDIA";
            case 0x106B: return "Apple";
            case 0x8086: return "Intel";
            case 0x13B5: return "ARM";
            case 0x5143: return "Imagination Technologies";
            case 0x1AD0: return "Google";
            case 0x1AE0: return "Samsung";
            case 0x1217: return "Qualcomm";
            default: return "Unknown";
        }

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

        std::vector<const char*> extensions = {
            VK_KHR_SURFACE_EXTENSION_NAME,
#if ASTRAL_PLATFORM_WINDOWS
            "VK_KHR_win32_surface",
#elif ASTRAL_PLATFORM_MACOS
            "VK_EXT_metal_surface",
            VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
#elif ASTRAL_PLATFORM_LINUX
            "VK_KHR_xcb_surface",
#endif
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        };

        VkApplicationInfo appInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "VulkanPlayground",
            .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
            .pEngineName = "Astral Engine",
            .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
            .apiVersion = VK_API_VERSION_1_2
        };

        VkInstanceCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0, // TODO: VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR for MacOS
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
        VulkanDeviceDesc desc = {
            .PhysicalDevice = m_PhysicalDevices.SelectedDevice(),
            .QueueFamilyIndex = m_QueueFamilyIndex,
            .WindowSurface = m_WindowSurface,
            .Window = m_Window,
        };

        m_Device = CreateGraphicsOwnedPtr<VulkanDevice>(desc);
    }


    void VulkanRenderingContext::DestroyDevice()
    {
        m_Device.reset();
    }


    void VulkanRenderingContext::InitImGuiForAPIBackend(RenderPassHandle renderPassHandle)
    {
        VkRenderPass renderPass = (VkRenderPass)renderPassHandle->GetNativeHandle();

        ImGui_ImplVulkan_InitInfo initInfo =
        {
            .Instance = m_Instance,
            .PhysicalDevice = m_PhysicalDevices.SelectedDevice().physicalDevice,
            .Device = (VkDevice)m_Device->GetNativeHandle(),
            .QueueFamily = m_QueueFamilyIndex,
            .Queue = (VkQueue)m_Device->GetCommandQueue()->GetNativeHandle(),
            .RenderPass = renderPass,
            .MinImageCount = m_Device->GetSwapchain().GetNumberOfImages(),
            .ImageCount = m_Device->GetSwapchain().GetNumberOfImages(),
            .MSAASamples = VK_SAMPLE_COUNT_1_BIT,
            .DescriptorPoolSize = 9,
            .MinAllocationSize = 1024 * 1024,
        };

        ImGui_ImplVulkan_Init(&initInfo);
        ImGui_ImplVulkan_CreateFontsTexture();
    }


    void VulkanRenderingContext::ShutdownImGuiForAPIBackend()
    {
        ImGui_ImplVulkan_DestroyFontsTexture();
        ImGui_ImplVulkan_Shutdown();
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

