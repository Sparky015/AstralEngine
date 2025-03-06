/**
* @file VulkanRenderingContext.cpp
* @author Andrew Fagan
* @date 3/4/2025
*/

#include "VulkanRenderingContext.h"

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
        m_Window(window)
    {}


    void VulkanRenderingContext::Init()
    {
        PROFILE_SCOPE("Vulkan Rendering Context Initialization");

        if (!glfwVulkanSupported()) { ASTRAL_ERROR("GLFW can't find Vulkan!"); }

        CreateInstance();
        CreateDebugMessageCallback();
        CreateWindowSurface();
    }


    void VulkanRenderingContext::Shutdown()
    {
        PROFILE_SCOPE("Vulkan Rendering Context Shutdown");

        DestroyWindowSurface();
        DestroyDebugMessageCallback();
        DestroyInstance();
    }


    std::string_view VulkanRenderingContext::GetGraphicsProcessorName()
    {
        return "";
    }


    std::string_view VulkanRenderingContext::GetGPUVendor()
    {
        return "";
    }


    std::string_view VulkanRenderingContext::GetRenderingAPI()
    {
        return "";
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

