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

        void BindPipeline(PipelineStateHandle pipeline) override;
        void BindDescriptorSet(DescriptorSetHandle descriptorSet, uint32 binding) override;
        void BindVertexBuffer(VertexBufferHandle vertexBuffer) override;
        void BindIndexBuffer(IndexBufferHandle indexBuffer) override;

        void SetViewportAndScissor(UVec2 dimensions) override;

        void BeginRenderPass(RenderPassHandle renderPassHandle, FramebufferHandle frameBufferHandle) override;
        void NextSubpass() override;
        void EndRenderPass() override;

        void DrawElementsIndexed(IndexBufferHandle indexBufferHandle) override;
        void Dispatch(uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ) override;
        void PushConstants(void* data, uint32 sizeInBytes) override;
        void SetPipelineBarrier(const PipelineBarrier& pipelineBarrier) override;

        void BeginLabel(std::string_view label, Vec4 color) override;
        void EndLabel() override;
        void InsertMarker(std::string_view label, Vec4 color) override;

        void* GetNativeHandle() override { return m_CommandBuffer; }

    private:

        void AllocateCommandBuffer();
        void FreeCommandBuffer();

        VkDevice m_Device;
        VkCommandPool m_CommandPool;
        VkCommandBuffer m_CommandBuffer;

        PipelineStateHandle m_BoundPipeline{nullptr};
        std::vector<DescriptorSetHandle> m_BoundDescriptorSets;
        VertexBufferHandle m_BoundVertexBuffer{nullptr};
        IndexBufferHandle m_BoundIndexBuffer{nullptr};
        RenderPassHandle m_ActiveRenderPass{nullptr};
        UVec2 m_PipelineDynamicStateViewportAndScissor{-1, -1};
    };

}
