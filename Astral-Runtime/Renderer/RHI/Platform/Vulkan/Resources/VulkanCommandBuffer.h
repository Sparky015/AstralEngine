/**
* @file VulkanCommandBuffer.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Renderer/RHI/Resources/CommandBuffer.h"
#include "Renderer/RHI/Resources/Device.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    struct VulkanCommandBufferDesc
    {
        VkDevice Device;
        VkCommandPool CommandPool;
    };

    class VulkanCommandBuffer : public CommandBuffer
    {
    public:
        explicit VulkanCommandBuffer(const VulkanCommandBufferDesc& desc);
        ~VulkanCommandBuffer() override;

        void BeginRecording() override;
        void EndRecording() override;
        void Reset() override;

        void BindPipeline() override;
        void BindDescriptorSet() override;

        void* GetNativeHandle() override { return m_CommandBuffer; }

    private:

        void AllocateCommandBuffer();
        void FreeCommandBuffer();

        VkDevice m_Device;
        VkCommandPool m_CommandPool;
        VkCommandBuffer m_CommandBuffer;
    };

}
