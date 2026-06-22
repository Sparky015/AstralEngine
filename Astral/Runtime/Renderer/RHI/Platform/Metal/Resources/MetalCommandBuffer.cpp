/**
* @file MetalCommandBuffer.cpp
* @author Andrew Fagan
* @date 5/24/26
*/

#include "MetalCommandBuffer.h"

#include "Core/Utilities/Asserts.h"
#include "Metal/MTL4CommandAllocator.hpp"
#include "Metal/MTL4CommandBuffer.hpp"
#include "Metal/MTL4RenderPass.hpp"
#include "Metal/MTLBuffer.hpp"

namespace Astral {

    MetalCommandBuffer::MetalCommandBuffer(const MetalCommandBufferDesc& commandBufferDesc) :
        m_Device(commandBufferDesc.Device)
    {
        CreateCommandBuffer();
    }


    MetalCommandBuffer::~MetalCommandBuffer()
    {
        ReleaseCommandBuffer();
    }


    void MetalCommandBuffer::BeginRecording()
    {
        // TODO: Get Command Allocator
        m_CommandBuffer->beginCommandBuffer();
    }


    void MetalCommandBuffer::EndRecording()
    {
        m_CommandBuffer->endCommandBuffer();
    }


    void MetalCommandBuffer::Reset()
    {
        // TODO: Get Command Allocator

    }


    void MetalCommandBuffer::BindPipeline(const PipelineStateHandle& pipeline)
    {
        if (m_ActiveEncodingType == EncodingType::RENDER)
        {
            ASSERT(pipeline->GetPipelineType() == PipelineType::GRAPHICS, "Given pipeline must be same type as active encoder (Render), but is not!")
            MTL::RenderPipelineState* renderPipelineState = (MTL::RenderPipelineState*)pipeline->GetNativeHandle();
            m_RenderCommandEncoder->setRenderPipelineState(renderPipelineState);
        }
        else if (m_ActiveEncodingType == EncodingType::COMPUTE)
        {
            ASSERT(pipeline->GetPipelineType() == PipelineType::COMPUTE, "Given pipeline must be same type as active encoder (Compute), but is not!")
            MTL::ComputePipelineState* computePipelineState = (MTL::ComputePipelineState*)pipeline->GetNativeHandle();
            m_ComputeCommandEncoder->setComputePipelineState(computePipelineState);
        }
    }


    void MetalCommandBuffer::BindDescriptorSet(const DescriptorSetHandle& descriptorSet, uint32 binding)
    {
        if (m_ActiveEncodingType == EncodingType::RENDER)
        {
            ASSERT(m_RenderCommandEncoder, "Render encoder must be active to use this function (BindDescriptorSet)!")
            MTL4::ArgumentTable* argumentTable = (MTL4::ArgumentTable*)descriptorSet->GetNativeHandle();
            m_RenderCommandEncoder->setArgumentTable(argumentTable, MTL::RenderStageVertex | MTL::RenderStageFragment | MTL::RenderStageMesh);
        }
        else if (m_ActiveEncodingType == EncodingType::COMPUTE)
        {
            ASSERT(m_ComputeCommandEncoder, "Compute encoder must be active to use this function (BindDescriptorSet)!")
            MTL4::ArgumentTable* argumentTable = (MTL4::ArgumentTable*)descriptorSet->GetNativeHandle();
            m_ComputeCommandEncoder->setArgumentTable(argumentTable);
        }
    }


    void MetalCommandBuffer::BindVertexBuffer(const VertexBufferHandle& vertexBuffer)
    {
        ASSERT(m_RenderCommandEncoder && m_ActiveEncodingType == EncodingType::RENDER, "Render encoder must be active to use this function (BindVertexBuffer)!")
        m_BoundVertexBuffer = vertexBuffer;
    }


    void MetalCommandBuffer::BindIndexBuffer(const IndexBufferHandle& indexBuffer)
    {
        ASSERT(m_RenderCommandEncoder && m_ActiveEncodingType == EncodingType::RENDER, "Render encoder must be active to use this function (BindIndexBuffer)!")
        m_BoundIndexBuffer = indexBuffer;
    }


    void MetalCommandBuffer::SetViewportAndScissor(UVec2 dimensions)
    {
        ASSERT(m_RenderCommandEncoder && m_ActiveEncodingType == EncodingType::RENDER, "Render encoder must be active to use this function (SetViewportAndScissor)!")

        MTL::Viewport viewport = MTL::Viewport(0, 0, dimensions.x, dimensions.y, 0, 1);
        m_RenderCommandEncoder->setViewport(viewport);

        MTL::ScissorRect scissorRect = MTL::ScissorRect(0, 0, dimensions.x, dimensions.y);
        m_RenderCommandEncoder->setScissorRect(scissorRect);
    }


    void MetalCommandBuffer::BeginRenderPass(const RenderPassHandle& renderPassHandle, const std::vector<AttachmentResource>& attachmentTextures)
    {
        // ==== End the current computer encoder if applicable ====================

        if (m_ActiveEncodingType == EncodingType::COMPUTE && m_ComputeCommandEncoder)
        {
            m_ComputeCommandEncoder->endEncoding();
            m_ComputeCommandEncoder = nullptr;
        }


        // ==== Create a new render encoder ====================

        // TODO: Populate render pass descriptor
        MTL4::RenderPassDescriptor* renderPassDescriptor = MTL4::RenderPassDescriptor::alloc()->init();
        renderPassDescriptor;


        m_RenderCommandEncoder = m_CommandBuffer->renderCommandEncoder(renderPassDescriptor);
        m_ActiveEncodingType = EncodingType::RENDER;
    }


