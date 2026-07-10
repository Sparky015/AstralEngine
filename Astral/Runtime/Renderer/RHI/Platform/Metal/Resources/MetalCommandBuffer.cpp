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
#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Platform/Metal/MetalRendererContext.h"
#include "Renderer/RHI/Platform/Metal/Common/MTLEnumConversions.h"

namespace Astral {

    MetalCommandBuffer::MetalCommandBuffer(const MetalCommandBufferDesc& commandBufferDesc) :
        m_Device(commandBufferDesc.Device),
        m_CommandBuffer(nullptr),
        m_CommandAllocator(nullptr),
        m_ComputeCommandEncoder(nullptr),
        m_RenderCommandEncoder(nullptr),
        m_EncoderAutoreleasePool(nullptr),
        m_ActiveEncodingType(EncodingType::NONE),

        m_BoundPipeline(nullptr),
        m_BoundIndexBuffer(nullptr),
        m_BoundVertexBuffer(nullptr),
        m_BoundDescriptorSets({}),

        m_ArgumentTable(nullptr),
        m_PushConstants({})
    {
        AcquireCommandAllocator();
        CreateArgumentTable();
        CreateCommandBuffer();
    }


    MetalCommandBuffer::~MetalCommandBuffer()
    {
        ReleaseCommandBuffer();
        ReleaseArgumentTable();
        ReleaseCommandAllocator();

        m_BoundDescriptorSets.clear();
    }


    void MetalCommandBuffer::BeginRecording()
    {
        m_CommandBuffer->beginCommandBuffer(m_CommandAllocator);
    }


    void MetalCommandBuffer::EndRecording()
    {
        if (m_RenderCommandEncoder)
        {
            AE_ERROR("EndRenderPass was not called before ending command buffer recording!")
        }
        if (m_ComputeCommandEncoder)
        {
            EndComputeEncoder();
        }
        m_ActiveEncodingType = EncodingType::NONE;

        m_CommandBuffer->endCommandBuffer();
    }


    void MetalCommandBuffer::Reset()
    {
        m_CommandAllocator->reset();
        m_PushConstants.clear();
    }


    void MetalCommandBuffer::BindPipeline(const PipelineStateHandle& pipeline)
    {
        if (m_BoundPipeline && pipeline->GetNativeHandle() == m_BoundPipeline->GetNativeHandle()) { return; } // Prevent redundant pipeline bind call
        if (m_BoundPipeline && pipeline->GetDescriptorSetLayout() != m_BoundPipeline->GetDescriptorSetLayout()) { m_BoundDescriptorSets.clear(); }

        if (m_ActiveEncodingType != EncodingType::COMPUTE && pipeline->GetPipelineType() == PipelineType::COMPUTE)
        {
            // Switch to compute encoder if a compute pipeline is bound
            BeginComputeEncoder();
        }

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

        m_BoundPipeline = pipeline;
    }


    void MetalCommandBuffer::BindDescriptorSet(const DescriptorSetHandle& descriptorSet, uint32 binding)
    {
        if (m_BoundDescriptorSets.size() <= binding) { m_BoundDescriptorSets.resize(binding + 1); }
        if (m_BoundDescriptorSets[binding] && m_BoundDescriptorSets[binding]->GetNativeHandle() == descriptorSet->GetNativeHandle()) { return; } // Prevent redundant descriptor set bind call

        MTL::Buffer* argumentBuffer = (MTL::Buffer*)descriptorSet->GetNativeHandle();
        MTL::GPUAddress argumentBufferAddress = argumentBuffer->gpuAddress();
        m_ArgumentTable->setAddress(argumentBufferAddress, binding);

        MetalRenderingContext& renderingContext = (MetalRenderingContext&)RendererAPI::GetContext();
        MTL::ResidencySet* residencySet = renderingContext.GetGlobalResidencySet();
        residencySet->addAllocation(argumentBuffer);

        m_BoundDescriptorSets[binding] = descriptorSet;
    }


