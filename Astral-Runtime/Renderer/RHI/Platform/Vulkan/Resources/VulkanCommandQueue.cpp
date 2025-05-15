/**
* @file VulkanCommandQueue.cpp
* @author Andrew Fagan
* @date 5/14/25
*/

#include "VulkanCommandQueue.h"

#include "Debug/Utilities/Asserts.h"

namespace Astral {

    VulkanCommandQueue::VulkanCommandQueue(VkDevice device, uint32 queueFamilyIndex, uint32 queueIndex) :
        m_Device(device),
        m_QueueFamilyIndex(queueFamilyIndex),
        m_QueueIndex(queueIndex)
    {
        GetQueue();
        CreateSemaphores();
    }


    VulkanCommandQueue::~VulkanCommandQueue()
    {
        vkDestroySemaphore(m_Device, m_PresentCompleteSemaphore, nullptr);
        vkDestroySemaphore(m_Device, m_RenderCompleteSemaphore, nullptr);
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

        VkResult result = vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_PresentCompleteSemaphore);
        ASSERT(result == VK_SUCCESS, "Semaphore failed to create!");
        result = vkCreateSemaphore(m_Device, &semaphoreCreateInfo, nullptr, &m_PresentCompleteSemaphore);
        ASSERT(result == VK_SUCCESS, "Semaphore failed to create!");
    }

}

