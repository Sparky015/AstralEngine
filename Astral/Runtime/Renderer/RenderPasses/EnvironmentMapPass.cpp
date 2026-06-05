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
        cubemapMesh->VertexShader = registry.CreateAsset<Shader>("Shaders/Cubemap.vert");
        sharedFrameContext.MainList.GetMeshes().push_back(cubemapMesh); // Hold onto reference so it is not destroyed early

        Material environmentMapMaterial{};
        environmentMapMaterial.FragmentShader = registry.CreateAsset<Shader>("Shaders/EnvironmentMap.frag");
        environmentMapMaterial.DescriptorSet = sharedFrameContext.EnvironmentMapDescriptorSet;

        PipelineStateCache& pipelineStateCache = RendererAPI::GetContext().GetPipelineStateCache();
        PipelineStateHandle cubemapPipeline = pipelineStateCache.GetGraphicsPipeline(renderGraphPassExecutionContext.RenderPass, environmentMapMaterial, *cubemapMesh, 0, CullMode::NONE, m_MSAASampleCount);
        commandBuffer->BindPipeline(cubemapPipeline);
        commandBuffer->SetViewportAndScissor(renderGraphPassExecutionContext.ViewportSize);

        commandBuffer->BindDescriptorSet(sharedFrameContext.SceneDataDescriptorSet, 0);
        commandBuffer->BindDescriptorSet(environmentMapMaterial.DescriptorSet, 1);

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
