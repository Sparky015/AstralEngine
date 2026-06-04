/**
* @file DeferredLightingRenderPass.cpp
* @author Andrew Fagan
* @date 6/4/26
*/

#include "DeferredLightingRenderPass.h"

#include "Asset/AssetManager.h"
#include "Renderer/RHI/RendererAPI.h"

namespace Astral {

    void DeferredLightingRenderPass::Init(uint32 numInFlightImages)
    {
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        m_DeferredLightingShader = registry.CreateAsset<Shader>("Shaders/DeferredLightingPass.frag");
    }


    void DeferredLightingRenderPass::Execute(RenderGraphPassExecutionContext& renderGraphPassExecutionContext, SharedFrameContext& sharedFrameContext)
    {
        PROFILE_SCOPE("SceneRendererImpl::DeferredLightingPass")

        struct DeferredLightingPushConstants
        {
            float CameraZNear;
            float CameraZFar;
            int32 NumShadowCascades;
            uint32 ShowCascadeDebugView;
            float ShadowMapBias;
            float ShadowMapCascadeLogFactor;
        };

        CommandBufferHandle commandBuffer = renderGraphPassExecutionContext.CommandBuffer;
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        PipelineStateCache& pipelineStateCache = RendererAPI::GetContext().GetPipelineStateCache();
        pipelineStateCache.SetDescriptorSetStack({sharedFrameContext.SceneDataDescriptorSet, sharedFrameContext.EnvironmentMapDescriptorSet, sharedFrameContext.ShadowLightMatricesDescriptorSet});

        Ref<Mesh> mesh = registry.CreateAsset<Mesh>("Meshes/Quad.obj");
        mesh->VertexShader = registry.CreateAsset<Shader>("Shaders/NoTransform.vert");
        sharedFrameContext.MainList.GetMeshes().push_back(mesh); // Hold onto reference so it is not destroyed early
        Material material{};
        material.FragmentShader = m_DeferredLightingShader;
        material.DescriptorSet = renderGraphPassExecutionContext.ReadAttachments;

        Ref<Shader> vertexShader = mesh->VertexShader;

        PipelineStateHandle pipeline = pipelineStateCache.GetGraphicsPipeline(renderGraphPassExecutionContext.RenderPass, material, *mesh, 0, CullMode::NONE);
        commandBuffer->BindPipeline(pipeline);
        commandBuffer->SetViewportAndScissor(renderGraphPassExecutionContext.ViewportSize);

        commandBuffer->BindDescriptorSet(sharedFrameContext.SceneDataDescriptorSet, 0);
        commandBuffer->BindDescriptorSet(sharedFrameContext.EnvironmentMapDescriptorSet, 1);
        commandBuffer->BindDescriptorSet(sharedFrameContext.ShadowLightMatricesDescriptorSet, 2);
        commandBuffer->BindDescriptorSet(renderGraphPassExecutionContext.ReadAttachments, 3);

        commandBuffer->BindVertexBuffer(mesh->VertexBuffer);
        commandBuffer->BindIndexBuffer(mesh->IndexBuffer);

        const RendererSettings& rendererSettings = SceneRenderer::GetRendererSettings();

        DeferredLightingPushConstants deferredLightingPushConstants
        {
            .CameraZNear = sharedFrameContext.SceneDescription.Camera.GetNearPlane(),
            .CameraZFar = sharedFrameContext.SceneDescription.Camera.GetFarPlane(),
            .NumShadowCascades = rendererSettings.NumShadowCascades,
            .ShowCascadeDebugView = rendererSettings.DebugView == RendererDebugView::CASCADED_SHADOW_MAP_BOUNDARIES,
            .ShadowMapBias = rendererSettings.ShadowMapBias,
            .ShadowMapCascadeLogFactor = rendererSettings.ShadowMapCascadeLogFactor
        };

        commandBuffer->PushConstants(&deferredLightingPushConstants, sizeof(deferredLightingPushConstants));

        commandBuffer->DrawElementsIndexed(mesh->IndexBuffer);

        pipelineStateCache.SetDescriptorSetStack({sharedFrameContext.SceneDataDescriptorSet});
    }


    void DeferredLightingRenderPass::Shutdown()
    {

    }

}
