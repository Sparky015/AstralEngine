/**
* @file CascadedShadowMapRenderPass.h
* @author Andrew Fagan
* @date 6/1/26
*/

#pragma once

#include "ECS/Components/PointLightComponent.h"
#include "RenderPassExecutionContext.h"
#include "Renderer/Common/Material.h"
#include "Renderer/Common/Mesh.h"
#include "Renderer/Common/SceneRendererTypes.h"
#include "Renderer/RHI/Resources/CommandBuffer.h"
#include "Renderer/RHI/Resources/PipelineStateCache.h"

namespace Astral {

    /**
     * @brief Defines the cascaded shadow map render pass execution context and logic
     */
    class CascadedShadowMapRenderPass : public RenderPassExecutionContext
    {
    public:
        virtual ~CascadedShadowMapRenderPass() = default;

        /**
         * @brief Initializes a render pass' context and resources
         */
        void Init(uint32 numInFlightImages) override;

        /**
         * @brief Executes the cascaded shadow map render pass' logic and submits commands to a command buffer
         * @param renderGraphPassExecutionContext The execution context of the render graph
         */
        void Execute(RenderGraphPassExecutionContext& renderGraphPassExecutionContext, SharedFrameContext& sharedFrameContext) override;

        /**
         * @brief Shuts down the render pass' context and cleans up its resources
         */
        void Shutdown() override;

    private:

        /**
         * @brief Calculates the frustum corner positions in world space
         * @param projectionView The projection view matrix of the frustum
         * @return The positions of the frustum corners
         */
        std::vector<Vec4> GetFrustumCornersWorldSpace(const Mat4& projectionView);

        /**
         * @brief Calculates the z far value of the given cascade based on the overall z near and z far values of the shadow camera
         * @param zNear The overall z near value of the shadow camera
         * @param zFar The overall z far value of the shadow camera
         * @param cascadeNum The number of the cascade to calculate the z far value for
         * @param totalCascades The total number of cascades in the cascaded shadow map
         */
        float CalcCascadeZFar(float zNear, float zFar, float cascadeNum, float totalCascades);

        // /**
        //  * @brief Resources needed by the cascaded shadow map render pass
        //  */
        // struct CascadedShadowMapResources
        // {
        //     BufferHandle ShadowLightMatrices;
        //     DescriptorSetHandle ShadowLightMatricesDescriptorSet;
        // };
        //
        // std::vector<CascadedShadowMapResources> m_BufferedResources;
        // uint32 m_NextBufferIndex{0}; // The next index to use into m_BufferedResources


        ShaderHandle m_FragmentDepthWriteOnlyShader;
        ShaderHandle m_VertexCascadedShadowMapShader;

        Light m_FirstDirectionalLightInScene = {};
        std::vector<Mat4> m_LightSpaceMatrices{};
    };

}

