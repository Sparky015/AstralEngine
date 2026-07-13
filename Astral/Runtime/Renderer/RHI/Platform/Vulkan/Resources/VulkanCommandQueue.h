/**
* @file VulkanCommandQueue.h
* @author Andrew Fagan
* @date 5/14/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Resources/CommandQueue.h"

#include <vulkan/vulkan_core.h>

#include "Renderer/RHI/Resources/Swapchain.h"

namespace Astral {

    /**
    * @brief Information to create a Vulkan command queue from
    */
    struct VulkanCommandQueueDesc
    {
        VkDevice Device;
        Swapchain& Swapchain;
        uint32 QueueFamilyIndex;
        uint32 QueueIndex;
    };

    /**
    * @brief Wraps Vulkan command queue operations
    */
    class VulkanCommandQueue : public CommandQueue
    {
    public:
        explicit VulkanCommandQueue(const VulkanCommandQueueDesc& desc);
        ~VulkanCommandQueue() override;

        /**
         * @brief Submits a command buffer to a command queue
         * @param commandBufferHandle The command buffer to submit to the command queue
         * @param renderTargetHandle The render target with a fence and semaphores to submit with the command buffer for the frame
         */
        void Submit(CommandBufferHandle commandBufferHandle, RenderTargetHandle renderTargetHandle) override;

        /**
         * @brief Submits a command buffer to a command queue without a fence or semaphore to signal
         * @param commandBufferHandle The command buffer to submit to the command queue
         */
        void SubmitSync(CommandBufferHandle commandBufferHandle) override;

        /**
         * @brief Presents a render target to the screen
         * @param renderTarget The render target to present to the screen
         */
        void Present(RenderTargetHandle renderTarget) override;

        /**
        * @brief Blocks the current CPU thread until the queue's work is finished on the GPU
        */
        void WaitIdle() override;

        /**
         * @brief Gets the native handle of the command queue
         * @return The native handle of the command queue
         */
        void* GetNativeHandle() override { return m_Queue; }

    private:

        /**
         * @brief Retrieves the vulkan queue object and caches it
         */
        void GetQueue();

        VkDevice m_Device;
        Swapchain& m_Swapchain;
        uint32 m_QueueFamilyIndex;
        uint32 m_QueueIndex;

        VkQueue m_Queue;
    };

}
