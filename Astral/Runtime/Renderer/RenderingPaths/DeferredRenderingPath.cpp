/**
* @file DeferredRenderingPath.cpp
* @author Andrew Fagan
* @date 6/4/26
*/

#include "DeferredRenderingPath.h"

#include "Renderer/SceneRenderer.h"

namespace Astral {

    void DeferredRenderingPath::InitRenderPasses(uint32 numInFlightImages)
    {
        m_DeferredGeometryRenderPass.Init(numInFlightImages);
        m_CascadedShadowMapRenderPass.Init(numInFlightImages);
        m_DeferredLightingRenderPass.Init(numInFlightImages);
        m_EnvironmentMapRenderPass.Init(numInFlightImages);
        m_ToneMappingRenderPass.Init(numInFlightImages);
    }


    void DeferredRenderingPath::ShutdownRenderPasses()
    {
        m_ToneMappingRenderPass.Shutdown();
        m_EnvironmentMapRenderPass.Shutdown();
        m_DeferredLightingRenderPass.Shutdown();
        m_CascadedShadowMapRenderPass.Shutdown();
        m_DeferredGeometryRenderPass.Shutdown();
    }


    void DeferredRenderingPath::BuildRenderGraph(RenderGraph& outRenderGraph, const std::vector<TextureHandle>& outputTextures)
    {
        const RendererSettings& rendererSettings = SceneRenderer::GetRendererSettings();

        AttachmentDescription albedoBufferDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0)
        };

        AttachmentDescription metallicBufferDescription = {
            .Format = ImageFormat::R8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0)
        };

        AttachmentDescription roughnessBufferDescription = {
            .Format = ImageFormat::R8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0)
        };

        AttachmentDescription emissionBufferDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0)
        };

        AttachmentDescription normalBufferDescription = {
            .Format = ImageFormat::R8G8B8A8_UNORM,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::COLOR_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 0.0, 1.0)
        };

        AttachmentDescription depthBufferDescription = {
            .Format = ImageFormat::D32_SFLOAT_S8_UINT,
            .ImageUsageFlags = IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .FinalLayout = ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
            .ClearColor = Vec4(1.0, 0.0, 0.0, 1.0)
        };

        RenderGraphPass geometryPass = RenderGraphPass(
            OutputAttachmentDimensions,
            "GBuffer Pass",
            [&](RenderGraphPassExecutionContext& renderPassGraphExecutionContext, SharedFrameContext& sharedFrameContext) {
                m_DeferredGeometryRenderPass.Execute(renderPassGraphExecutionContext, sharedFrameContext);
            });
        geometryPass.CreateColorAttachment(albedoBufferDescription, "GBuffer_Albedo", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.CreateColorAttachment(metallicBufferDescription, "GBuffer_Metallic", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.CreateColorAttachment(roughnessBufferDescription, "GBuffer_Roughness", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.CreateColorAttachment(emissionBufferDescription, "GBuffer_Emission", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.CreateColorAttachment(normalBufferDescription, "GBuffer_Normals", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        geometryPass.CreateDepthStencilAttachment(depthBufferDescription, "GBuffer_Depth_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);


        AttachmentDescription lightDepthBufferDescription = {
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
        shadowMapPass.CreateDepthStencilAttachment(lightDepthBufferDescription, "Light_Depth_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);


        AttachmentDescription lightingTextureDescription = {
            .Format = ImageFormat::R16G16B16A16_SFLOAT,
            .ImageUsageFlags = IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .LoadOp = AttachmentLoadOp::CLEAR,
            .StoreOp = AttachmentStoreOp::STORE,
            .InitialLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .FinalLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL,
            .ClearColor = Vec4(0.0, 0.0, 1.0, 1.0)
        };


        RenderGraphPass lightingPass = RenderGraphPass(
            OutputAttachmentDimensions,
            "Lighting Pass",
            [&](RenderGraphPassExecutionContext& renderPassGraphExecutionContext, SharedFrameContext& sharedFrameContext) {
            m_DeferredLightingRenderPass.Execute(renderPassGraphExecutionContext, sharedFrameContext);
            });
        lightingPass.LinkReadInputAttachment(&geometryPass, "GBuffer_Albedo", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.LinkReadInputAttachment(&geometryPass, "GBuffer_Metallic", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.LinkReadInputAttachment(&geometryPass, "GBuffer_Roughness", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.LinkReadInputAttachment(&geometryPass, "GBuffer_Emission", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.LinkReadInputAttachment(&geometryPass, "GBuffer_Normals", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.LinkReadInputAttachment(&geometryPass, "GBuffer_Depth_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        lightingPass.LinkReadInputAttachment(&shadowMapPass, "Light_Depth_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);

        lightingPass.CreateColorAttachment(lightingTextureDescription, "Deferred_Lighting_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        lightingPass.AddDependency(&shadowMapPass);


        RenderGraphPass environmentMapPass = RenderGraphPass(
            OutputAttachmentDimensions,
            "Environment Map Pass",
            [&](RenderGraphPassExecutionContext& renderPassGraphExecutionContext, SharedFrameContext& sharedFrameContext) {
                m_EnvironmentMapRenderPass.Execute(renderPassGraphExecutionContext, sharedFrameContext);
            });
        environmentMapPass.LinkWriteInputAttachment(&lightingPass, "Deferred_Lighting_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        environmentMapPass.LinkWriteInputAttachment(&geometryPass, "GBuffer_Depth_Buffer", ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        m_EnvironmentMapRenderPass.SetMSAASampleCount(SampleCount::SAMPLE_1_BIT);

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
            OutputAttachmentDimensions,
            "Tonemapping Pass",
            [&](RenderGraphPassExecutionContext& renderPassGraphExecutionContext, SharedFrameContext& sharedFrameContext) {
                m_ToneMappingRenderPass.Execute(renderPassGraphExecutionContext, sharedFrameContext);
            });
        tonemappingPass.LinkReadInputAttachment(&lightingPass, "Deferred_Lighting_Buffer", ImageLayout::SHADER_READ_ONLY_OPTIMAL);
        tonemappingPass.CreateColorAttachment(toneMappingOutputTextureDescription, "Tonemapping_Output_Buffer", ImageLayout::COLOR_ATTACHMENT_OPTIMAL);
        tonemappingPass.AddDependency(&environmentMapPass);


        uint32 maxFramesInFlight = outputTextures.size();
        outRenderGraph.BeginBuildingRenderGraph(maxFramesInFlight, "World Rendering");
        outRenderGraph.AddPass(geometryPass);
        outRenderGraph.AddPass(shadowMapPass);
        outRenderGraph.AddPass(lightingPass);
        outRenderGraph.AddPass(environmentMapPass);
        outRenderGraph.AddOutputPass(tonemappingPass);

        switch (rendererSettings.DebugView)
        {
            case RendererDebugView::NONE: outRenderGraph.SetOutputAttachment(tonemappingPass, "Tonemapping_Output_Buffer", outputTextures); break;
            case RendererDebugView::GBUFFER_ALBEDO: outRenderGraph.SetOutputAttachment(geometryPass, "GBuffer_Albedo", outputTextures); break;
            case RendererDebugView::GBUFFER_ROUGHNESS: outRenderGraph.SetOutputAttachment(geometryPass, "GBuffer_Roughness", outputTextures); break;
            case RendererDebugView::GBUFFER_METALLIC: outRenderGraph.SetOutputAttachment(geometryPass, "GBuffer_Metallic", outputTextures); break;
            case RendererDebugView::GBUFFER_EMISSION: outRenderGraph.SetOutputAttachment(geometryPass, "GBuffer_Emission", outputTextures); break;
            case RendererDebugView::GBUFFER_NORMAL: outRenderGraph.SetOutputAttachment(geometryPass, "GBuffer_Normals", outputTextures); break;
            case RendererDebugView::DEPTH: outRenderGraph.SetOutputAttachment(geometryPass, "GBuffer_Depth_Buffer", outputTextures); break;
            default: outRenderGraph.SetOutputAttachment(tonemappingPass, "Tonemapping_Output_Buffer", outputTextures); break;
        }

        outRenderGraph.EndBuildingRenderGraph();
    }

}
