/**
* @file VulkanRenderingContext.h
* @author Andrew Fagan
* @date 3/4/2025
*/

#pragma once

#include "Renderer/RenderingContext/RenderingContext.h"

#include "VulkanPhysicalDevices.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace Graphics {

    /*
    * This sets up the rendering context for when Vulkan is used. It defines functions that are overridden from
    * RendererContext and also needs a window from the window solution to set the context to.
    */
    class VulkanRenderingContext : public RenderingContext
    {
    public:
        explicit VulkanRenderingContext(GLFWwindow* window);

        void Init() override;
        void Shutdown() override;

        std::string_view GetGraphicsProcessorName() override;
        std::string_view GetGPUVendor() override;
        std::string_view GetRenderingAPI() override;

    private:

        void CreateInstance();
        void DestroyInstance();

        void CreateDebugMessageCallback();
        void DestroyDebugMessageCallback();

        void CreateWindowSurface();
        void DestroyWindowSurface();

        void CreateDevice();
        void DestroyDevice();

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
                VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                VkDebugUtilsMessageTypeFlagsEXT type,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData);

        VkInstance m_Instance;
        VkDebugUtilsMessengerEXT m_DebugMessenger;
        VkSurfaceKHR m_WindowSurface;
        VulkanPhysicalDevices m_PhysicalDevices;
        GLFWwindow* m_Window;
        uint32 m_QueueFamilyIndex;
        VkDevice m_Device;
    };

}