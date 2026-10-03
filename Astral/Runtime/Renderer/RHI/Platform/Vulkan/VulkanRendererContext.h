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

    /**
     * @brief Holds components needed for the lifetime of the Vulkan renderer such as the instance, physical devices, window surface,
     *        debug callback hook, and logical device.
     */
    class VulkanRenderingContext : public RenderingContext
    {
    public:
        explicit VulkanRenderingContext(GLFWwindow* window);
        ~VulkanRenderingContext() override = default;

        /**
         * @brief Initializes the Vulkan rendering context
         */
        void Init() override;

        /**
         * @brief Shuts down the Vulkan rendering context
         */
        void Shutdown() override;

        /**
         * @brief Gets the graphics device
         * @return The graphics device
         */
        Device& GetDevice() override { return *m_Device; }

        /**
         * @brief Gets the pipeline state cache
         * @return The pipeline state cache
         */
        PipelineStateCache& GetPipelineStateCache() override;

        /**
         * @brief Gets the number of validation errors and warnings triggered
         * @return The number of validation errors and warnings triggered
         */
        uint32 GetNumValidationErrorsAndWarnings() override { return m_NumValidationErrorsAndWarnings; }

        /**
         * @brief Clears the counter of the number of validation errors and warnings triggered
         */
        void ClearNumValidationErrorsAndWarnings() override { m_NumValidationErrorsAndWarnings = 0; }

        /**
         * @brief Gets the native instance handle of the graphics API context
         * @return The native instance handle of the graphics API context
         */
        void* GetInstanceHandle() override { return m_Instance; }

        /**
         * @brief Initializes ImGui rendering backend
         */
        void InitImGuiForAPIBackend(RenderPassHandle renderPassHandle) override;

        /**
         * @brief Initializes a new frame for ImGui backend
         */
        void MarkNewImGuiFrame() override;

        /**
         * @brief Shuts down the ImGui rendering backend
         */
        void ShutdownImGuiForAPIBackend() override;

        /**
         * @brief Gets the primary command queue
         */
        CommandQueueHandle GetPrimaryCommandQueue();

        /**
         * @brief Gets the async command queue
         */
        CommandQueueHandle GetAsyncCommandQueue();

        /**
         * @brief Gets the command pool for the executing thread
         */
        VkCommandPool GetThreadCommandPool();

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

        /**
         * @brief Creates the primary command queue RHI handle
         */
        void CreateCommandQueues();

        /**
         * @brief Releases the primary command queue RHI handle
         */
        void DestroyCommandQueues();

        void ReleaseAllThreadCommandPools();

        VkInstance m_Instance;
        VkDebugUtilsMessengerEXT m_DebugMessenger;

        GLFWwindow* m_Window;
        VkSurfaceKHR m_WindowSurface;

        VulkanPhysicalDevices m_PhysicalDevices;
        uint32 m_QueueFamilyIndex;

        static uint32 m_NumValidationErrorsAndWarnings;

        GraphicsOwnedPtr<Device> m_Device;
        GraphicsOwnedPtr<PipelineStateCache> m_PipelineStateCache;

        CommandQueueHandle m_PrimaryCommandQueue;
        CommandQueueHandle m_AsyncCommandQueue;

        std::unordered_map<std::thread::id, VkCommandPool> m_ThreadCommandPools;
        std::mutex m_CommandPoolsMutex;
    };

}