    void MetalCommandBuffer::BindVertexBuffer(const VertexBufferHandle& vertexBuffer)
    {
        ASSERT(m_RenderCommandEncoder && m_ActiveEncodingType == EncodingType::RENDER, "Render encoder must be active to use this function (BindVertexBuffer)!")
        m_BoundVertexBuffer = vertexBuffer;

        MTL::Buffer* mtlVertexBuffer = (MTL::Buffer*)vertexBuffer->GetNativeHandle();
        MTL::GPUAddress bufferAddress = mtlVertexBuffer->gpuAddress();
        m_ArgumentTable->setAddress(bufferAddress, 29);
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


    void MetalCommandBuffer::BeginRenderPass(const RenderPassHandle& renderPassHandle, const std::vector<AttachmentResource>& attachmentResources)
    {
        // ==== End the current computer encoder if applicable ====================

        if (m_ActiveEncodingType == EncodingType::COMPUTE && m_ComputeCommandEncoder)
        {
            EndComputeEncoder();
        }

        m_EncoderAutoreleasePool = NS::AutoreleasePool::alloc()->init(); // Autorelease pool for render encoder

        ASSERT(attachmentResources.size() > 0, "No attachment resources were given to render pass!")
        UVec2 extent = attachmentResources[0].Resource->GetDimensions();
        uint32 layerCount = attachmentResources[0].Resource->GetNumLayers();

        MTL4::RenderPassDescriptor* renderPassDescriptor = MTL4::RenderPassDescriptor::alloc()->init();

        renderPassDescriptor->setDefaultRasterSampleCount(ConvertSampleCountToIntSampleCount(attachmentResources[0].Resource->GetMSAASampleCount()));
        renderPassDescriptor->setRenderTargetWidth(extent.x);
        renderPassDescriptor->setRenderTargetHeight(extent.y);
        renderPassDescriptor->setRenderTargetArrayLength(layerCount);
        renderPassDescriptor->setSupportColorAttachmentMapping(false);


        // ==== Populating the color attachments fields ========================================================

        MTL::RenderPassColorAttachmentDescriptorArray* colorAttachmentDescriptorArray = renderPassDescriptor->colorAttachments();

        const std::vector<AttachmentReference>& colorAttachmentReferences = renderPassHandle->GetColorAttachmentReferences();
        const std::vector<AttachmentReference>& resolveAttachmentReferences = renderPassHandle->GetResolveAttachmentReferences();

        for (size_t i = 0; i < colorAttachmentReferences.size(); i++)
        {

            const AttachmentReference& colorAttachmentReference = colorAttachmentReferences[i];
            AttachmentDescription colorAttachmentDescription = renderPassHandle->GetAttachmentDescription(colorAttachmentReference.AttachmentIndex);
            AttachmentResource colorAttachmentResource = attachmentResources[colorAttachmentReference.AttachmentIndex];
            MTL::Texture* colorImage = (MTL::Texture*)colorAttachmentResource.Resource->GetNativeImageView(colorAttachmentResource.ArrayLayer, colorAttachmentResource.MipLevel);

            MTL::RenderPassColorAttachmentDescriptor* colorAttachmentDescriptor = MTL::RenderPassColorAttachmentDescriptor::alloc()->init();

            MTL::ClearColor clearColor = MTL::ClearColor(colorAttachmentDescription.ClearColor.r, colorAttachmentDescription.ClearColor.g,
                                                         colorAttachmentDescription.ClearColor.b, colorAttachmentDescription.ClearColor.a);
            uint32 mipLevel = (colorAttachmentResource.MipLevel == FullSubresourceRange) ? 0 : colorAttachmentResource.MipLevel;
            uint32 arraySlice = (colorAttachmentResource.ArrayLayer == FullSubresourceRange) ? 0 : colorAttachmentResource.ArrayLayer;

            colorAttachmentDescriptor->setClearColor(clearColor);
            colorAttachmentDescriptor->setLevel(mipLevel);
            colorAttachmentDescriptor->setLoadAction(ConvertAttachmentLoadOpToMTLLoadAction(colorAttachmentDescription.LoadOp));
            colorAttachmentDescriptor->setSlice(arraySlice);
            colorAttachmentDescriptor->setStoreAction(ConvertAttachmentStoreOpToMTLStoreAction(colorAttachmentDescription.StoreOp));
            colorAttachmentDescriptor->setTexture(colorImage);


            if (i < resolveAttachmentReferences.size())
            {
                // A resolve attachment exists corresponding to this color attachment
                const AttachmentReference& resolveAttachmentReference = resolveAttachmentReferences[i];
                AttachmentResource resolveAttachmentResource = attachmentResources[resolveAttachmentReference.AttachmentIndex];
                MTL::Texture* resolveImageView = (MTL::Texture*)resolveAttachmentResource.Resource->GetNativeImageView(resolveAttachmentResource.ArrayLayer, resolveAttachmentResource.MipLevel);

                uint32 resolveMipLevel = (resolveAttachmentResource.MipLevel == FullSubresourceRange) ? 0 : resolveAttachmentResource.MipLevel;
                uint32 resolveArraySlice = (resolveAttachmentResource.ArrayLayer == FullSubresourceRange) ? 0 : resolveAttachmentResource.ArrayLayer;

                colorAttachmentDescriptor->setResolveLevel(resolveMipLevel);
                colorAttachmentDescriptor->setResolveSlice(resolveArraySlice);
                colorAttachmentDescriptor->setResolveTexture(resolveImageView);
                colorAttachmentDescriptor->setStoreAction(colorAttachmentDescription.StoreOp == AttachmentStoreOp::STORE ? MTL::StoreActionStoreAndMultisampleResolve : MTL::StoreActionMultisampleResolve);
            }

            colorAttachmentDescriptorArray->setObject(colorAttachmentDescriptor, i);
            colorAttachmentDescriptor->release();
        }


        // ==== Populating the depth/stencil attachment fields ========================================================

        AttachmentReference depthStencilAttachmentReference = renderPassHandle->GetDepthStencilAttachmentReference();


        if (depthStencilAttachmentReference.AttachmentIndex != NullAttachmentIndex)
        {
            // Depth stencil attachment exists
            MTL::RenderPassDepthAttachmentDescriptor* depthAttachmentDescriptor = MTL::RenderPassDepthAttachmentDescriptor::alloc()->init();

            AttachmentDescription depthStencilAttachmentDescription = renderPassHandle->GetAttachmentDescription(depthStencilAttachmentReference.AttachmentIndex);
            AttachmentResource depthStencilAttachmentResource = attachmentResources[depthStencilAttachmentReference.AttachmentIndex];
            MTL::Texture* depthStencilTexture = (MTL::Texture*)depthStencilAttachmentResource.Resource->GetNativeImageView(depthStencilAttachmentResource.ArrayLayer, depthStencilAttachmentResource.MipLevel);

            uint32 depthStencilMipLevel = (depthStencilAttachmentResource.MipLevel == FullSubresourceRange) ? 0 : depthStencilAttachmentResource.MipLevel;
            uint32 depthStencilArraySlice = (depthStencilAttachmentResource.ArrayLayer == FullSubresourceRange) ? 0 : depthStencilAttachmentResource.ArrayLayer;


            depthAttachmentDescriptor->setClearDepth(depthStencilAttachmentDescription.ClearColor.x);
            depthAttachmentDescriptor->setLevel(depthStencilMipLevel);
            depthAttachmentDescriptor->setLoadAction(ConvertAttachmentLoadOpToMTLLoadAction(depthStencilAttachmentDescription.LoadOp));
            depthAttachmentDescriptor->setSlice(depthStencilArraySlice);
            depthAttachmentDescriptor->setStoreAction(ConvertAttachmentStoreOpToMTLStoreAction(depthStencilAttachmentDescription.StoreOp));
            depthAttachmentDescriptor->setTexture(depthStencilTexture);


            renderPassDescriptor->setDepthAttachment(depthAttachmentDescriptor);
            depthAttachmentDescriptor->release();
            depthAttachmentDescriptor = nullptr;

            ImageFormat imageFormat = depthStencilAttachmentResource.Resource->GetFormat();
            if (IsStencilFormat(imageFormat))
            {
                MTL::RenderPassStencilAttachmentDescriptor* stencilAttachmentDescriptor = MTL::RenderPassStencilAttachmentDescriptor::alloc()->init();

                stencilAttachmentDescriptor->setClearStencil(depthStencilAttachmentDescription.ClearColor.y);
                stencilAttachmentDescriptor->setLevel(depthStencilMipLevel);
                stencilAttachmentDescriptor->setLoadAction(ConvertAttachmentLoadOpToMTLLoadAction(depthStencilAttachmentDescription.LoadOp));
                stencilAttachmentDescriptor->setSlice(depthStencilArraySlice);
                stencilAttachmentDescriptor->setStoreAction(ConvertAttachmentStoreOpToMTLStoreAction(depthStencilAttachmentDescription.StoreOp));
                stencilAttachmentDescriptor->setTexture(depthStencilTexture);

                renderPassDescriptor->setStencilAttachment(stencilAttachmentDescriptor);
                stencilAttachmentDescriptor->release();
                stencilAttachmentDescriptor = nullptr;
            }
        }


        // ==== Create a new render encoder ====================

        m_RenderCommandEncoder = m_CommandBuffer->renderCommandEncoder(renderPassDescriptor);
        m_ActiveEncodingType = EncodingType::RENDER;
        m_BoundPipeline = nullptr;
        m_BoundIndexBuffer = nullptr;
        m_BoundVertexBuffer = nullptr;

        MTL::DepthStencilDescriptor* depthStencilDescriptor = MTL::DepthStencilDescriptor::alloc()->init();
        depthStencilDescriptor->setDepthCompareFunction(MTL::CompareFunctionLessEqual);
        depthStencilDescriptor->setDepthWriteEnabled(true);
        MTL::DepthStencilState* depthStencilState = m_Device->newDepthStencilState(depthStencilDescriptor);
        m_RenderCommandEncoder->setDepthStencilState(depthStencilState);
        depthStencilDescriptor->release();
    }


    void MetalCommandBuffer::EndRenderPass()
    {
        ASSERT(m_RenderCommandEncoder && m_ActiveEncodingType == EncodingType::RENDER, "Render encoder must be active to use this function (EndRenderPass)!")

        FlushQueuePipelineBarrier(m_RenderCommandEncoder);

        m_RenderCommandEncoder->endEncoding();

        m_RenderCommandEncoder = nullptr;
        m_ActiveEncodingType = EncodingType::NONE;

        m_EncoderAutoreleasePool->drain();
        m_EncoderAutoreleasePool = nullptr;
    }


    void MetalCommandBuffer::DrawElementsIndexed(const IndexBufferHandle& indexBufferHandle)
    {
        ASSERT(m_RenderCommandEncoder && m_ActiveEncodingType == EncodingType::RENDER, "Render encoder must be active to use this function (DrawElementsIndexed)!")

        m_RenderCommandEncoder->setArgumentTable(m_ArgumentTable, MTL::RenderStageVertex | MTL::RenderStageFragment);

        uint32 numOfIndices = indexBufferHandle->GetCount();
        MTL::Buffer* indexBuffer = (MTL::Buffer*)indexBufferHandle->GetNativeHandle();
        MTL::GPUAddress bufferAddress = indexBuffer->gpuAddress();
        uint32 bufferLength = indexBuffer->length();

        MetalRenderingContext& renderingContext = (MetalRenderingContext&)RendererAPI::GetContext();
        MTL::ResidencySet* residencySet = renderingContext.GetGlobalResidencySet();
        residencySet->addAllocation(indexBuffer);

        m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, numOfIndices, MTL::IndexTypeUInt32, bufferAddress, bufferLength);
    }


