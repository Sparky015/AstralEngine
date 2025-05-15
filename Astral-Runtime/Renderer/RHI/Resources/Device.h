/**
* @file Device.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once
#include "CommandQueue.h"
#include "Swapchain.h"
#include "Renderer/RHI/Resources/CommandBuffer.h"

namespace Astral {

    class Device
    {
    public:
        virtual ~Device() = default;

        virtual Swapchain* CreateSwapchain() = 0;
        virtual CommandBuffer* AllocateCommandBuffer() = 0;
        virtual CommandQueue* GetCommandQueue() = 0;

        virtual void* GetNativeHandle() = 0;
    };

}
