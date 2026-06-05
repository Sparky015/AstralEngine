/**
* @file ForwardRenderingPath.h
* @author Andrew Fagan
* @date 6/4/26
*/

#pragma once

#include "RenderingPath.h"
#include "Renderer/RenderPasses/CascadedShadowMapRenderPass.h"
#include "Renderer/RenderPasses/DepthRenderPass.h"
#include "Renderer/RenderPasses/EnvironmentMapPass.h"
#include "Renderer/RenderPasses/ForwardLightingRenderPass.h"
#include "Renderer/RenderPasses/ToneMappingPass.h"

namespace Astral {

    /**
     * @brief Defines a forward rendering path
     */
    class ForwardRenderingPath : public RenderingPath
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
        void BuildRenderGraph(RenderGraph& outRenderGraph, const std::vector<TextureHandle>& outputTextures) override;

    private:

        DepthRenderPass m_DepthRenderPass;
        CascadedShadowMapRenderPass m_CascadedShadowMapRenderPass;
        ForwardLightingRenderPass m_ForwardLightingRenderPass;
        EnvironmentMapRenderPass m_EnvironmentMapRenderPass;
        ToneMappingRenderPass m_ToneMappingRenderPass;
    };



}
