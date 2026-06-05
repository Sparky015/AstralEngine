/**
* @file ForwardLightingRenderPass.cpp
* @author Andrew Fagan
* @date 6/4/26
*/

#include "ForwardLightingRenderPass.h"

#include "Asset/AssetManager.h"
#include "Renderer/RHI/RendererAPI.h"

namespace Astral {

    void ForwardLightingRenderPass::Init(uint32 numInFlightImages)
    {
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        m_ForwardUnpackedLightingShader = registry.CreateAsset<Shader>("Shaders/ForwardLightingPassUnpacked.frag");
        m_ForwardORMLightingShader = registry.CreateAsset<Shader>("Shaders/ForwardLightingPassORM.frag");
    }


    void ForwardLightingRenderPass::Execute(RenderGraphPassExecutionContext& renderGraphPassExecutionContext, SharedFrameContext& sharedFrameContext)
    {
        PROFILE_SCOPE("SceneRendererImpl::ForwardLightingPass")

        struct ForwardLightingPassPushData
        {
            Mat4 ModelMatrix;
            uint32 HasNormalMap;
            uint32 HasDirectXNormals;
            float CameraZNear;
            float CameraZFar;
            int32 NumShadowCascades;
            uint32 ShowCascadeDebugView;
            float ShadowMapBias;
            float ShadowMapCascadeLogFactor;
        };
        static_assert(sizeof(ForwardLightingPassPushData) <= MaxPushConstantRange, "Push constant can not be greater than MaxPushConstantRange (usually 128) bytes in size");


        CommandBufferHandle commandBuffer = renderGraphPassExecutionContext.CommandBuffer;
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        PipelineStateCache& pipelineStateCache = RendererAPI::GetContext().GetPipelineStateCache();
        pipelineStateCache.SetDescriptorSetStack({sharedFrameContext.SceneDataDescriptorSet, sharedFrameContext.EnvironmentMapDescriptorSet, renderGraphPassExecutionContext.ReadAttachments, sharedFrameContext.ShadowLightMatricesDescriptorSet});


        for (uint32 i = 0; i < sharedFrameContext.MainList.Size(); i++)
        {
            Mesh& mesh = *sharedFrameContext.MainList.GetMeshes()[i];
            Material& material = *sharedFrameContext.MainList.GetMaterials()[i];

            if (material.ShaderModel != ShaderModel::PBR) { continue; }

            if (material.DescriptorSet == nullptr) { continue; }

            if (material.TextureConvention == TextureConvention::UNPACKED)
            {
                material.FragmentShader = m_ForwardUnpackedLightingShader;
            }
            else if (material.TextureConvention == TextureConvention::ORM_PACKED)
            {
                material.FragmentShader = m_ForwardORMLightingShader;
            }

            Ref<Shader> vertexShader = mesh.VertexShader;

            PipelineStateHandle pipeline = pipelineStateCache.GetGraphicsPipeline(renderGraphPassExecutionContext.RenderPass, material, mesh, 0, CullMode::NONE, SampleCount::SAMPLE_4_BIT);
            commandBuffer->BindPipeline(pipeline);
            commandBuffer->SetViewportAndScissor(renderGraphPassExecutionContext.ViewportSize);

            commandBuffer->BindDescriptorSet(sharedFrameContext.SceneDataDescriptorSet, 0);
            commandBuffer->BindDescriptorSet(sharedFrameContext.EnvironmentMapDescriptorSet, 1);
            commandBuffer->BindDescriptorSet(renderGraphPassExecutionContext.ReadAttachments, 2);
            commandBuffer->BindDescriptorSet(sharedFrameContext.ShadowLightMatricesDescriptorSet, 3);
            commandBuffer->BindDescriptorSet(material.DescriptorSet, 4);

            commandBuffer->BindVertexBuffer(mesh.VertexBuffer);
            commandBuffer->BindIndexBuffer(mesh.IndexBuffer);


            const RendererSettings& rendererSettings = SceneRenderer::GetRendererSettings();
            const Camera& sceneCamera = sharedFrameContext.SceneDescription.Camera;

            ForwardLightingPassPushData pushConstantData = {
                .ModelMatrix = sharedFrameContext.MainList.GetTransforms()[i],
                .HasNormalMap = material.HasNormalMap,
                .HasDirectXNormals = material.HasDirectXNormals,
                .CameraZNear = sceneCamera.GetNearPlane(),
                .CameraZFar = sceneCamera.GetFarPlane(),
                .NumShadowCascades = rendererSettings.NumShadowCascades,
                .ShowCascadeDebugView = rendererSettings.DebugView == RendererDebugView::CASCADED_SHADOW_MAP_BOUNDARIES,
                .ShadowMapBias = rendererSettings.ShadowMapBias,
                .ShadowMapCascadeLogFactor = rendererSettings.ShadowMapCascadeLogFactor,
            };

            commandBuffer->PushConstants(&pushConstantData, sizeof(ForwardLightingPassPushData));

            commandBuffer->DrawElementsIndexed(mesh.IndexBuffer);
        }

        pipelineStateCache.SetDescriptorSetStack({sharedFrameContext.SceneDataDescriptorSet});
    }


    void ForwardLightingRenderPass::Shutdown()
    {

    }

}