    void MetalCommandBuffer::EndRenderPass()
    {
        ASSERT(m_RenderCommandEncoder && m_ActiveEncodingType == EncodingType::RENDER, "Render encoder must be active to use this function (EndRenderPass)!")
        m_RenderCommandEncoder->endEncoding();

        m_RenderCommandEncoder = nullptr;
        m_ActiveEncodingType = EncodingType::NONE;
    }


    void MetalCommandBuffer::DrawElementsIndexed(const IndexBufferHandle& indexBufferHandle)
    {
        ASSERT(m_RenderCommandEncoder && m_ActiveEncodingType == EncodingType::RENDER, "Render encoder must be active to use this function (DrawElementsIndexed)!")
        uint32 numOfIndices = indexBufferHandle->GetCount();
        MTL::Buffer* indexBuffer = (MTL::Buffer*)indexBufferHandle->GetNativeHandle();
        MTL::GPUAddress bufferAddress = indexBuffer->gpuAddress();
        uint32 bufferLength = indexBuffer->length();

        m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, numOfIndices, MTL::IndexTypeUInt32, bufferAddress, bufferLength);
    }


    void MetalCommandBuffer::DrawElementsInstanced(const IndexBufferHandle& indexBufferHandle, uint32 numberOfInstances)
    {
        ASSERT(m_RenderCommandEncoder && m_ActiveEncodingType == EncodingType::RENDER, "Render encoder must be active to use this function (DrawElementsInstanced)!")
        uint32 numOfIndices = indexBufferHandle->GetCount();
        MTL::Buffer* indexBuffer = (MTL::Buffer*)indexBufferHandle->GetNativeHandle();
        MTL::GPUAddress bufferAddress = indexBuffer->gpuAddress();
        uint32 bufferLength = indexBuffer->length();

        m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, numOfIndices, MTL::IndexTypeUInt32, bufferAddress, bufferLength, numberOfInstances);
    }


    void MetalCommandBuffer::Dispatch(uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ)
    {
        ASSERT(m_ActiveEncodingType == EncodingType::NONE || m_ActiveEncodingType == EncodingType::COMPUTE, "Render encoder must not be active when using this function (Dispatch)!")

        if (m_ActiveEncodingType != EncodingType::COMPUTE)
        {
            m_ComputeCommandEncoder = m_CommandBuffer->computeCommandEncoder();
            m_ActiveEncodingType = EncodingType::COMPUTE;
        }

        MTL::Size groupSize = MTL::Size(groupCountX, groupCountY, groupCountZ);
        MTL::Size localThreadGrid = MTL::Size(-1, -1, -1); // TODO: Use shader reflection from bound pipeline to query thread grid size
        m_ComputeCommandEncoder->dispatchThreads(groupSize, localThreadGrid);
    }


    void MetalCommandBuffer::PushConstants(void* data, uint32 sizeInBytes)
    {
        // TODO: Implement later with bindless
    }


    void MetalCommandBuffer::SetPipelineBarrier(const PipelineBarrier& pipelineBarrier)
    {
        ASSERT(m_ActiveEncodingType != EncodingType::NONE, "Encoder must be active to use this function (SetPipelineBarrier)!")

        if (m_ActiveEncodingType == EncodingType::RENDER)
        {
            m_RenderCommandEncoder;
        }
        else if (m_ActiveEncodingType == EncodingType::COMPUTE)
        {
            m_ComputeCommandEncoder;
        }
    }


    void MetalCommandBuffer::BeginLabel(const std::string_view& label, Vec4 color)
    {
        m_CommandBuffer->pushDebugGroup(NS::String::string(label.data(), NS::UTF8StringEncoding));
    }


    void MetalCommandBuffer::EndLabel()
    {
        m_CommandBuffer->popDebugGroup();
    }


    void MetalCommandBuffer::InsertMarker(const std::string_view& label, Vec4 color)
    {
        ASSERT(m_ActiveEncodingType != EncodingType::NONE, "Encoder must be active to use this function!")

        if (m_ActiveEncodingType == EncodingType::RENDER)
        {
            m_RenderCommandEncoder->insertDebugSignpost(NS::String::string(label.data(), NS::UTF8StringEncoding));
        }
        else if (m_ActiveEncodingType == EncodingType::COMPUTE)
        {
            m_ComputeCommandEncoder->insertDebugSignpost(NS::String::string(label.data(), NS::UTF8StringEncoding));
        }
    }


    void* MetalCommandBuffer::GetNativeHandle()
    {
        return m_CommandBuffer;
    }


    void MetalCommandBuffer::CreateCommandBuffer()
    {
        m_CommandBuffer = m_Device->newCommandBuffer();
    }


    void MetalCommandBuffer::ReleaseCommandBuffer()
    {
        if (m_RenderCommandEncoder)
        {
            m_RenderCommandEncoder->endEncoding();
            m_RenderCommandEncoder = nullptr;
        }
        if (m_ComputeCommandEncoder)
        {
            m_ComputeCommandEncoder->endEncoding();
            m_ComputeCommandEncoder = nullptr;
        }

        if (m_CommandBuffer)
        {
            m_CommandBuffer->release();
            m_CommandBuffer = nullptr;
        }
    }

}
