/**
* @file DeferredRenderingPath.h
* @author Andrew Fagan
* @date 6/4/26
*/

#pragma once

#include "RenderingPath.h"
#include "Renderer/RenderPasses/CascadedShadowMapRenderPass.h"
#include "Renderer/RenderPasses/DeferredGeometryRenderPass.h"
#include "Renderer/RenderPasses/DeferredLightingRenderPass.h"
#include "Renderer/RenderPasses/EnvironmentMapPass.h"
#include "Renderer/RenderPasses/ToneMappingPass.h"

namespace Astral {

    class DeferredRenderingPath : public RenderingPath
    {
    public:

        /**
         * @brief Initializes the render passes' execution context
         * @param numInFlightImages The number of in flight images
         */
        void InitRenderPasses(uint32 numInFlightImages) override;

        /**
         * @brief Shuts down the render passes' execution context
         */
        void ShutdownRenderPasses() override;

        /**
         * @brief Defines render pass resources and builds the render passes into a render graph
         */
        void BuildRenderGraph(RenderGraph& outRenderGraph, UVec2 outputAttachmentDimensions) override;

    private:

        DeferredGeometryRenderPass m_DeferredGeometryRenderPass;
        CascadedShadowMapRenderPass m_CascadedShadowMapRenderPass;
        DeferredLightingRenderPass m_DeferredLightingRenderPass;
        EnvironmentMapRenderPass m_EnvironmentMapRenderPass;
        ToneMappingRenderPass m_ToneMappingRenderPass;
    };

}
