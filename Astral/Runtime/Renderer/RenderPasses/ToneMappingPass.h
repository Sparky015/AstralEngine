/**
* @file ToneMappingPass.h
* @author Andrew Fagan
* @date 6/4/26
*/

#pragma once

#include "RenderPassExecutionContext.h"
#include "Renderer/Common/SceneRendererTypes.h"

namespace Astral {

    /**
     * @brief Defines the tone mapping render pass execution context and logic
     */
    class ToneMappingRenderPass : public RenderPassExecutionContext
    {
    public:
        virtual ~ToneMappingRenderPass() = default;

        /**
         * @brief Initializes the tone mapping render pass' context and resources
         */
        void Init(uint32 numInFlightImages) override;

        /**
         * @brief Executes the tone mapping render pass' logic and submits commands to a command buffer
         * @param renderGraphPassExecutionContext The execution context of the render graph
         */
        void Execute(RenderGraphPassExecutionContext& renderGraphPassExecutionContext, SharedFrameContext& sharedFrameContext) override;

        /**
         * @brief Shuts down the tone mapping render pass' context and cleans up its resources
         */
        void Shutdown() override;

    private:

        // ACES Color Transforms and Tone Mapping
        DescriptorSetHandle m_RTT_ODT_LUT_DescriptorSet;
    };

}

