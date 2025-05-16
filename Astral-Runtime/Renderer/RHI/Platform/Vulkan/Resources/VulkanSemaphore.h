/**
* @file VulkanSemaphore.h
* @author Andrew Fagan
* @date 5/14/25
*/

#pragma once

#include "Renderer/RHI/Resources/Semaphore.h"
#include "VulkanDevice.h"

namespace Astral {

    class VulkanSemaphore : public Semaphore
    {
    public:
        VulkanSemaphore(VkDevice device);
        ~VulkanSemaphore() override;

        void* GetNativeHandle() override { return m_Semaphore; }

    private:
        VkSemaphore m_Semaphore;
    };

}
