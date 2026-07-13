/**
* @file RenderPassExecutionContext.h
* @author Andrew Fagan
* @date 6/1/26
*/

#pragma once

#include "Renderer/Common/SceneRendererTypes.h"
#include "Renderer/RenderGraph/RenderGraph.h"
#include "Renderer/RHI/Resources/CommandBuffer.h"
#include "Renderer/RHI/Resources/PipelineStateCache.h"

namespace Astral {

    /**
     * @brief Defines the interface for a render pass execution context with a derived render pass class
     */
    class RenderPassExecutionContext
    {
    public:
        virtual ~RenderPassExecutionContext() = default;

        /**
         * @brief Initializes a render pass' context and resources
         */
        virtual void Init(uint32 numInFlightImages) = 0;

        /**
         * @brief Executes the render pass' logic and submits commands to a command buffer
         * @param renderGraphPassExecutionContext The execution context of the render graph
         */
        virtual void Execute(RenderGraphPassExecutionContext& renderGraphPassExecutionContext, SharedFrameContext& sharedFrameContext) = 0;

        /**
         * @brief Shuts down the render pass' context and cleans up its resources
         */
        virtual void Shutdown() = 0;

    };

}
