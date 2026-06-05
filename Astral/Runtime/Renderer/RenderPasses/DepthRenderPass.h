/**
* @file DepthRenderPass.h
* @author Andrew Fagan
* @date 6/2/26
*/

#pragma once

#include "RenderPassExecutionContext.h"
#include "Renderer/Common/SceneRendererTypes.h"

namespace Astral {

    /**
     * @brief Defines the depth render pass execution context and logic
     */
    class DepthRenderPass : public RenderPassExecutionContext
    {
    public:
        virtual ~DepthRenderPass() = default;

        /**
         * @brief Initializes the depth render pass' context and resources
         */
        void Init(uint32 numInFlightImages) override;

        /**
         * @brief Executes the depth render pass' logic and submits commands to a command buffer
         * @param renderGraphPassExecutionContext The execution context of the render graph
         */
        void Execute(RenderGraphPassExecutionContext& renderGraphPassExecutionContext, SharedFrameContext& sharedFrameContext) override;

        /**
         * @brief Shuts down the depth render pass' context and cleans up its resources
         */
        void Shutdown() override;

    private:

        ShaderHandle m_DepthWriteOnlyShader;
    };

}

