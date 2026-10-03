/**
* @file EnvironmentMapPass.h
* @author Andrew Fagan
* @date 6/2/26
*/

#pragma once

#include "RenderPassExecutionContext.h"
#include "Renderer/Common/SceneRendererTypes.h"

namespace Astral {

    /**
     * @brief Defines the environment map render pass execution context and logic
     */
    class EnvironmentMapRenderPass : public RenderPassExecutionContext
    {
    public:
        EnvironmentMapRenderPass();
        virtual ~EnvironmentMapRenderPass() = default;

        /**
         * @brief Initializes a render pass' context and resources
         */
        void Init(uint32 numInFlightImages) override;

        /**
         * @brief Executes the environment map render pass' logic and submits commands to a command buffer
         * @param renderGraphPassExecutionContext The execution context of the render graph
         */
        void Execute(RenderGraphPassExecutionContext& renderGraphPassExecutionContext, SharedFrameContext& sharedFrameContext) override;

        /**
         * @brief Shuts down the render pass' context and cleans up its resources
         */
        void Shutdown() override;

        /**
         * @brief Sets the MSAA sample count for the color attachment the environment render pass should expect to render to
         */
        void SetMSAASampleCount(SampleCount msaaSampleCount);

    private:

        SampleCount m_MSAASampleCount;
    };

}
