/**
* @file VulkanRendererContext.h
* @author Andrew Fagan
* @date 3/4/2025
*/

#pragma once

#include "Renderer/RHI/RendererContext.h"

#include "Resources/VulkanPhysicalDevices.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "Renderer/RHI/Resources/Swapchain.h"
#include "Renderer/RHI/Resources/Device.h"
#include "Resources/VulkanDevice.h"

namespace Astral {

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

        Device& GetDevice() override { return *m_Device; }

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

        GLFWwindow* m_Window;
        VkSurfaceKHR m_WindowSurface;

        VulkanPhysicalDevices m_PhysicalDevices;
        uint32 m_QueueFamilyIndex;

        GraphicsOwnedPtr<Device> m_Device;
    };

}
