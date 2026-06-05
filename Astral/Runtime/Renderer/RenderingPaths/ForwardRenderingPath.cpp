/**
* @file ForwardRenderingPath.cpp
* @author Andrew Fagan
* @date 6/4/26
*/

#include "ForwardRenderingPath.h"

#include "Renderer/SceneRenderer.h"

namespace Astral {

    void ForwardRenderingPath::InitRenderPasses(uint32 numInFlightImages)
    {
        m_DepthRenderPass.Init(numInFlightImages);
        m_CascadedShadowMapRenderPass.Init(numInFlightImages);
        m_ForwardLightingRenderPass.Init(numInFlightImages);
        m_EnvironmentMapRenderPass.Init(numInFlightImages);
        m_ToneMappingRenderPass.Init(numInFlightImages);
    }


    void ForwardRenderingPath::ShutdownRenderPasses()
    {
        m_ToneMappingRenderPass.Shutdown();
        m_EnvironmentMapRenderPass.Shutdown();
        m_ForwardLightingRenderPass.Shutdown();
        m_CascadedShadowMapRenderPass.Shutdown();
        m_DepthRenderPass.Shutdown();
    }


    static constexpr SampleCount ForwardMSAASampleCount = SampleCount::SAMPLE_4_BIT;

    void ForwardRenderingPath::BuildRenderGraph(RenderGraph& outRenderGraph, const std::vector<TextureHandle>& outputTextures)
    {
        const RendererSettings& rendererSettings = SceneRenderer::GetRendererSettings();

        AttachmentDescription depthMSAABufferDescription = {
            .Format = ImageFormat::D32_SFLOAT_S8_UINT,
            .ImageUsageFlags = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(1.0, 0.0, 0.0, 1.0),
            .MSAASamples = ForwardMSAASampleCount
        };

        RenderGraphPass depthPrePass = RenderGraphPass(
            RenderGraph_ViewportDimensions,
            "Depth Pre-Pass",
            [&](RenderGraphPassExecutionContext& renderPassGraphExecutionContext, SharedFrameContext& sharedFrameContext) {
                m_DepthRenderPass.Execute(renderPassGraphExecutionContext, sharedFrameContext);
            });

        depthPrePass.CreateDepthStencilAttachment(depthMSAABufferDescription, "Forward_Depth_MSSA_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);


        AttachmentDescription shadowMapBufferDescription = {
            .Format = ImageFormat::D32_SFLOAT_S8_UINT,
            .ImageUsageFlags = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(1.0, 0.0, 0.0, 1.0),
            .LayerCount = (uint32)rendererSettings.NumShadowCascades,
            .TextureType = TextureType::IMAGE_2D_ARRAY
        };

        RenderGraphPass shadowMapPass = RenderGraphPass(
            Vec3(rendererSettings.ShadowMapResolution, rendererSettings.ShadowMapResolution, rendererSettings.NumShadowCascades),
            "Shadow Map Pass",
            [&](RenderGraphPassExecutionContext& renderPassGraphExecutionContext, SharedFrameContext& sharedFrameContext) {
                m_CascadedShadowMapRenderPass.Execute(renderPassGraphExecutionContext, sharedFrameContext);
             });

        shadowMapPass.CreateDepthStencilAttachment(shadowMapBufferDescription, "Shadow_Map_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);


        AttachmentDescription lightingMSAATextureDescription = {
            .Format = ImageFormat::R16G16B16A16_SFLOAT,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .FinalLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0),
            .MSAASamples = ForwardMSAASampleCount,
        };


        RenderGraphPass lightingPass = RenderGraphPass(
            RenderGraph_ViewportDimensions,
            "Lighting Pass",
            [&](RenderGraphPassExecutionContext& renderPassGraphExecutionContext, SharedFrameContext& sharedFrameContext) {
            m_ForwardLightingRenderPass.Execute(renderPassGraphExecutionContext, sharedFrameContext);
            });

        lightingPass.LinkReadInputAttachment(&shadowMapPass, "Shadow_Map_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.CreateColorAttachment(lightingMSAATextureDescription, "Forward_Lighting_MSAA_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        lightingPass.LinkWriteInputAttachment(&depthPrePass, "Forward_Depth_MSSA_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        lightingPass.AddDependency(&shadowMapPass);

        AttachmentDescription lightingResolveTextureDescription = {
            .Format = ImageFormat::R16G16B16A16_SFLOAT,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::DONT_CARE,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .FinalLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0),
        };

        RenderGraphPass environmentMapPass = RenderGraphPass(
            RenderGraph_ViewportDimensions,
            "Environment Map Pass",
            [&](RenderGraphPassExecutionContext& renderPassGraphExecutionContext, SharedFrameContext& sharedFrameContext) {
                m_EnvironmentMapRenderPass.Execute(renderPassGraphExecutionContext, sharedFrameContext);
            });

        environmentMapPass.LinkWriteInputAttachment(&lightingPass, "Forward_Lighting_MSAA_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        environmentMapPass.CreateResolveAttachment(lightingResolveTextureDescription, "Forward_Lighting_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        environmentMapPass.LinkWriteInputAttachment(&depthPrePass, "Forward_Depth_MSSA_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        m_EnvironmentMapRenderPass.SetMSAASampleCount(SampleCount::SAMPLE_4_BIT);


        AttachmentDescription toneMappingOutputTextureDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 1.0, 1.0)
        };

        RenderGraphPass tonemappingPass = RenderGraphPass(
            RenderGraph_ViewportDimensions,
            "Tonemapping Pass",
            [&](RenderGraphPassExecutionContext& renderPassGraphExecutionContext, SharedFrameContext& sharedFrameContext) {
                m_ToneMappingRenderPass.Execute(renderPassGraphExecutionContext, sharedFrameContext);
            });

        tonemappingPass.LinkReadInputAttachment(&environmentMapPass, "Forward_Lighting_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        tonemappingPass.CreateColorAttachment(toneMappingOutputTextureDescription, "Tonemapping_Output_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        tonemappingPass.AddDependency(&environmentMapPass);


        uint32 maxFramesInFlight = outputTextures.size();
        outRenderGraph.BeginBuildingRenderGraph(maxFramesInFlight, "World Rendering");
        outRenderGraph.AddPass(depthPrePass);
        outRenderGraph.AddPass(shadowMapPass);
        outRenderGraph.AddPass(lightingPass);
        outRenderGraph.AddPass(environmentMapPass);
        outRenderGraph.AddOutputPass(tonemappingPass);
        outRenderGraph.SetOutputAttachment(tonemappingPass, "Tonemapping_Output_Buffer", outputTextures);
        outRenderGraph.EndBuildingRenderGraph();
    }

}
