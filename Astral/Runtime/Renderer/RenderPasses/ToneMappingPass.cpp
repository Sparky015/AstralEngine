/**
* @file ToneMappingPass.cpp
* @author Andrew Fagan
* @date 6/4/26
*/

#include "ToneMappingPass.h"

#include "Asset/AssetManager.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/Common/CubeLUT.h"
#include "Renderer/RHI/RendererAPI.h"

namespace Astral {

    void ToneMappingRenderPass::Init(uint32 numInFlightImages)
    {
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        Ref<CubeLUT> toneMappingLUT = registry.CreateAsset<CubeLUT>("LUTs/ACEScg_to_sRGB_RRT_ODT.cube");
        m_RTT_ODT_LUT_DescriptorSet = RendererAPI::GetDevice().CreateDescriptorSet();
        m_RTT_ODT_LUT_DescriptorSet->BeginBuildingSet();
        m_RTT_ODT_LUT_DescriptorSet->AddDescriptorImageSampler(toneMappingLUT->LUT3D, ShaderStage::FRAGMENT);
        m_RTT_ODT_LUT_DescriptorSet->AddDescriptorImageSampler(toneMappingLUT->Shaper1D, ShaderStage::FRAGMENT);
        m_RTT_ODT_LUT_DescriptorSet->EndBuildingSet();
    }


    // For demo purposes to show benefits of ACES (ACES is used normally)
    enum class ToneMappingDebugView : uint32
    {
        NO_DEBUG_VIEW,
        DEBUG_VIEW_REINHARD,
        DEBUG_VIEW_NO_TONE_MAPPING,
    };


    void ToneMappingRenderPass::Execute(RenderGraphPassExecutionContext& renderGraphPassExecutionContext, SharedFrameContext& sharedFrameContext)
    {
        PROFILE_SCOPE("SceneRendererImpl::ToneMappingPass")

        const RendererSettings& rendererSettings = SceneRenderer::GetRendererSettings();

        struct ToneMappingPassPushConstants
        {
            float Exposure;
            Vec2 ShaperInputRange;
            ToneMappingDebugView ToneMappingDebugView;
        };

        CommandBufferHandle commandBuffer = renderGraphPassExecutionContext.CommandBuffer;

        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
        Ref<CubeLUT> toneMappingLUT = registry.CreateAsset<CubeLUT>("LUTs/ACEScg_to_sRGB_RRT_ODT.cube");

        Ref<Mesh> quadMesh = registry.CreateAsset<Mesh>("Meshes/Quad.obj");
        quadMesh->VertexShader = registry.CreateAsset<Shader>("Shaders/NoTransform.vert");
        sharedFrameContext.MainList.GetMeshes().push_back(quadMesh); // Hold onto reference so it is not destroyed early

        PipelineStateCache& pipelineStateCache = RendererAPI::GetContext().GetPipelineStateCache();
        pipelineStateCache.SetDescriptorSetStack({sharedFrameContext.SceneDataDescriptorSet, renderGraphPassExecutionContext.ReadAttachments});

        Material toneMapperMaterial{};
        toneMapperMaterial.FragmentShader = registry.CreateAsset<Shader>("Shaders/ToneMapping.frag");
        toneMapperMaterial.DescriptorSet = m_RTT_ODT_LUT_DescriptorSet;

        PipelineStateHandle toneMappingPipeline = pipelineStateCache.GetGraphicsPipeline(renderGraphPassExecutionContext.RenderPass, toneMapperMaterial, *quadMesh, 0, CullMode::NONE);
        commandBuffer->BindPipeline(toneMappingPipeline);
        commandBuffer->SetViewportAndScissor(renderGraphPassExecutionContext.ViewportSize);

        commandBuffer->BindDescriptorSet(sharedFrameContext.SceneDataDescriptorSet, 0);
        commandBuffer->BindDescriptorSet(renderGraphPassExecutionContext.ReadAttachments, 1);
        commandBuffer->BindDescriptorSet(toneMapperMaterial.DescriptorSet, 2);

        ToneMappingPassPushConstants toneMappingPushConstants;
        toneMappingPushConstants.Exposure = sharedFrameContext.SceneDescription.Exposure;
        toneMappingPushConstants.ShaperInputRange = toneMappingLUT->ShaperInputRange;
        toneMappingPushConstants.ToneMappingDebugView = ToneMappingDebugView::NO_DEBUG_VIEW;


        if (rendererSettings.DebugView == RendererDebugView::TONE_MAPPING_REINHARD)
        {
            toneMappingPushConstants.ToneMappingDebugView = ToneMappingDebugView::DEBUG_VIEW_REINHARD;
        }
        else if (rendererSettings.DebugView == RendererDebugView::TONE_MAPPING_NONE)
        {
            toneMappingPushConstants.ToneMappingDebugView = ToneMappingDebugView::DEBUG_VIEW_NO_TONE_MAPPING;
        }

        commandBuffer->PushConstants(&toneMappingPushConstants, sizeof(toneMappingPushConstants));

        commandBuffer->BindVertexBuffer(quadMesh->VertexBuffer);
        commandBuffer->BindIndexBuffer(quadMesh->IndexBuffer);

        commandBuffer->DrawElementsIndexed(quadMesh->IndexBuffer);

        pipelineStateCache.SetDescriptorSetStack(sharedFrameContext.SceneDataDescriptorSet);
    }


    void ToneMappingRenderPass::Shutdown()
    {

    }

}
