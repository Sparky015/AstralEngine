/**
* @file RenderingPath.h
* @author Andrew Fagan
* @date 6/4/26
*/

#pragma once

#include "Renderer/RenderGraph/RenderGraph.h"

namespace Astral {

    /**
     * @brief Defines an interface organizing render passes for a renderer path
     */
    class RenderingPath
    {
    public:
        virtual ~RenderingPath() = default;

        /**
         * @brief Initializes the render passes' execution context
         * @param numInFlightImages The number of in flight images
         */
        virtual void InitRenderPasses(uint32 numInFlightImages) = 0;

        /**
         * @brief Shuts down the render passes' execution context
         */
        virtual void ShutdownRenderPasses() = 0;

        /**
         * @brief Defines render pass resources and builds the render passes into a render graph
         */
        virtual void BuildRenderGraph(RenderGraph& outRenderGraph, const std::vector<TextureHandle>& outputTextures) = 0;

    private:

    };

}
