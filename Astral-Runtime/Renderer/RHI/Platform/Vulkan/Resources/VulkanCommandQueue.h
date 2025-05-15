/**
* @file VulkanCommandQueue.h
* @author Andrew Fagan
* @date 5/14/25
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Resources/CommandQueue.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    class VulkanCommandQueue : public CommandQueue
    {
    public:
        explicit VulkanCommandQueue(VkDevice device, uint32 m_QueueFamilyIndex, uint32 m_QueueIndex);
        ~VulkanCommandQueue() override;

    private:

        void GetQueue();
        void CreateSemaphores();

        VkDevice m_Device;
        uint32 m_QueueFamilyIndex;
        uint32 m_QueueIndex;
        VkQueue m_Queue;
        VkSemaphore m_RenderCompleteSemaphore;
        VkSemaphore m_PresentCompleteSemaphore;
    };

}
