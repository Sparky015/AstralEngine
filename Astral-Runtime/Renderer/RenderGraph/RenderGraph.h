/**
* @file RenderGraph.h
* @author Andrew Fagan
* @date 7/7/2025
*/

#pragma once

#include "RenderGraphSubpass.h"
#include "Renderer/RHI/Resources/Texture.h"

#include <vector>

#include "Core/Containers/DirectedGraph.h"


namespace Astral {

    class RenderGraph
    {
    public:

        void BeginBuildingRenderGraph();
        void AddSubpass(const RenderGraphSubpass& subpass);
        void SetOutputPass(const RenderGraphSubpass& subpass);
        void EndBuildingRenderGraph();

        void Execute();


    private:

        // TODO: Manage the creation of all the textures after all subpasses are defined

        SubpassIndex GetSubpassIndex(const RenderGraphSubpass& subpass);

        RenderPassHandle m_RenderPass;

        std::vector<RenderGraphSubpass> m_Subpasses;
        std::vector<AEDirectedGraph<SubpassIndex>::Vertex> m_SubpassNodes;
        std::vector<uint32> m_SubpassAttachmentOffset;
        uint32 m_NumAttachments {0};

        AEDirectedGraph<SubpassIndex> m_RenderGraph;
        uint32 m_OutputSubpassIndex {0};
        std::vector<TextureHandle> m_Textures;
    };

}
