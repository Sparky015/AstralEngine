/**
* @file CommandQueue.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "CommandBuffer.h"
#include "RenderTarget.h"

namespace Astral {

    /**
     * @brief Wraps graphics API command queue operations
     */
    class CommandQueue
    {
    public:
        virtual ~CommandQueue() = default;

        /**
         * @brief Submits a command buffer to a command queue
         * @param commandBuffer The command buffer to submit to the command queue
         * @param renderTargetHandle The render target with a fence and semaphores to submit with the command buffer for the frame
         */
        virtual void Submit(CommandBufferHandle commandBuffer, RenderTargetHandle renderTargetHandle) = 0;

        /**
         * @brief Submits a command buffer to a command queue without a fence or semaphore to signal
         * @param commandBufferHandle The command buffer to submit to the command queue
         */
        virtual void Submit(CommandBufferHandle commandBufferHandle) = 0;

        /**
         * @brief Submits a batch of command buffers to a command queue with the render target fence and semaphore to signal
         * @param commandBufferHandles The command buffer to submit to the command queue
         * @param renderTargetHandle The render target with a fence and semaphores to submit with the command buffer for the frame
         */
        virtual void Submit(const std::vector<CommandBufferHandle>& commandBufferHandles, RenderTargetHandle renderTargetHandle) = 0;

        /**
         * @brief Submits a batch of command buffers to a command queue
         * @param commandBufferHandles The command buffer to submit to the command queue
         */
        virtual void Submit(const std::vector<CommandBufferHandle>& commandBufferHandles) = 0;

        /**
         * @brief Presents a render target to the screen
         * @param renderTarget The render target to present to the screen
         */
        virtual void Present(RenderTargetHandle renderTarget) = 0;

        /**
         * @brief Blocks the current CPU thread until the queue's work is finished on the GPU
         */
        virtual void WaitIdle() = 0;

        /**
         * @brief Gets the native handle of the command queue
         * @return The native handle of the command queue
         */
        virtual void* GetNativeHandle() = 0;
    };

    using CommandQueueHandle = GraphicsRef<CommandQueue>;

}
