/**
* @file MetalCommandQueue.cpp
* @author Andrew Fagan
* @date 5/26/26
*/

#include "MetalCommandQueue.h"

#include "Metal/MTL4CommandQueue.hpp"
#include "Metal/MTLDrawable.hpp"
#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Platform/Metal/MetalRendererContext.h"

namespace Astral {

    MetalCommandQueue::MetalCommandQueue(const MetalCommandQueueDesc& commandQueueDesc) :
        m_Device(commandQueueDesc.Device)
    {
        CreateQueue(commandQueueDesc);

        MetalRenderingContext& renderingContext = (MetalRenderingContext&)RendererAPI::GetContext();
        MTL::ResidencySet* globalResidencySet = renderingContext.GetGlobalResidencySet();
        std::mutex& globalResidencySetMutex = renderingContext.GetGlobalResidencySetMutex();

        m_Queue->addResidencySet(globalResidencySet);

        std::unique_lock residencySetLock(globalResidencySetMutex);
        globalResidencySet->commit(); // This is not thread safe
        residencySetLock.unlock();


        // This is the only place where the swapchain residency set is accessed so no lock

        CA::MetalLayer* swapchainMetalLayer = (CA::MetalLayer*)RendererAPI::GetDevice().GetSwapchain().GetNativeHandle();
        MTL::ResidencySet* swapchainResidencySet = swapchainMetalLayer->residencySet();
        m_Queue->addResidencySet(swapchainResidencySet);
        swapchainResidencySet->commit(); // This is not thread safe
    }


    MetalCommandQueue::~MetalCommandQueue()
    {
        ReleaseQueue();
    }


    void MetalCommandQueue::Submit(CommandBufferHandle commandBufferHandle, RenderTargetHandle renderTargetHandle)
    {
        MetalRenderingContext& renderingContext = (MetalRenderingContext&)RendererAPI::GetContext();
        MTL::ResidencySet* residencySet = renderingContext.GetGlobalResidencySet();
        std::mutex& globalResidencySetMutex = renderingContext.GetGlobalResidencySetMutex();

        std::unique_lock residencySetLock(globalResidencySetMutex);
        residencySet->commit(); // This is not thread safe
        residencySetLock.unlock();


        MTL::Drawable* drawable = (MTL::Drawable*)renderTargetHandle->GetNativeImage();
        MTL4::CommandBuffer* commandBuffer = (MTL4::CommandBuffer*)commandBufferHandle->GetNativeHandle();

        MTL4::CommitOptions* commitOptions = MTL4::CommitOptions::alloc()->init();
        commitOptions->addFeedbackHandler([this, commandBuffer](MTL4::CommitFeedback* commitFeedback) {
            std::unique_lock lock(m_ActiveCommandBufferTrackingLock);

            if (m_ActiveCommandBuffers.contains(commandBuffer))
            {
                m_ActiveCommandBuffers.erase(commandBuffer);
            }

            if (commitFeedback->error())
            {
                AE_WARN("Commit Feedback Error: " << commitFeedback->error()->localizedDescription()->utf8String());
            }

            lock.unlock();
            m_ActiveCommandBufferTrackingCondition.notify_all();
        });

        std::unique_lock lock(m_ActiveCommandBufferTrackingLock);
        m_ActiveCommandBuffers.insert(commandBuffer);
        lock.unlock();

        m_Queue->wait(drawable);
        m_Queue->commit(&commandBuffer, 1, commitOptions);

        commitOptions->release();

    }


    void MetalCommandQueue::SubmitSync(CommandBufferHandle commandBufferHandle)
    {
        MetalRenderingContext& renderingContext = (MetalRenderingContext&)RendererAPI::GetContext();
        MTL::ResidencySet* residencySet = renderingContext.GetGlobalResidencySet();
        std::mutex& globalResidencySetMutex = renderingContext.GetGlobalResidencySetMutex();

        std::unique_lock residencySetLock(globalResidencySetMutex);
        residencySet->commit();
        residencySetLock.unlock();

        MTL4::CommandBuffer* commandBuffer = (MTL4::CommandBuffer*)commandBufferHandle->GetNativeHandle();

        MTL4::CommitOptions* commitOptions = MTL4::CommitOptions::alloc()->init();
        commitOptions->addFeedbackHandler([this, commandBuffer](MTL4::CommitFeedback* commitFeedback) {
            std::unique_lock lock(m_ActiveCommandBufferTrackingLock);

            if (m_ActiveCommandBuffers.contains(commandBuffer))
            {
                m_ActiveCommandBuffers.erase(commandBuffer);
            }

            if (commitFeedback->error())
            {
                AE_WARN("Commit Feedback Error: " << commitFeedback->error()->localizedDescription()->utf8String());
            }

            lock.unlock();
            m_ActiveCommandBufferTrackingCondition.notify_all();
        });

        std::unique_lock lock(m_ActiveCommandBufferTrackingLock);
        m_ActiveCommandBuffers.insert(commandBuffer);
        lock.unlock();

        m_Queue->commit(&commandBuffer, 1, commitOptions);
        commitOptions->release();
    }


    void MetalCommandQueue::Present(RenderTargetHandle renderTarget)
    {
        MTL::Drawable* drawable = (MTL::Drawable*)renderTarget->GetNativeImage();
        m_Queue->signalDrawable(drawable);
        drawable->present();
    }


    void MetalCommandQueue::WaitIdle()
    {
        std::unique_lock lock(m_ActiveCommandBufferTrackingLock);

        m_ActiveCommandBufferTrackingCondition.wait(
            lock,
            [this]() {
                return m_ActiveCommandBuffers.size() == 0;
            }
        );
    }


    void* MetalCommandQueue::GetNativeHandle()
    {
        return m_Queue;
    }


    void MetalCommandQueue::CreateQueue(const MetalCommandQueueDesc& desc)
    {
        MTL4::CommandQueueDescriptor* commandQueueDescriptor = MTL4::CommandQueueDescriptor::alloc()->init();
        commandQueueDescriptor->setLabel(NS::String::string("Global Command Queue",NS::UTF8StringEncoding));

        NS::Error* error = nullptr;
        m_Queue = m_Device->newMTL4CommandQueue(commandQueueDescriptor, &error);

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
