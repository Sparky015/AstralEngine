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

        GraphicsPipelineStateConfiguration pipelineConfig = {};
        pipelineConfig.RenderPass = renderGraphPassExecutionContext.RenderPass;
        pipelineConfig.VertexShader = nullptr;
        pipelineConfig.FragmentShader = m_DepthWriteOnlyShader;
        pipelineConfig.ShaderDataLayout = sharedFrameContext.SceneDataDescriptorSet->GetDescriptorSetLayout();
        pipelineConfig.VertexBufferLayout = {};
        pipelineConfig.IsAlphaBlended = false;
        pipelineConfig.CullMode = CullMode::NONE;
        pipelineConfig.MSAASampleCount = SampleCount::SAMPLE_4_BIT;

        std::vector<DescriptorSetHandle> descriptorSetStack = {sharedFrameContext.SceneDataDescriptorSet};

        for (uint32 i = 0; i < sharedFrameContext.MainList.Size(); i++)
        {
            Mesh& mesh = *sharedFrameContext.MainList.GetMeshes()[i];
            Material& material = *sharedFrameContext.MainList.GetMaterials()[i];

            if (material.ShaderModel != ShaderModel::PBR) { continue; }
            if (material.DescriptorSet == nullptr) { continue; }

            pipelineConfig.VertexBufferLayout = mesh.VertexBuffer->GetBufferLayout();
            pipelineConfig.VertexShader = mesh.VertexShader;

            PipelineStateCache& pipelineStateCache = RendererAPI::GetContext().GetPipelineStateCache();
            PipelineStateHandle pipeline = pipelineStateCache.GetGraphicsPipeline(pipelineConfig, descriptorSetStack);


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
