/**
* @file CascadedShadowMapRenderPass.cpp
* @author Andrew Fagan
* @date 6/1/26
*/
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "CascadedShadowMapRenderPass.h"

#include "Asset/AssetManager.h"
#include "Core/Engine.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/Cameras/Camera.h"
#include "Renderer/RenderGraph/RenderGraph.h"
#include "Renderer/RHI/RendererAPI.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Astral {

    void CascadedShadowMapRenderPass::Init(uint32 numInFlightImages)
    {
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        m_FragmentDepthWriteOnlyShader = registry.CreateAsset<Shader>("Shaders/DepthWriteOnly.frag");
        m_VertexCascadedShadowMapShader = registry.CreateAsset<Shader>("Shaders/ShadowMap.vert");

        // Device& device = RendererAPI::GetDevice();

        // for (uint32 i = 0; i < numInFlightImages; i++)
        // {
        //     m_BufferedResources.emplace_back(CascadedShadowMapResources());
        //     CascadedShadowMapResources& resourcesBuffer = m_BufferedResources[i];
        //
        //
        //     resourcesBuffer.ShadowLightMatrices = device.CreateUniformBuffer(nullptr, sizeof(Mat4) * 8, GPUMemoryType::HOST_VISIBLE);
        //     resourcesBuffer.ShadowLightMatricesDescriptorSet = device.CreateDescriptorSet();
        //     resourcesBuffer.ShadowLightMatricesDescriptorSet->BeginBuildingSet();
        //     resourcesBuffer.ShadowLightMatricesDescriptorSet->AddDescriptorUniformBuffer(resourcesBuffer.ShadowLightMatrices, ShaderStage::ALL);
        //     resourcesBuffer.ShadowLightMatricesDescriptorSet->EndBuildingSet();
        // }

    }


    void CascadedShadowMapRenderPass::Execute(RenderGraphPassExecutionContext& renderGraphPassExecutionContext, SharedFrameContext& sharedFrameContext)
    {
        PROFILE_SCOPE("CascadedShadowMapRenderPass::Execute")

        const RendererSettings& rendererSettings = SceneRenderer::GetRendererSettings();

        for (const Light& light : sharedFrameContext.SceneDescription.Lights)
        {
            if (light.LightType == LightType::DIRECTIONAL)
            {
                m_FirstDirectionalLightInScene = light;
            }
        }

        if (!rendererSettings.IsShadowsOn) { return; }
        if (m_FirstDirectionalLightInScene.LightType != LightType::DIRECTIONAL) { return; }

        const RenderGraphPassExecutionContext& executionContext = renderGraphPassExecutionContext;
        SharedFrameContext& frameContext = sharedFrameContext;
        CommandBufferHandle commandBuffer = executionContext.CommandBuffer;
        Camera& sceneCamera = sharedFrameContext.SceneDescription.Camera;

        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        m_LightSpaceMatrices.clear();

        float zNear = sceneCamera.GetNearPlane();
        float zFar = sceneCamera.GetFarPlane();
        std::vector<float> frustumRanges;
        frustumRanges.reserve(rendererSettings.NumShadowCascades);
        frustumRanges.push_back(zNear);


        for (uint32 i = 0; i < rendererSettings.NumShadowCascades; i++)
        {
            float cascadeZFar = CalcCascadeZFar(zNear, zFar, i + 1, rendererSettings.NumShadowCascades);
            frustumRanges.push_back(cascadeZFar);
            Camera subfrustumCamera = Camera(CameraType::PERSPECTIVE, sceneCamera.GetAspectRatio(), frustumRanges[i], cascadeZFar);
            subfrustumCamera.SetPosition(sceneCamera.GetPosition());
            subfrustumCamera.SetRotation(sceneCamera.GetRotation());
            std::vector<Vec4> frustumCorners = GetFrustumCornersWorldSpace(subfrustumCamera.GetViewProjectionMatrix());


            // Find center of frustum

            Vec3 center = Vec3(0.0f);
            for (auto& cornerPosition : frustumCorners)
            {
                center += Vec3(cornerPosition);
            }
            center /= frustumCorners.size();

            const Vec3 lightDir = glm::normalize(m_FirstDirectionalLightInScene.Position);
            Vec3 up = Vec3(0.0f, 1.0f, 0.0f);

            // Check if the light direction is parallel to the default up vector
            if (glm::abs(glm::dot(lightDir, up)) > 0.999f)
            {
                up = Vec3(0.0f, 0.0f, 1.0f);
            }

            Mat4 lightView = glm::lookAt(center - lightDir,
                               center,
                               up);


            // Get the min and max positions of the frustum in world space
            float minX = std::numeric_limits<float>::max();
            float maxX = std::numeric_limits<float>::lowest();
            float minY = std::numeric_limits<float>::max();
            float maxY = std::numeric_limits<float>::lowest();
            float minZ = std::numeric_limits<float>::max();
            float maxZ = std::numeric_limits<float>::lowest();
            for (const auto& cornerPosition : frustumCorners)
            {
                const Vec4 trf = lightView * cornerPosition;
                minX = std::min(minX, trf.x);
                maxX = std::max(maxX, trf.x);
                minY = std::min(minY, trf.y);
                maxY = std::max(maxY, trf.y);
                minZ = std::min(minZ, trf.z);
                maxZ = std::max(maxZ, trf.z);
            }


            // Snap to texel boundaries for stability
            float shadowMapSize = rendererSettings.ShadowMapResolution;
            float texelSizeX = (maxX - minX) / shadowMapSize;
            float texelSizeY = (maxY - minY) / shadowMapSize;

            minX = floor(minX / texelSizeX) * texelSizeX;
            maxX = ceil(maxX / texelSizeX) * texelSizeX;
            minY = floor(minY / texelSizeY) * texelSizeY;
            maxY = ceil(maxY / texelSizeY) * texelSizeY;

            float zMult = rendererSettings.ShadowMapZMultiplier;
            const Mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ * zMult, maxZ * zMult);


            m_LightSpaceMatrices.push_back(lightProjection * lightView);
        }

        frameContext.ShadowLightMatrices->CopyDataToBuffer(m_LightSpaceMatrices.data(), sizeof(Mat4) * m_LightSpaceMatrices.size());

        GraphicsPipelineStateConfiguration pipelineConfig = {};
        pipelineConfig.RenderPass = renderGraphPassExecutionContext.RenderPass;
        pipelineConfig.VertexShader = m_VertexCascadedShadowMapShader;
        pipelineConfig.FragmentShader = m_FragmentDepthWriteOnlyShader;
        pipelineConfig.ShaderDataLayout = frameContext.ShadowLightMatricesDescriptorSet->GetDescriptorSetLayout();
        pipelineConfig.VertexBufferLayout = {};
        pipelineConfig.IsAlphaBlended = false;
        pipelineConfig.CullMode = CullMode::FRONT;
        pipelineConfig.MSAASampleCount = SampleCount::SAMPLE_1_BIT;

        std::vector<DescriptorSetHandle> descriptorSetStack = {frameContext.SceneDataDescriptorSet, frameContext.ShadowLightMatricesDescriptorSet};

        for (uint32 i = 0; i < frameContext.ShadowMapList.Size(); i++)
        {
            Ref<Material>& material = frameContext.ShadowMapList.GetMaterials()[i];
            if (material->ShaderModel != ShaderModel::PBR) { continue; }

            Ref<Mesh>& mesh = frameContext.ShadowMapList.GetMeshes()[i];
            if (pipelineConfig.VertexBufferLayout != mesh->VertexBuffer->GetBufferLayout())
            {
                pipelineConfig.VertexBufferLayout = mesh->VertexBuffer->GetBufferLayout();
            }

            PipelineStateCache& pipelineStateCache = RendererAPI::GetContext().GetPipelineStateCache();
            PipelineStateHandle shadowMapPipeline = pipelineStateCache.GetGraphicsPipeline(pipelineConfig, descriptorSetStack);

            commandBuffer->BindPipeline(shadowMapPipeline);
            commandBuffer->SetViewportAndScissor(Vec2(rendererSettings.ShadowMapResolution));

            commandBuffer->BindDescriptorSet(frameContext.SceneDataDescriptorSet, 0);
            commandBuffer->BindDescriptorSet(frameContext.ShadowLightMatricesDescriptorSet, 1);


            commandBuffer->BindVertexBuffer(mesh->VertexBuffer);
            commandBuffer->BindIndexBuffer(mesh->IndexBuffer);


            commandBuffer->PushConstants(&frameContext.ShadowMapList.GetTransforms()[i], sizeof(Mat4));

            commandBuffer->DrawElementsInstanced(mesh->IndexBuffer, rendererSettings.NumShadowCascades);
        }
    }


    void CascadedShadowMapRenderPass::Shutdown()
    {

    }


    std::vector<Vec4> CascadedShadowMapRenderPass::GetFrustumCornersWorldSpace(const Mat4& projectionView)
    {
        const Mat4 cameraInverse = glm::inverse(projectionView);

        std::vector<Vec4> frustumCorners;

        // Loops to get each corner of the frustum
        for (unsigned int x = 0; x < 2; ++x)
        {
            for (unsigned int y = 0; y < 2; ++y)
            {
                for (unsigned int z = 0; z < 2; ++z)
                {
                    const Vec4 point =
                        cameraInverse * Vec4(
                            2.0f * x - 1.0f,
                            2.0f * y - 1.0f,
                            2.0f * z - 1.0f,
                            1.0f);
                    frustumCorners.push_back(point / point.w);
                }
            }
        }

        return frustumCorners;
    }


    float CascadedShadowMapRenderPass::CalcCascadeZFar(float zNear, float zFar, float cascadeNum, float totalCascades)
    {
        float blendFactor = SceneRenderer::GetRendererSettings().ShadowMapCascadeLogFactor;

        const float logComponent = blendFactor * (zNear * std::pow((zFar / zNear), cascadeNum / totalCascades));
        const float linearComponent = (1 - blendFactor) * (zNear + cascadeNum / totalCascades * (zFar - zNear));
        return logComponent + linearComponent;
    }

}

