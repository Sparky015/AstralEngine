/**
* @file DeferredGeometryRenderPass.cpp
* @author Andrew Fagan
* @date 6/4/26
*/

#include "DeferredGeometryRenderPass.h"

#include "Asset/AssetManager.h"
#include "Renderer/RHI/RendererAPI.h"

namespace Astral {

    void DeferredGeometryRenderPass::Init(uint32 numInFlightImages)
    {
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        m_DeferredGeometryPassUnpackedShader = registry.CreateAsset<Shader>("Shaders/DeferredGeometryPassUnpacked.frag");
        m_DeferredGeometryPassORMShader = registry.CreateAsset<Shader>("Shaders/DeferredGeometryPassORM.frag");
    }


    void DeferredGeometryRenderPass::Execute(RenderGraphPassExecutionContext& renderGraphPassExecutionContext, SharedFrameContext& sharedFrameContext)
    {
        PROFILE_SCOPE("SceneRendererImpl::GeometryPass")

        struct GeometryPassPushData
        {
            Mat4 ModelMatrix;
            uint32 HasNormalMap;
            uint32 HasDirectXNormals;
        };
        static_assert(sizeof(GeometryPassPushData) <= MaxPushConstantRange, "Push constant can not be greater than MaxPushConstantRange (usually 128) bytes in size");


        CommandBufferHandle commandBuffer = renderGraphPassExecutionContext.CommandBuffer;
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        for (uint32 i = 0; i < sharedFrameContext.MainList.Size(); i++)
        {
            Mesh& mesh = *sharedFrameContext.MainList.GetMeshes()[i];
            Material& material = *sharedFrameContext.MainList.GetMaterials()[i];

            if (material.ShaderModel != ShaderModel::PBR) { continue; }

            DescriptorSetHandle& materialDescriptorSet = material.DescriptorSet;

            if (material.TextureConvention == TextureConvention::UNPACKED)
            {
                material.FragmentShader = m_DeferredGeometryPassUnpackedShader;
            }
            else if (material.TextureConvention == TextureConvention::ORM_PACKED)
            {
                material.FragmentShader = m_DeferredGeometryPassORMShader;
            }

            PipelineStateCache& pipelineStateCache = RendererAPI::GetContext().GetPipelineStateCache();
            PipelineStateHandle pipeline = pipelineStateCache.GetGraphicsPipeline(renderGraphPassExecutionContext.RenderPass, material, mesh, 0, CullMode::NONE, {sharedFrameContext.SceneDataDescriptorSet});
            commandBuffer->BindPipeline(pipeline);
            commandBuffer->SetViewportAndScissor(renderGraphPassExecutionContext.ViewportSize);

            GeometryPassPushData pushConstantData = {
                .ModelMatrix = sharedFrameContext.MainList.GetTransforms()[i],
                .HasNormalMap = material.HasNormalMap,
                .HasDirectXNormals = material.HasDirectXNormals
            };

            commandBuffer->PushConstants(&pushConstantData, sizeof(GeometryPassPushData));

            commandBuffer->BindDescriptorSet(sharedFrameContext.SceneDataDescriptorSet, 0);
            commandBuffer->BindDescriptorSet(materialDescriptorSet, 1);

            commandBuffer->BindVertexBuffer(mesh.VertexBuffer);
            commandBuffer->BindIndexBuffer(mesh.IndexBuffer);

            commandBuffer->DrawElementsIndexed(mesh.IndexBuffer);
        }

    }


    void DeferredGeometryRenderPass::Shutdown()
    {

    }

}
