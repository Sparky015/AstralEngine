/**
* @file DeferredGeometryRenderPass.h
* @author Andrew Fagan
* @date 6/4/26
*/

#pragma once

#include "RenderPassExecutionContext.h"
#include "Renderer/Common/SceneRendererTypes.h"

namespace Astral {

    /**
     * @brief Defines the geometry render pass execution context and logic for the deferred rendering path
     */
    class DeferredGeometryRenderPass : public RenderPassExecutionContext
    {
    public:
        virtual ~DeferredGeometryRenderPass() = default;

        /**
         * @brief Initializes the geometry render pass' context and resources
         */
        void Init(uint32 numInFlightImages) override;

        /**
         * @brief Executes the geometry render pass' logic and submits commands to a command buffer
         * @param renderGraphPassExecutionContext The execution context of the render graph
         */
        void Execute(RenderGraphPassExecutionContext& renderGraphPassExecutionContext, SharedFrameContext& sharedFrameContext) override;

        /**
         * @brief Shuts down the geometry render pass' context and cleans up its resources
         */
        void Shutdown() override;

    private:

        ShaderHandle m_DeferredGeometryPassUnpackedShader;
        ShaderHandle m_DeferredGeometryPassORMShader;
    };

}

