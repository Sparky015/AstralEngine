/**
* @file VulkanCommandQueue.h
* @author Andrew Fagan
* @date 5/14/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Resources/CommandQueue.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    struct VulkanCommandQueueDesc
    {
        VkDevice Device;
        VkSwapchainKHR Swapchain;
        uint32 QueueFamilyIndex;
        uint32 QueueIndex;
    };

    class VulkanCommandQueue : public CommandQueue
    {
    public:
        explicit VulkanCommandQueue(const VulkanCommandQueueDesc& desc);
        ~VulkanCommandQueue() override;

        void Submit(CommandBufferHandle commandBufferHandle, RenderTargetHandle renderTargetHandle) override;
        void SubmitSync(CommandBufferHandle commandBufferHandle) override;
        void Present(RenderTargetHandle renderTarget) override;
        void WaitIdle() override;

    private:

        void GetQueue();

        VkDevice m_Device;
        VkSwapchainKHR m_Swapchain;
        uint32 m_QueueFamilyIndex;
        uint32 m_QueueIndex;

        VkQueue m_Queue;
    };

}
