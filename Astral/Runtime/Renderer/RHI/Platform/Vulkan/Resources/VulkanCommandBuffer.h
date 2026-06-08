/**
* @file VulkanCommandBuffer.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Renderer/RHI/Resources/CommandBuffer.h"
#include "Renderer/RHI/Resources/Device.h"

#include <vulkan/vulkan_core.h>

#include "Astral.h"
#include "Astral.h"
#include "Astral.h"
#include "Astral.h"

namespace Astral {

    /**
    * @brief Information to create a Vulkan command buffer from
    */
    struct VulkanCommandBufferDesc
    {
        VkDevice Device;
        VkCommandPool CommandPool;
    };

    /**
     * @brief Wraps Vulkan command buffer operations
     */
    class VulkanCommandBuffer : public CommandBuffer
    {
    public:
        explicit VulkanCommandBuffer(const VulkanCommandBufferDesc& desc);
        ~VulkanCommandBuffer() override;

        /**
         * @brief Opens command recording for the command buffer
         */
        void BeginRecording() override;

        /**
         * @brief Ends command recording for the command buffer
         */
        void EndRecording() override;

        /**
         * @brief Resets the command buffer
         */
        void Reset() override;

        /**
         * @brief Binds the given pipeline
         * @param pipeline The pipeline to bind
         */
        void BindPipeline(const PipelineStateHandle& pipeline) override;

        /**
         * @brief Binds the given descriptor set at the given binding
         * @param descriptorSet The descriptor set to bind
         * @param binding The bind slot to use
         */
        void BindDescriptorSet(const DescriptorSetHandle& descriptorSet, uint32 binding) override;

        /**
         * @brief Binds the given vertex buffer
         * @param vertexBuffer The vertex buffer to bind
         */
        void BindVertexBuffer(const VertexBufferHandle& vertexBuffer) override;

        /**
         * @brief Binds the given index buffer
         * @param indexBuffer The index buffer to bind
         */
        void BindIndexBuffer(const IndexBufferHandle& indexBuffer) override;

        /**
         * @brief Sets the viewport and scissor dimensions through the dynamic pipeline states
         * @param dimensions The dimensions to set
         */
        void SetViewportAndScissor(UVec2 dimensions) override;

        /**
         * @brief Begins a render pass using the given frame buffer
         * @param renderPassHandle The render pass being used
         * @param attachmentResources The frame buffer to use with the render pass
         */
        void BeginRenderPass(const RenderPassHandle& renderPassHandle, const std::vector<AttachmentResource>& attachmentResources) override;

        /**
         * @brief Indicates the start of the next subpass
         */
        void NextSubpass() override;

        /**
         * @brief Indicates the end of the render pass
         */
        void EndRenderPass() override;

        /**
         * @brief Draws the elements of the bound vertex buffer using the indices of the bound index buffer
         * @param indexBufferHandle The index buffer currently bound
         */
        void DrawElementsIndexed(const IndexBufferHandle& indexBufferHandle) override;

        /**
         * @brief Draws a given number of instances of elements from the bound vertex buffer and index buffer
         * @param indexBufferHandle The index buffer currently bound
         * @param numberOfInstances The number of instances to draw
         */
        void DrawElementsInstanced(const IndexBufferHandle& indexBufferHandle, uint32 numberOfInstances) override;

        /**
         * @brief Dispatches a compute shader based on the bound compute pipeline
         * @param groupCountX The number of groups in the X dimension
         * @param groupCountY The number of groups in the Y dimension
         * @param groupCountZ The number of groups in the Z dimension
         */
        void Dispatch(uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ) override;

        /**
         * @brief Sets the push constant data into the command buffer
         * @param data The data to copy to the push constant buffer
         * @param sizeInBytes The number of bytes to copy from the data parameter
         */
        void PushConstants(void* data, uint32 sizeInBytes) override;

        /**
         * @brief Inserts a pipeline barrier into the command buffer
         * @param pipelineBarrier The definition of the pipeline barrier to insert
         */
        void SetPipelineBarrier(const PipelineBarrier& pipelineBarrier) override;

        /**
         * @brief Begins a debug label
         * @param label The label name
         * @param color The label color shown in the graphics debugger software
         */
        void BeginLabel(const ::std::string_view& label, Vec4 color) override;

        /**
         * @brief Ends the debug label
         */
        void EndLabel() override;

        /**
         * @brief Inserts a debug marker into the command buffer
         * @param label The marker name
         * @param color The marker color shown in the graphics debugger software
         */
        void InsertMarker(const ::std::string_view& label, Vec4 color) override;

        /**
         * @brief Gets the native command buffer object
         * @return The native command buffer object
         */
        void* GetNativeHandle() override { return m_CommandBuffer; }

    private:

        /**
         * @brief Allocates the command buffer from the command buffer pool
         */
        void AllocateCommandBuffer();

        /**
         * @brief Frees the command buffer from the command buffer pool
         */
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
