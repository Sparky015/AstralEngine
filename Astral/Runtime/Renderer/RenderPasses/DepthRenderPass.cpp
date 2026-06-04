/**
* @file DepthRenderPass.cpp
* @author Andrew Fagan
* @date 6/2/26
*/

#include "DepthRenderPass.h"

#include "Asset/AssetManager.h"
#include "Renderer/RHI/RendererAPI.h"

namespace Astral {

    void DepthRenderPass::Init(uint32 numInFlightImages)
    {
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        m_DepthWriteOnlyShader = registry.CreateAsset<Shader>("Shaders/DepthWriteOnly.frag");
    }


    void DepthRenderPass::Execute(RenderGraphPassExecutionContext& renderGraphPassExecutionContext, SharedFrameContext& sharedFrameContext)
    {
        PROFILE_SCOPE("DepthRenderPass::Execute")

        CommandBufferHandle commandBuffer = renderGraphPassExecutionContext.CommandBuffer;
        DescriptorSetHandle materialDescriptorSetSave = nullptr;
        ShaderHandle materialShaderSave = nullptr;

        for (uint32 i = 0; i < sharedFrameContext.MainList.Size(); i++)
        {
            Mesh& mesh = *sharedFrameContext.MainList.GetMeshes()[i];
            Material& material = *sharedFrameContext.MainList.GetMaterials()[i];

            if (material.ShaderModel != ShaderModel::PBR) { continue; }

            if (material.DescriptorSet == nullptr) { continue; }
            materialDescriptorSetSave = material.DescriptorSet;
            material.DescriptorSet = nullptr;

            materialShaderSave = material.FragmentShader;
            material.FragmentShader = m_DepthWriteOnlyShader;

            PipelineStateCache& pipelineStateCache = RendererAPI::GetContext().GetPipelineStateCache();
            PipelineStateHandle pipeline = pipelineStateCache.GetGraphicsPipeline(renderGraphPassExecutionContext.RenderPass, material, mesh, 0, CullMode::NONE, SampleCount::SAMPLE_4_BIT);

            material.DescriptorSet = materialDescriptorSetSave;
            material.FragmentShader = materialShaderSave;

            commandBuffer->BindPipeline(pipeline);
            commandBuffer->SetViewportAndScissor(renderGraphPassExecutionContext.ViewportSize);

            commandBuffer->BindDescriptorSet(sharedFrameContext.SceneDataDescriptorSet, 0);

            commandBuffer->BindVertexBuffer(mesh.VertexBuffer);
            commandBuffer->BindIndexBuffer(mesh.IndexBuffer);

            commandBuffer->PushConstants(&sharedFrameContext.MainList.GetTransforms()[i], sizeof(sharedFrameContext.MainList.GetTransforms()[i]));

            commandBuffer->DrawElementsIndexed(mesh.IndexBuffer);
        }

    }


    void DepthRenderPass::Shutdown()
    {

    }

}
