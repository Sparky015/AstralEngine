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

    class CommandBuffer
    {
    public:
        virtual ~CommandBuffer() = default;

        virtual void BeginRecording() = 0;
        virtual void EndRecording() = 0;
        virtual void Reset() = 0;

        virtual void BindPipeline(PipelineStateHandle pipeline) = 0;
        virtual void BindDescriptorSet(DescriptorSetHandle descriptorSet, uint32 binding) = 0;
        virtual void BindVertexBuffer(VertexBufferHandle vertexBuffer) = 0;
        virtual void BindIndexBuffer(IndexBufferHandle indexBuffer) = 0;

        virtual void SetViewportAndScissor(UVec2 dimensions) = 0;

        virtual void BeginRenderPass(RenderPassHandle renderPassHandle, FramebufferHandle frameBufferHandle) = 0;
        virtual void NextSubpass() = 0;
        virtual void EndRenderPass() = 0;

        virtual void DrawElementsIndexed(IndexBufferHandle indexBufferHandle) = 0;
        virtual void Dispatch(uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ) = 0;
        virtual void PushConstants(PipelineStateHandle pipelineStateObjectHandle, void* data, uint32 sizeInBytes) = 0;
        virtual void SetPipelineBarrier(const PipelineBarrier& pipelineBarrier) = 0;

        virtual void BeginLabel(std::string_view label, Vec4 color) = 0;
        virtual void EndLabel() = 0;
        virtual void InsertMarker(std::string_view label, Vec4 color) = 0;

        virtual void* GetNativeHandle() = 0;

        bool IsValid() { return m_IsValid; }
        bool IsRecording() { return m_State == State::RECORDING; }
        bool IsRecorded() { return m_State == State::RECORDED; }
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
