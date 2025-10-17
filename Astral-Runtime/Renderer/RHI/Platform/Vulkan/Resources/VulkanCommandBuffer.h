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

        void BindPipeline(const PipelineStateHandle& pipeline) override;
        void BindDescriptorSet(const DescriptorSetHandle& descriptorSet, uint32 binding) override;
        void BindVertexBuffer(const VertexBufferHandle& vertexBuffer) override;
        void BindIndexBuffer(const IndexBufferHandle& indexBuffer) override;

        void SetViewportAndScissor(UVec2 dimensions) override;

        void BeginRenderPass(const RenderPassHandle& renderPassHandle, const FramebufferHandle& frameBufferHandle) override;
        void NextSubpass() override;
        void EndRenderPass() override;

        void DrawElementsIndexed(const IndexBufferHandle& indexBufferHandle) override;
        void DrawElementsInstanced(const IndexBufferHandle& indexBufferHandle, uint32 numberOfInstances) override;
        void Dispatch(uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ) override;
        void PushConstants(void* data, uint32 sizeInBytes) override;
        void SetPipelineBarrier(const PipelineBarrier& pipelineBarrier) override;

        void BeginLabel(const ::std::string_view& label, Vec4 color) override;
        void EndLabel() override;
        void InsertMarker(const ::std::string_view& label, Vec4 color) override;

        void ResetQueryPool(const QueryPoolHandle& queryPoolHandle) override;
        void WriteTimestamp(const QueryPoolHandle& queryPoolHandle, PipelineStageFlagBits pipelineStage, uint32 query) override;

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
