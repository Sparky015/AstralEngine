/**
* @file CommandQueue.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "CommandBuffer.h"
#include "RenderTarget.h"

namespace Astral {

    class CommandQueue
    {
    public:
        virtual ~CommandQueue() = default;

        virtual void SubmitAsync(CommandBufferHandle commandBuffer) = 0;
        virtual void SubmitSync(CommandBufferHandle commandBuffer) = 0;
        virtual void Present(RenderTargetHandle renderTarget) = 0;
    };

    using CommandQueueHandle = GraphicsRef<CommandQueue>;

}
