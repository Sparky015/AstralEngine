/**
* @file MetalCommandQueue.h
* @author Andrew Fagan
* @date 5/26/26
*/

#pragma once

#include "Renderer/RHI/Resources/CommandQueue.h"

#include "Metal/MTLDevice.hpp"

namespace Astral {

    /**
    * @brief Information to create a Metal command queue from
    */
    struct MetalCommandQueueDesc
    {
        MTL::Device* Device;
    };

    /**
     * @brief Wraps Metal command queue operations
     */
    class MetalCommandQueue : public CommandQueue
    {
    public:

        MetalCommandQueue(const MetalCommandQueueDesc& commandQueueDesc);
        ~MetalCommandQueue() override;

        /**
         * @brief Submits a command buffer to a command queue
         * @param commandBufferHandle The command buffer to submit to the command queue
         * @param renderTargetHandle The render target with a fence and semaphores to submit with the command buffer for the frame
         */
        void Submit(CommandBufferHandle commandBufferHandle, RenderTargetHandle renderTargetHandle) override;

        /**
         * @brief Submits a command buffer to a command queue without a fence or semaphore to signal
         * @param commandBufferHandle The command buffer to submit to the command queue
         */
        void SubmitSync(CommandBufferHandle commandBufferHandle) override;

        /**
         * @brief Presents a render target to the screen
         * @param renderTarget The render target to present to the screen
         */
        void Present(RenderTargetHandle renderTarget) override;

        /**
         * @brief Gets the native handle of the command queue
         * @return The native handle of the command queue
         */
        void* GetNativeHandle() override;

    private:

        /**
         * @brief Creates the MTL4 command queue
         */
        void CreateQueue(const MetalCommandQueueDesc& desc);

        /**
         * @brief Releases the command queue
         */
        void ReleaseQueue();

        MTL::Device* m_Device;
        MTL4::CommandQueue* m_Queue;
    };

}

