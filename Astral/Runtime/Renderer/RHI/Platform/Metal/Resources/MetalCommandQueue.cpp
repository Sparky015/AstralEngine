/**
* @file MetalCommandQueue.cpp
* @author Andrew Fagan
* @date 5/26/26
*/

#include "MetalCommandQueue.h"

#include "Metal/MTL4CommandQueue.hpp"
#include "Metal/MTLDrawable.hpp"

namespace Astral {

    MetalCommandQueue::MetalCommandQueue(const MetalCommandQueueDesc& commandQueueDesc)
    {
        CreateQueue(commandQueueDesc);
    }


    MetalCommandQueue::~MetalCommandQueue()
    {
        ReleaseQueue();
    }


    void MetalCommandQueue::Submit(CommandBufferHandle commandBufferHandle, RenderTargetHandle renderTargetHandle)
    {
        MTL4::CommandBuffer* commandBuffer = (MTL4::CommandBuffer*)commandBufferHandle->GetNativeHandle();
        m_Queue->commit(&commandBuffer, 1);
    }


    void MetalCommandQueue::SubmitSync(CommandBufferHandle commandBufferHandle)
    {
        MTL4::CommandBuffer* commandBuffer = (MTL4::CommandBuffer*)commandBufferHandle->GetNativeHandle();
        m_Queue->commit(&commandBuffer, 1);
    }


    void MetalCommandQueue::Present(RenderTargetHandle renderTarget)
    {
        MTL::Drawable* drawable = (MTL::Drawable*)renderTarget->GetNativeImage();
        m_Queue->signalDrawable(drawable);
    }


    void* MetalCommandQueue::GetNativeHandle()
    {
        return m_Queue;
    }


    void MetalCommandQueue::CreateQueue(const MetalCommandQueueDesc& desc)
    {
        MTL4::CommandQueueDescriptor* commandQueueDescriptor = MTL4::CommandQueueDescriptor::alloc()->init();

        NS::Error* error = nullptr;
        m_Device->newMTL4CommandQueue(commandQueueDescriptor, &error);

        commandQueueDescriptor->release();

        if (error)
        {
            AE_ERROR("MTL4 command queue failed to be created! Error: " << error->localizedDescription()->utf8String())
            error->release();
        }
    }


    void MetalCommandQueue::ReleaseQueue()
    {
        if (m_Queue)
        {
            m_Queue->release();
        }
    }

}
