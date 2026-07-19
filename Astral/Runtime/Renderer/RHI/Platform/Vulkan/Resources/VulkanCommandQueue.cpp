/**
* @file VulkanCommandQueue.cpp
* @author Andrew Fagan
* @date 5/14/25
*/

#include "VulkanCommandQueue.h"

#include "Profiler/Instrumentation/ScopeProfiler.h"
#include "Core/Utilities/Asserts.h"

namespace Astral {

    VulkanCommandQueue::VulkanCommandQueue(const VulkanCommandQueueDesc& desc) :
        m_Device(desc.Device),
        m_Swapchain(desc.Swapchain),
        m_QueueFamilyIndex(desc.QueueFamilyIndex),
        m_QueueIndex(desc.QueueIndex)
    {
        GetQueue();
    }


    VulkanCommandQueue::~VulkanCommandQueue()
    {

    }


    void VulkanCommandQueue::Submit(CommandBufferHandle commandBufferHandle, RenderTargetHandle renderTargetHandle)
    {
        PROFILE_SCOPE("VulkanCommandQueue::Submit")

        std::unique_lock lock{m_QueueMutex};

        VkPipelineStageFlags waitFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkCommandBuffer commandBuffer =(VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        VkSemaphore renderCompleteSemaphore = (VkSemaphore)renderTargetHandle->GetRenderCompleteSemaphore();
        VkSemaphore imageAvailableSemaphore = (VkSemaphore)renderTargetHandle->GetImageAvailableSemaphore();
        VkFence workCompletedFence = (VkFence)renderTargetHandle->GetFence();

        VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &imageAvailableSemaphore,
            .pWaitDstStageMask = &waitFlags,
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &renderCompleteSemaphore,
        };

        VkResult result = vkQueueSubmit(m_Queue, 1, &submitInfo, workCompletedFence);
        ASSERT(result == VK_SUCCESS, "Queue failed to submit command buffer");
    }


    void VulkanCommandQueue::Submit(CommandBufferHandle commandBufferHandle)
    {
        PROFILE_SCOPE("VulkanCommandQueue::SubmitSync")

        std::unique_lock lock{m_QueueMutex};

        VkCommandBuffer commandBuffer =(VkCommandBuffer)commandBufferHandle->GetNativeHandle();

        VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffer,
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
        };

        VkResult result = vkQueueSubmit(m_Queue, 1, &submitInfo, nullptr);
        ASSERT(result == VK_SUCCESS, "Queue failed to submit command buffer");
    }


    void VulkanCommandQueue::Submit(const std::vector<CommandBufferHandle>& commandBufferHandles, RenderTargetHandle renderTargetHandle)
    {
        PROFILE_SCOPE("VulkanCommandQueue::Submit")

        std::unique_lock lock{m_QueueMutex};

        std::vector<VkCommandBuffer> vkCommandBuffers = {};
        vkCommandBuffers.resize(commandBufferHandles.size());

        for (const CommandBufferHandle& commandBufferHandle : commandBufferHandles)
        {
            VkCommandBuffer vkCommandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
            vkCommandBuffers.push_back(vkCommandBuffer);
        }

        VkPipelineStageFlags waitFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSemaphore renderCompleteSemaphore = (VkSemaphore)renderTargetHandle->GetRenderCompleteSemaphore();
        VkSemaphore imageAvailableSemaphore = (VkSemaphore)renderTargetHandle->GetImageAvailableSemaphore();
        VkFence workCompletedFence = (VkFence)renderTargetHandle->GetFence();

        VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &imageAvailableSemaphore,
            .pWaitDstStageMask = &waitFlags,
            .commandBufferCount = (uint32)vkCommandBuffers.size(),
            .pCommandBuffers = vkCommandBuffers.data(),
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &renderCompleteSemaphore,
        };

        VkResult result = vkQueueSubmit(m_Queue, 1, &submitInfo, workCompletedFence);
        ASSERT(result == VK_SUCCESS, "Queue failed to submit command buffer");
    }


    void VulkanCommandQueue::Submit(const std::vector<CommandBufferHandle>& commandBufferHandles)
    {
        PROFILE_SCOPE("VulkanCommandQueue::Submit")

        std::unique_lock lock{m_QueueMutex};

        std::vector<VkCommandBuffer> vkCommandBuffers = {};
        vkCommandBuffers.resize(commandBufferHandles.size());

        for (const CommandBufferHandle& commandBufferHandle : commandBufferHandles)
        {
            VkCommandBuffer vkCommandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
            vkCommandBuffers.push_back(vkCommandBuffer);
        }

        VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = (uint32)vkCommandBuffers.size(),
            .pCommandBuffers = vkCommandBuffers.data(),
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
        };

        VkResult result = vkQueueSubmit(m_Queue, 1, &submitInfo, nullptr);
        ASSERT(result == VK_SUCCESS, "Queue failed to submit command buffer");
    }


    void VulkanCommandQueue::Present(RenderTargetHandle renderTarget)
    {
        PROFILE_SCOPE("VulkanCommandQueue::Present")

        std::unique_lock lock{m_QueueMutex};

        uint32 imageIndex = renderTarget->GetImageIndex();
        VkSemaphore renderCompleteSemaphore = (VkSemaphore)renderTarget->GetRenderCompleteSemaphore();
        VkSwapchainKHR swapchain = (VkSwapchainKHR)m_Swapchain.GetNativeHandle();

        VkPresentInfoKHR presentInfo = {
          .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
          .pNext = nullptr,
          .waitSemaphoreCount = 1,
          .pWaitSemaphores = &renderCompleteSemaphore,
          .swapchainCount = 1,
          .pSwapchains = &swapchain,
          .pImageIndices = &imageIndex,
        };

        VkResult result = vkQueuePresentKHR(m_Queue, &presentInfo);
        ASSERT(result == VK_SUCCESS, "Queue failed to present!");
    }


    void VulkanCommandQueue::WaitIdle()
    {
        std::unique_lock lock{m_QueueMutex};

        vkQueueWaitIdle(m_Queue);
    }


    void VulkanCommandQueue::GetQueue()
    {
        std::unique_lock lock{m_QueueMutex};

        vkGetDeviceQueue(m_Device, m_QueueFamilyIndex, m_QueueIndex, &m_Queue);
    }

}

