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
        CreateSemaphores();
    }


    VulkanCommandQueue::~VulkanCommandQueue()
    {
        DestroySemaphores();
    }


    void VulkanCommandQueue::SubmitAsync(CommandBufferHandle commandBufferHandle)
    {
        VkPipelineStageFlags waitFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkCommandBuffer commandBuffer =(VkCommandBuffer)commandBufferHandle->GetNativeHandle();

        VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &m_WaitSemaphore,
            .pWaitDstStageMask = &waitFlags,
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &m_SignalSemaphore,
        };

        VkResult result = vkQueueSubmit(m_Queue, 1, &submitInfo, nullptr);
        ASSERT(result == VK_SUCCESS, "Queue failed to submit command buffer");
    }


    void VulkanCommandQueue::SubmitSync(CommandBufferHandle commandBuffer)
    {
        VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pWaitDstStageMask = nullptr,
            .commandBufferCount = 1,
            .pCommandBuffers = (VkCommandBuffer*)commandBuffer->GetNativeHandle(),
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr,
        };

        VkResult result = vkQueueSubmit(m_Queue, 1, &submitInfo, nullptr);
        ASSERT(result == VK_SUCCESS, "Queue failed to submit command buffer");
    }


    void VulkanCommandQueue::Present(RenderTargetHandle renderTarget)
    {
        uint32 imageIndex = renderTarget->GetImageIndex();

        VkPresentInfoKHR presentInfo = {
          .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
          .pNext = nullptr,
          .waitSemaphoreCount = 1,
          .pWaitSemaphores = &m_WaitSemaphore,
          .swapchainCount = 1,
          .pSwapchains = &m_Swapchain,
          .pImageIndices = &imageIndex,
        };

        VkResult result = vkQueuePresentKHR(m_Queue, &presentInfo);
        ASSERT(result == VK_SUCCESS, "Queue failed to present!");
    }


    void VulkanCommandQueue::GetQueue()
    {
        vkGetDeviceQueue(m_Device, m_QueueFamilyIndex, m_QueueIndex, &m_Queue);
    }


    void VulkanCommandQueue::CreateSemaphores()
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0
        };

        VkResult result = vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_WaitSemaphore);
        ASSERT(result == VK_SUCCESS, "Semaphore failed to create!");
        result = vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_SignalSemaphore);
        ASSERT(result == VK_SUCCESS, "Semaphore failed to create!");
    }

    void VulkanCommandQueue::DestroySemaphores()
    {
        vkDestroySemaphore(m_Device, m_WaitSemaphore, nullptr);
        vkDestroySemaphore(m_Device, m_SignalSemaphore, nullptr);
    }

}

