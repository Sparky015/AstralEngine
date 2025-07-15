/**
* @file RenderGraph.h
* @author Andrew Fagan
* @date 7/7/2025
*/

#pragma once

#include "RenderGraphPass.h"
#include "Renderer/RHI/Resources/Texture.h"

#include <vector>

#include "Core/Containers/DirectedGraph.h"
#include "Renderer/RHI/Resources/DescriptorSet.h"


namespace Astral {

    struct RenderGraphPassExecutionContext
    {
        CommandBufferHandle CommandBuffer;
        RenderPassHandle RenderPass;
        DescriptorSetHandle ReadAttachments;
    };

    using PassIndex = uint8;
    static constexpr PassIndex NullRenderPassIndex = -1;

    class RenderGraph
    {
    public:



        void BeginBuildingRenderGraph(const std::string_view& debugName = "Untitled Render Graph");
        void AddPass(const RenderGraphPass& pass);
        void SetOutputAttachment(const RenderGraphPass& pass, std::string_view attachmentName, const std::vector<TextureHandle>& offscreenTargets);
        void SetOutputAttachment(const RenderGraphPass& pass, std::string_view attachmentName, const std::vector<RenderTargetHandle>& swapchainTargets);
        void EndBuildingRenderGraph();

        void Execute(CommandBufferHandle commandBuffer, uint32 swapchainImageIndex);
        const RenderGraphPassExecutionContext& GetExecutionContext() const { return m_ExecutionContext; }

        // TODO: Create method to resize all resources in render graph to new size (also pass in newly sized offscreen textures/render targets)
        void ResizeResources(const std::vector<TextureHandle>& offscreenTargets);
        void ResizeResources(const std::vector<RenderTargetHandle>& swapchainTargets);

    private:

        struct RenderPassResources
        {
            DescriptorSetHandle ReadAttachmentDescriptorSet;
            FramebufferHandle Framebuffer;
            std::vector<TextureHandle> AttachmentTextures;
        };

        // TODO: Manage the creation of all the textures after all subpasses are defined

        void BuildRenderGraph();
        void SolveRenderPassExecutionOrder();
        void BuildRenderPassObjects();
        void BuildRenderPassResources();

        PassIndex GetRenderPassIndex(const RenderGraphPass& pass);


        std::vector<RenderGraphPass> m_Passes;
        std::vector<RenderPassHandle> m_RenderPasses;
        uint32 m_FramesInFlight{3};

        using RenderGraphResources = std::vector<std::vector<RenderPassResources>>;
        RenderGraphResources m_RenderPassResources;
        std::vector<RenderGraphResources> m_RenderPassResourcesHold;
        uint32 m_FramesTillHoldClear = 0;

        AEDirectedGraph<PassIndex> m_RenderGraph;
        std::vector<AEDirectedGraph<PassIndex>::Vertex> m_RenderPassNodes;


        std::vector<PassIndex> m_ExecutionOrder;
        RenderGraphPassExecutionContext m_ExecutionContext{};


        PassIndex m_OutputRenderPassIndex {0};
        std::string_view m_OutputAttachmentName;
        std::vector<TextureHandle> m_OffscreenOutputTargets;
        UVec2 m_ViewportDimensions{};

        std::string_view m_DebugName;
    };

}
