/**
* @file CommandBuffer.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "IndexBuffer.h"
#include "PipelineState.h"
#include "VertexBuffer.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"
#include "Renderer/RHI/Common/PipelineBarriers.h"

namespace Astral {

    /**
     * @brief Wraps graphics API command buffer operations
     */
    class CommandBuffer
    {
    public:
        virtual ~CommandBuffer() = default;

        /**
         * @brief Opens command recording for the command buffer
         */
        virtual void BeginRecording() = 0;

        /**
         * @brief Ends command recording for the command buffer
         */
        virtual void EndRecording() = 0;

        /**
         * @brief Resets the command buffer
         */
        virtual void Reset() = 0;

        /**
         * @brief Binds the given pipeline
         * @param pipeline The pipeline to bind
         */
        virtual void BindPipeline(const PipelineStateHandle& pipeline) = 0;

        /**
         * @brief Binds the given descriptor set at the given binding
         * @param descriptorSet The descriptor set to bind
         * @param binding The bind slot to use
         */
        virtual void BindDescriptorSet(const DescriptorSetHandle& descriptorSet, uint32 binding) = 0;

        /**
         * @brief Binds the given vertex buffer
         * @param vertexBuffer The vertex buffer to bind
         */
        virtual void BindVertexBuffer(const VertexBufferHandle& vertexBuffer) = 0;

        /**
         * @brief Binds the given index buffer
         * @param indexBuffer The index buffer to bind
         */
        virtual void BindIndexBuffer(const IndexBufferHandle& indexBuffer) = 0;

        /**
         * @brief Sets the viewport and scissor dimensions through the dynamic pipeline states
         * @param dimensions The dimensions to set
         */
        virtual void SetViewportAndScissor(UVec2 dimensions) = 0;

        /**
         * @brief Begins a render pass using the given frame buffer
         * @param renderPassHandle The render pass being used
         * @param attachmentTextures The frame buffer to use with the render pass
         */
        virtual void BeginRenderPass(const RenderPassHandle& renderPassHandle, const std::vector<AttachmentResource>& attachmentTextures) = 0;

        /**
         * @brief Indicates the end of the render pass
         */
        virtual void EndRenderPass() = 0;

        /**
         * @brief Draws the elements of the bound vertex buffer using the indices of the bound index buffer
         * @param indexBufferHandle The index buffer currently bound
         */
        virtual void DrawElementsIndexed(const IndexBufferHandle& indexBufferHandle) = 0;

        /**
         * @brief Draws a given number of instances of elements from the bound vertex buffer and index buffer
         * @param indexBufferHandle The index buffer currently bound
         * @param numberOfInstances The number of instances to draw
         */
        virtual void DrawElementsInstanced(const IndexBufferHandle& indexBufferHandle, uint32 numberOfInstances) = 0;

        /**
         * @brief Dispatches a compute shader based on the bound compute pipeline
         * @param groupCountX The number of groups in the X dimension
         * @param groupCountY The number of groups in the Y dimension
         * @param groupCountZ The number of groups in the Z dimension
         */
        virtual void Dispatch(uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ) = 0;

        /**
         * @brief Sets the push constant data into the command buffer
         * @param data The data to copy to the push constant buffer
         * @param sizeInBytes The number of bytes to copy from the data parameter
         */
        virtual void PushConstants(void* data, uint32 sizeInBytes) = 0;

        /**
         * @brief Inserts a pipeline barrier into the command buffer
         * @param pipelineBarrier The definition of the pipeline barrier to insert
         */
        virtual void SetPipelineBarrier(const PipelineBarrier& pipelineBarrier) = 0;

        /**
         * @brief Begins a debug label
         * @param label The label name
         * @param color The label color shown in the graphics debugger software
         */
        virtual void BeginLabel(const std::string_view& label, Vec4 color) = 0;

        /**
         * @brief Ends the debug label
         */
        virtual void EndLabel() = 0;

        /**
         * @brief Inserts a debug marker into the command buffer
         * @param label The marker name
         * @param color The marker color shown in the graphics debugger software
         */
        virtual void InsertMarker(const std::string_view& label, Vec4 color) = 0;

        /**
         * @brief Gets the native command buffer object
         * @return The native command buffer object
         */
        virtual void* GetNativeHandle() = 0;

        /**
         * @brief Checks if this command buffer is valid
         * @return True if this command buffer is valid, false otherwise
         */
        bool IsValid() { return m_IsValid; }

        /**
         * @brief Checks if this command buffer is in the recording mode
         * @return True if this command buffer is in the recording mode, false otherwise
         */
        bool IsRecording() { return m_State == State::RECORDING; }

        /**
         * @brief Checks if this command buffer has finished recording
         * @return True if this command buffer has finished recording, false otherwise
         */
        bool IsRecorded() { return m_State == State::RECORDED; }

        /**
         * @brief Checks if this command buffer is empty
         * @return True if this command buffer is empty, false otherwise
         */
        bool IsEmpty() { return m_State == State::EMPTY; }

    protected:

        enum class State
        {
            EMPTY,
            RECORDING,
            RECORDED
        };

        State m_State = State::EMPTY;
        bool m_IsValid = false;
    };

    using CommandBufferHandle = GraphicsRef<CommandBuffer>;

}
