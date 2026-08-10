/**
* @file EnvironmentMapPass.cpp
* @author Andrew Fagan
* @date 6/2/26
*/

#include "EnvironmentMapPass.h"

#include "Renderer/RendererManager.h"
#include "Renderer/RHI/RendererAPI.h"
#include "Scenes/SceneManager.h"

namespace Astral {

    EnvironmentMapRenderPass::EnvironmentMapRenderPass() :
        m_MSAASampleCount(SampleCount::SAMPLE_1_BIT)
    {
    }


    void EnvironmentMapRenderPass::Init(uint32 numInFlightImages)
    {

    }


    void EnvironmentMapRenderPass::Execute(RenderGraphPassExecutionContext& renderGraphPassExecutionContext, SharedFrameContext& sharedFrameContext)
    {
        PROFILE_SCOPE("SceneRendererImpl::EnvironmentMapPass")

        CommandBufferHandle commandBuffer = renderGraphPassExecutionContext.CommandBuffer;

        // Cubemap
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        Scene& activeScene = Engine::Get().GetSceneManager().GetActiveScene();

        Ref<Mesh> cubemapMesh = registry.CreateAsset<Mesh>("Meshes/Cube.obj");

        GraphicsPipelineStateConfiguration pipelineConfig = {};
        pipelineConfig.RenderPass = renderGraphPassExecutionContext.RenderPass;
        pipelineConfig.VertexShader = registry.CreateAsset<Shader>("Shaders/Cubemap.vert");;
        pipelineConfig.FragmentShader = registry.CreateAsset<Shader>("Shaders/EnvironmentMap.frag");
        pipelineConfig.ShaderDataLayout = sharedFrameContext.EnvironmentMapDescriptorSet->GetDescriptorSetLayout();
        pipelineConfig.VertexBufferLayout = cubemapMesh->VertexBuffer->GetBufferLayout();
        pipelineConfig.IsAlphaBlended = false;
        pipelineConfig.CullMode = CullMode::NONE;
        pipelineConfig.MSAASampleCount = m_MSAASampleCount;

        std::vector<DescriptorSetHandle> descriptorSetStack = {sharedFrameContext.SceneDataDescriptorSet, sharedFrameContext.EnvironmentMapDescriptorSet};


        PipelineStateCache& pipelineStateCache = RendererAPI::GetContext().GetPipelineStateCache();
        PipelineStateHandle cubemapPipeline = pipelineStateCache.GetGraphicsPipeline(pipelineConfig, descriptorSetStack);
        commandBuffer->BindPipeline(cubemapPipeline);
        commandBuffer->SetViewportAndScissor(renderGraphPassExecutionContext.ViewportSize);

        commandBuffer->BindDescriptorSet(sharedFrameContext.SceneDataDescriptorSet, 0);
        commandBuffer->BindDescriptorSet(sharedFrameContext.EnvironmentMapDescriptorSet, 1);

        commandBuffer->BindVertexBuffer(cubemapMesh->VertexBuffer);
        commandBuffer->BindIndexBuffer(cubemapMesh->IndexBuffer);

        commandBuffer->PushConstants(&activeScene.EnvironmentMapBlur, sizeof(activeScene.EnvironmentMapBlur));
        commandBuffer->DrawElementsIndexed(cubemapMesh->IndexBuffer);
    }


    void EnvironmentMapRenderPass::Shutdown()
    {

    }


    void EnvironmentMapRenderPass::SetMSAASampleCount(SampleCount msaaSampleCount)
    {
        m_MSAASampleCount = msaaSampleCount;
    }

}
