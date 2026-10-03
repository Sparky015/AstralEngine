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


        GraphicsPipelineStateConfiguration pipelineConfig = {};
        pipelineConfig.RenderPass = renderGraphPassExecutionContext.RenderPass;
        pipelineConfig.VertexShader = nullptr;
        pipelineConfig.FragmentShader = nullptr;
        pipelineConfig.ShaderDataLayout ={};
        pipelineConfig.VertexBufferLayout = {};
        pipelineConfig.IsAlphaBlended = false;
        pipelineConfig.CullMode = CullMode::NONE;
        pipelineConfig.MSAASampleCount = SampleCount::SAMPLE_1_BIT;

        std::vector<DescriptorSetHandle> descriptorSetStack = {sharedFrameContext.SceneDataDescriptorSet, nullptr};


        for (uint32 i = 0; i < sharedFrameContext.MainList.Size(); i++)
        {
            Mesh& mesh = *sharedFrameContext.MainList.GetMeshes()[i];
            Material& material = *sharedFrameContext.MainList.GetMaterials()[i];

            if (material.ShaderModel != ShaderModel::PBR) { continue; }

            pipelineConfig.VertexBufferLayout = mesh.VertexBuffer->GetBufferLayout();
            pipelineConfig.VertexShader = mesh.VertexShader;
            descriptorSetStack[1] = material.DescriptorSet;
            pipelineConfig.ShaderDataLayout = material.DescriptorSet->GetDescriptorSetLayout();

            if (material.TextureConvention == TextureConvention::UNPACKED)
            {
                pipelineConfig.FragmentShader = m_DeferredGeometryPassUnpackedShader;
            }
            else if (material.TextureConvention == TextureConvention::ORM_PACKED)
            {
                pipelineConfig.FragmentShader = m_DeferredGeometryPassORMShader;
            }

            PipelineStateCache& pipelineStateCache = RendererAPI::GetContext().GetPipelineStateCache();
            PipelineStateHandle pipeline = pipelineStateCache.GetGraphicsPipeline(pipelineConfig, descriptorSetStack);
            commandBuffer->BindPipeline(pipeline);
            commandBuffer->SetViewportAndScissor(renderGraphPassExecutionContext.ViewportSize);

            GeometryPassPushData pushConstantData = {
                .ModelMatrix = sharedFrameContext.MainList.GetTransforms()[i],
                .HasNormalMap = material.HasNormalMap,
                .HasDirectXNormals = material.HasDirectXNormals
            };

            commandBuffer->PushConstants(&pushConstantData, sizeof(GeometryPassPushData));

            commandBuffer->BindDescriptorSet(sharedFrameContext.SceneDataDescriptorSet, 0);
            commandBuffer->BindDescriptorSet(material.DescriptorSet, 1);

            commandBuffer->BindVertexBuffer(mesh.VertexBuffer);
            commandBuffer->BindIndexBuffer(mesh.IndexBuffer);

            commandBuffer->DrawElementsIndexed(mesh.IndexBuffer);
        }

    }


    void DeferredGeometryRenderPass::Shutdown()
    {

    }

}
