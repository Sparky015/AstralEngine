/**
* @file ForwardLightingRenderPass.h
* @author Andrew Fagan
* @date 6/4/26
*/

#pragma once

#include "RenderPassExecutionContext.h"
#include "Renderer/Common/SceneRendererTypes.h"

namespace Astral {

    /**
     * @brief Defines the lighting render pass execution context and logic for the forward rendering path
     */
    class ForwardLightingRenderPass : public RenderPassExecutionContext
    {
    public:
        virtual ~ForwardLightingRenderPass() = default;

        /**
         * @brief Initializes the forward lighting render pass' context and resources
         */
        void Init(uint32 numInFlightImages) override;

        /**
         * @brief Executes the forward lighting render pass' logic and submits commands to a command buffer
         * @param renderGraphPassExecutionContext The execution context of the render graph
         */
        void Execute(RenderGraphPassExecutionContext& renderGraphPassExecutionContext, SharedFrameContext& sharedFrameContext) override;

        /**
         * @brief Shuts down the forward lighting render pass' context and cleans up its resources
         */
        void Shutdown() override;

    private:

        ShaderHandle m_ForwardUnpackedLightingShader;
        ShaderHandle m_ForwardORMLightingShader;
    };

}
