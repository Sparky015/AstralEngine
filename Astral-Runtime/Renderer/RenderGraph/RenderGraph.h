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

    struct RenderGraphExecutionContext
    {
        CommandBufferHandle CommandBuffer;
        RenderPassHandle RenderPass;
    };

    using PassIndex = uint8;
    static constexpr PassIndex NullRenderPassIndex = -1;

    class RenderGraph
    {
    public:



        void BeginBuildingRenderGraph(const std::string_view& debugName = "Untitled Render Graph");
        void AddPass(const RenderGraphPass& pass);
        void SetOutputPass(const RenderGraphPass& pass);
        void EndBuildingRenderGraph();

        void Execute(CommandBufferHandle commandBuffer, uint32 swapchainImageIndex);
        const RenderGraphExecutionContext& GetExecutionContext() const { return m_ExecutionContext; }


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

        PassIndex GetRenderPassIndex(const RenderGraphPass& pass);


        std::vector<RenderGraphPass> m_Passes;
        std::vector<RenderPassHandle> m_RenderPasses;
        std::vector<std::vector<RenderPassResources>> m_RenderPassResources;
        uint32 m_FramesInFlight{3};

        AEDirectedGraph<PassIndex> m_RenderGraph;
        std::vector<AEDirectedGraph<PassIndex>::Vertex> m_RenderPassNodes;
        PassIndex m_OutputRenderPassIndex {0};


        std::vector<PassIndex> m_ExecutionOrder;
        RenderGraphExecutionContext m_ExecutionContext{};


        std::string_view m_DebugName;
    };

}