    void MetalCommandBuffer::DrawElementsInstanced(const IndexBufferHandle& indexBufferHandle, uint32 numberOfInstances)
    {
        ASSERT(m_RenderCommandEncoder && m_ActiveEncodingType == EncodingType::RENDER, "Render encoder must be active to use this function (DrawElementsInstanced)!")

        m_RenderCommandEncoder->setArgumentTable(m_ArgumentTable, MTL::RenderStageVertex | MTL::RenderStageFragment);

        uint32 numOfIndices = indexBufferHandle->GetCount();
        MTL::Buffer* indexBuffer = (MTL::Buffer*)indexBufferHandle->GetNativeHandle();
        MTL::GPUAddress bufferAddress = indexBuffer->gpuAddress();
        uint32 bufferLength = indexBuffer->length();

        MetalRenderingContext& renderingContext = (MetalRenderingContext&)RendererAPI::GetContext();
        MTL::ResidencySet* residencySet = renderingContext.GetGlobalResidencySet();
        residencySet->addAllocation(indexBuffer);

        m_RenderCommandEncoder->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, numOfIndices, MTL::IndexTypeUInt32, bufferAddress, bufferLength, numberOfInstances);
    }


    void MetalCommandBuffer::Dispatch(uint32 groupCountX, uint32 groupCountY, uint32 groupCountZ)
    {
        ASSERT(m_ActiveEncodingType == EncodingType::COMPUTE, "Render encoder must not be active when using this function (Dispatch)!")

        m_ComputeCommandEncoder->setArgumentTable(m_ArgumentTable);

        const ShaderReflectionInfo& computeShaderReflectionInfo = m_BoundPipeline->GetCompiledComputeShader()->GetShaderReflectionInfo();
        Vec3 workgroupSize = computeShaderReflectionInfo.WorkgroupDimensions;

        MTL::Size threadsPerGrid = MTL::Size(groupCountX, groupCountY, groupCountZ);
        MTL::Size threadsPerThreadgroup = MTL::Size(workgroupSize.x, workgroupSize.y, workgroupSize.z);
        m_ComputeCommandEncoder->dispatchThreadgroups(threadsPerGrid, threadsPerThreadgroup);
    }


    void MetalCommandBuffer::PushConstants(void* data, uint32 sizeInBytes)
    {
        BufferHandle pushConstantBuffer = RendererAPI::GetDevice().CreateUniformBuffer(data, sizeInBytes, GPUMemoryType::HOST_VISIBLE);
        char pushConstantName[30] = "";
        snprintf(pushConstantName, sizeof(pushConstantName), "Push_Constant_%zu", m_PushConstants.size());
        RendererAPI::NameObject(pushConstantBuffer, pushConstantName);

        m_PushConstants.push_back(pushConstantBuffer);
        MTL::Buffer* mtlBuffer = (MTL::Buffer*)pushConstantBuffer->GetNativeHandle();
        m_ArgumentTable->setAddress(mtlBuffer->gpuAddress(), 30);

        MetalRenderingContext& renderingContext = (MetalRenderingContext&)RendererAPI::GetContext();
        MTL::ResidencySet* residencySet = renderingContext.GetGlobalResidencySet();
        residencySet->addAllocation(mtlBuffer);
    }


    void MetalCommandBuffer::SetPipelineBarrier(const PipelineBarrier& pipelineBarrier)
    {
        if (m_ActiveEncodingType == EncodingType::NONE)
        {
            AE_WARN("Encoder must be active to use this function (SetPipelineBarrier)! Skipping pipeline barrier!")
            return;
        }

        MTL4::CommandEncoder* commandEncoder;
        if (m_ActiveEncodingType == EncodingType::RENDER)
        {
            commandEncoder = m_RenderCommandEncoder;
        }
        else if (m_ActiveEncodingType == EncodingType::COMPUTE)
        {
            commandEncoder = m_ComputeCommandEncoder;
        }
        else
        {
            AE_ERROR("Encoding type is not supported!")
        }

        MTL::Stages beforeStages = ConvertPipelineStateFlagsToMTLStages(pipelineBarrier.SourceStageMask);
        MTL::Stages afterStages = ConvertPipelineStateFlagsToMTLStages(pipelineBarrier.DestinationStageMask);

        m_PipelineBarrierBeforeStages |= beforeStages;
        m_PipelineBarrierAfterStages |= afterStages;
        m_IsPipelineBarrierFlagsDirty = true;
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

        MTL4::CommandEncoder* commandEncoder;
        if (m_ActiveEncodingType == EncodingType::RENDER)
        {
            commandEncoder = m_RenderCommandEncoder;
        }
        else if (m_ActiveEncodingType == EncodingType::COMPUTE)
        {
            commandEncoder = m_ComputeCommandEncoder;
        }
        else
        {
            AE_ERROR("Encoding type is not supported!")
        }

        commandEncoder->insertDebugSignpost(NS::String::string(label.data(), NS::UTF8StringEncoding));
    }


    void* MetalCommandBuffer::GetNativeHandle()
    {
        return m_CommandBuffer;
    }


    MTL4::RenderCommandEncoder* MetalCommandBuffer::GetRenderCommandEncoder()
    {
        return m_RenderCommandEncoder;
    }


    MTL4::ComputeCommandEncoder* MetalCommandBuffer::GetComputeCommandEncoder()
    {
        return m_ComputeCommandEncoder;
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


    void MetalCommandBuffer::AcquireCommandAllocator()
    {
        MetalRenderingContext& context = static_cast<MetalRenderingContext&>(RendererAPI::GetContext());
        m_CommandAllocator = context.AcquireThreadCommandAllocator();
    }


    void MetalCommandBuffer::ReleaseCommandAllocator()
    {
        Reset();

        MetalRenderingContext& context = static_cast<MetalRenderingContext&>(RendererAPI::GetContext());
        context.ReleaseThreadCommandAllocator(m_CommandAllocator);
        m_CommandAllocator = nullptr;
    }


    void MetalCommandBuffer::CreateArgumentTable()
    {
        MTL4::ArgumentTableDescriptor* argumentTableDescriptor = MTL4::ArgumentTableDescriptor::alloc();
        NS::Error* error = nullptr;

        constexpr uint32 maxSupportedBufferCount = 31;
        constexpr uint32 maxSupportedTextureCount = 31;
        constexpr uint32 maxSupportedSamplerCount = 16;
        argumentTableDescriptor->setMaxBufferBindCount(maxSupportedBufferCount);
        argumentTableDescriptor->setMaxSamplerStateBindCount(maxSupportedSamplerCount);
        argumentTableDescriptor->setMaxTextureBindCount(maxSupportedTextureCount);

        m_ArgumentTable = m_Device->newArgumentTable(argumentTableDescriptor, &error);

        if (m_ArgumentTable == nullptr)
        {
            AE_ERROR("Argument table failed to be created! Error: " << error->localizedDescription()->utf8String());
            error->release();
        }
    }


    void MetalCommandBuffer::ReleaseArgumentTable()
    {
        if (m_ArgumentTable)
        {
            m_ArgumentTable->release();
            m_ArgumentTable = nullptr;
        }
    }


    void MetalCommandBuffer::BeginComputeEncoder()
    {
        m_EncoderAutoreleasePool = NS::AutoreleasePool::alloc()->init();

        m_ComputeCommandEncoder = m_CommandBuffer->computeCommandEncoder();
        m_ActiveEncodingType = EncodingType::COMPUTE;
        m_BoundPipeline = nullptr;
        m_BoundIndexBuffer = nullptr;
        m_BoundVertexBuffer = nullptr;
    }


    void MetalCommandBuffer::EndComputeEncoder()
    {
        FlushQueuePipelineBarrier(m_RenderCommandEncoder);

        m_ComputeCommandEncoder->endEncoding();
        m_ComputeCommandEncoder = nullptr;

        m_EncoderAutoreleasePool->drain();
        m_EncoderAutoreleasePool = nullptr;
    }


    void MetalCommandBuffer::FlushQueuePipelineBarrier(MTL4::CommandEncoder* commandEncoder)
    {
        if (m_IsPipelineBarrierFlagsDirty)
        {
            commandEncoder->barrierAfterQueueStages(m_PipelineBarrierAfterStages, m_PipelineBarrierBeforeStages, MTL4::VisibilityOptionDevice);
            m_PipelineBarrierAfterStages = 0;
            m_PipelineBarrierBeforeStages = 0;
            m_IsPipelineBarrierFlagsDirty = false;
        }
    }

}
