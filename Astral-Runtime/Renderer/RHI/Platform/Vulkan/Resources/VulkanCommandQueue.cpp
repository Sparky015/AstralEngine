/**
* @file VulkanCommandQueue.cpp
* @author Andrew Fagan
* @date 5/14/25
*/

#include "VulkanCommandQueue.h"

#include "Debug/Utilities/Asserts.h"

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
        VkPipelineStageFlags waitFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkCommandBuffer commandBuffer =(VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        VkSemaphore renderCompleteSemaphore = (VkSemaphore)renderTargetHandle->GetRenderCompleteSemaphore();
        VkSemaphore imageAvailableSemaphore = (VkSemaphore)renderTargetHandle->GetImageAvailableSemaphore();

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

        VkResult result = vkQueueSubmit(m_Queue, 1, &submitInfo, nullptr);
        ASSERT(result == VK_SUCCESS, "Queue failed to submit command buffer");
    }


    void VulkanCommandQueue::SubmitSync(CommandBufferHandle commandBufferHandle)
    {
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


    void VulkanCommandQueue::Present(RenderTargetHandle renderTarget)
    {
        uint32 imageIndex = renderTarget->GetImageIndex();
        VkSemaphore renderCompleteSemaphore = (VkSemaphore)renderTarget->GetRenderCompleteSemaphore();

        VkPresentInfoKHR presentInfo = {
          .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
          .pNext = nullptr,
          .waitSemaphoreCount = 1,
          .pWaitSemaphores = &renderCompleteSemaphore,
          .swapchainCount = 1,
          .pSwapchains = &m_Swapchain,
          .pImageIndices = &imageIndex,
        };

        VkResult result = vkQueuePresentKHR(m_Queue, &presentInfo);
        ASSERT(result == VK_SUCCESS, "Queue failed to present!");
    }


    void VulkanCommandQueue::WaitIdle()
    {
        vkQueueWaitIdle(m_Queue);
    }


    void VulkanCommandQueue::GetQueue()
    {
        vkGetDeviceQueue(m_Device, m_QueueFamilyIndex, m_QueueIndex, &m_Queue);
    }

}

