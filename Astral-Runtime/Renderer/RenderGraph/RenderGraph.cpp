/**
* @file RenderGraph.cpp
* @author Andrew Fagan
* @date 7/7/2025
*/

#include "RenderGraph.h"

#include "Debug/Utilities/Asserts.h"
#include "Renderer/RHI/RendererAPI.h"

#include <stack>
#include <unordered_set>

namespace Astral {

    void RenderGraph::BeginBuildingRenderGraph()
    {
        Device& device = RendererAPI::GetDevice();
        m_RenderPass = device.CreateRenderPass();

        m_Subpasses.clear();
        m_SubpassAttachmentOffset.clear();
        m_NumAttachments = 0;
        m_Textures.clear();
    }


    void RenderGraph::AddSubpass(const RenderGraphSubpass& subpass)
    {
        m_Subpasses.push_back(subpass);
    }


    void RenderGraph::SetOutputPass(const RenderGraphSubpass& subpass)
    {
        m_Subpasses.push_back(subpass);
        m_OutputSubpassIndex = m_Subpasses.size() - 1;
    }


    void RenderGraph::EndBuildingRenderGraph()
    {
        m_RenderPass->BeginBuildingRenderPass();

        for (const RenderGraphSubpass& subpass : m_Subpasses)
        {
            m_SubpassAttachmentOffset.push_back(m_NumAttachments);

            for (const RenderGraphSubpass::LocalAttachment& localAttachment : subpass.GetAttachments())
            {
                m_RenderPass->DefineAttachment(localAttachment.AttachmentDescription);
                m_NumAttachments++;
            }

            SubpassIndex subpassIndex = m_SubpassNodes.size();
            AEDirectedGraph<SubpassIndex>::Vertex subpassPassNode = m_RenderGraph.AddVertex(subpassIndex);
            m_SubpassNodes.push_back(subpassPassNode);
        }

        for (size_t i = 0; i < m_Subpasses.size(); i++)
        {
            m_RenderPass->BeginBuildingSubpass();

            const RenderGraphSubpass& subpass = m_Subpasses[i];
            const std::vector<RenderGraphSubpass::LocalAttachment>& m_SubpassAttachments = subpass.GetAttachments();

            for (AttachmentIndex localAttachmentIndex : subpass.GetColorAttachments())
            {
                const RenderGraphSubpass::LocalAttachment& localAttachment = m_SubpassAttachments[localAttachmentIndex];
                AttachmentIndex globalAttachmentIndex = m_SubpassAttachmentOffset[i] + localAttachmentIndex;
                m_RenderPass->AddColorAttachment(globalAttachmentIndex, localAttachment.OptimalImageLayout);
            }


            for (AttachmentIndex localAttachmentIndex : subpass.GetResolveAttachments())
            {
                const RenderGraphSubpass::LocalAttachment& localAttachment = m_SubpassAttachments[localAttachmentIndex];
                AttachmentIndex globalAttachmentIndex = m_SubpassAttachmentOffset[i] + localAttachmentIndex;
                m_RenderPass->AddResolveAttachment(globalAttachmentIndex, localAttachment.OptimalImageLayout);
            }


            for (RenderGraphSubpass::ExternalAttachment externalAttachment : subpass.GetInputAttachments())
            {
                std::string_view& inputAttachmentName = externalAttachment.Name;
                const RenderGraphSubpass& owningSubpass = externalAttachment.OwningSubpass;
                AttachmentIndex localAttachmentIndex = owningSubpass.GetAttachment(inputAttachmentName);
                ASSERT(localAttachmentIndex != NullAttachmentIndex, "External subpass does not contain attachment by name " << inputAttachmentName << "!")

                SubpassIndex subpassIndex = GetSubpassIndex(owningSubpass);
                ASSERT(subpassIndex != NullSubpassIndex, "Subpass does not exist in render graph!")

                AEDirectedGraph<SubpassIndex>::Vertex originSubpassNode = m_SubpassNodes[subpassIndex];
                AEDirectedGraph<SubpassIndex>::Vertex userSubpassNode = m_SubpassNodes[i];
                userSubpassNode.AddEdge(originSubpassNode, subpassIndex); // User node depends on origin node

                AttachmentIndex globalAttachmentIndex = m_SubpassAttachmentOffset[subpassIndex] + localAttachmentIndex;
                m_RenderPass->AddInputAttachment(globalAttachmentIndex, externalAttachment.OptimalImageLayout);
            }


            AttachmentIndex localAttachmentIndex = subpass.GetDepthAttachment();
            if (localAttachmentIndex != NullAttachmentIndex)
            {
                const RenderGraphSubpass::LocalAttachment& localAttachment = m_SubpassAttachments[localAttachmentIndex];
                AttachmentIndex globalAttachmentIndex = m_SubpassAttachmentOffset[i] + localAttachmentIndex;
                m_RenderPass->AddDepthStencilAttachment(globalAttachmentIndex, localAttachment.OptimalImageLayout);
            }


            m_RenderPass->EndBuildingSubpass();
        }

        m_RenderPass->EndBuildingRenderPass();
    }


    void RenderGraph::Execute()
    {
        std::vector<SubpassIndex> executionOrder;

        std::unordered_set<AEDirectedGraph<SubpassIndex>::Vertex> visitedSubpassNodes;
        std::unordered_set<AEDirectedGraph<SubpassIndex>::Vertex> childPushedNodes;
        std::unordered_set<AEDirectedGraph<SubpassIndex>::Vertex> processedSubpassNodes;

        std::stack<AEDirectedGraph<SubpassIndex>::Vertex> nodesToVisit;

        AEDirectedGraph<SubpassIndex>::Vertex outputNode = m_SubpassNodes[m_OutputSubpassIndex];
        nodesToVisit.push(outputNode);

        while (!nodesToVisit.empty())
        {
            AEDirectedGraph<SubpassIndex>::Vertex currentNode = nodesToVisit.top();
            nodesToVisit.pop();

            if (processedSubpassNodes.contains(currentNode)) { continue; }

            if (childPushedNodes.contains(currentNode))
            {
                executionOrder.push_back(currentNode.GetData());
                processedSubpassNodes.insert(currentNode);
            }
            else
            {
                childPushedNodes.insert(currentNode);
                nodesToVisit.push(currentNode);
            }

            for (AEDirectedGraph<SubpassIndex>::Edge edge : currentNode)
            {
                AEDirectedGraph<SubpassIndex>::Vertex dependentNode = edge.GetRightVertex();
                nodesToVisit.push(dependentNode);
                ASSERT(!(visitedSubpassNodes.contains(dependentNode) && !processedSubpassNodes.contains(dependentNode)), "Cycle detected in render graph!")
                visitedSubpassNodes.insert(dependentNode);
            }

        }

        // Execute subpass' code
        for (SubpassIndex subpassIndex : executionOrder)
        {
            const RenderGraphSubpass& subpass = m_Subpasses[subpassIndex];
            LOG(subpass.GetDebugName());

            subpass.Execute();
        }
    }


    SubpassIndex RenderGraph::GetSubpassIndex(const RenderGraphSubpass& subpass)
    {
        for (int i = 0; i < m_Subpasses.size(); i++)
        {
            if (m_Subpasses[i] == subpass) { return i; }
        }

        return NullSubpassIndex;
    }

}
