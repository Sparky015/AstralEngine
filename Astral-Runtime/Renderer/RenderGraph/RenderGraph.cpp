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

    void RenderGraph::BeginBuildingRenderGraph(const std::string_view& debugName)
    {
        m_DebugName = debugName;

        m_Passes.clear();

        m_RenderPassResources.clear();
    }


    void RenderGraph::AddPass(const RenderGraphPass& pass)
    {
        m_Passes.push_back(pass);
        m_RenderPasses.push_back(nullptr);
        m_RenderPassResources.push_back(std::vector<RenderPassResources>());
    }


    void RenderGraph::SetOutputPass(const RenderGraphPass& pass)
    {
        AddPass(pass);
        m_OutputRenderPassIndex = m_Passes.size() - 1;
    }


    void RenderGraph::EndBuildingRenderGraph()
    {

        // Populates the actual graph struction
        BuildRenderGraph();

        // Detects dependencies between the render passes and sorts the passes into the correct order
        SolveRenderPassExecutionOrder();


        BuildRenderPassObjects();

    }


    void RenderGraph::Execute(CommandBufferHandle commandBuffer, uint32 swapchainImageIndex)
    {
        m_ExecutionContext.CommandBuffer = commandBuffer;

        RendererAPI::BeginLabel(commandBuffer, m_DebugName, Vec4(1.0 , 1.0, 0, 1.0));

        for (size_t i = 0; i < m_ExecutionOrder.size(); i++)
        {
            PassIndex renderPassIndex = m_ExecutionOrder[i];
            const RenderGraphPass& pass = m_Passes[renderPassIndex];
            RenderPassHandle rhiRenderPass = m_RenderPasses[renderPassIndex];
            RenderPassResources& renderPassResource = m_RenderPassResources[renderPassIndex][swapchainImageIndex];


            m_ExecutionContext.RenderPass = rhiRenderPass;


            RendererAPI::BeginLabel(commandBuffer, pass.GetDebugName(), Vec4(1.0 , 0.0, 1.0, 1.0));
            rhiRenderPass->BeginRenderPass(commandBuffer, renderPassResource.Framebuffer);

            pass.Execute();

            RendererAPI::EndLabel(commandBuffer);

            rhiRenderPass->EndRenderPass(commandBuffer);
        }

        RendererAPI::EndLabel(commandBuffer);

        m_ExecutionContext = {};
    }


    void RenderGraph::BuildRenderGraph()
    {
        // Add all the render passes as a node in the render graph
        for (size_t renderPassIndex = 0; renderPassIndex < m_Passes.size(); renderPassIndex++)
        {
            AEDirectedGraph<PassIndex>::Vertex renderPassNode = m_RenderGraph.AddVertex(renderPassIndex);
            m_RenderPassNodes.push_back(renderPassNode);
        }

        // Add edges to all render passes with dependencies
        for (size_t i = 0; i < m_Passes.size(); i++)
        {
            const RenderGraphPass& pass = m_Passes[i];

            for (RenderGraphPass::ExternalAttachment externalAttachment : pass.GetInputAttachments())
            {
                std::string_view& inputAttachmentName = externalAttachment.Name;
                const RenderGraphPass& owningPass = externalAttachment.OwningPass;
                AttachmentIndex localAttachmentIndex = owningPass.GetAttachment(inputAttachmentName);
                ASSERT(localAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << inputAttachmentName << "!")

                PassIndex externalRenderPassIndex = GetRenderPassIndex(owningPass);
                ASSERT(externalRenderPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

                AEDirectedGraph<PassIndex>::Vertex& originPassNode = m_RenderPassNodes[externalRenderPassIndex];
                AEDirectedGraph<PassIndex>::Vertex& userPassNode = m_RenderPassNodes[i];
                userPassNode.AddEdge(originPassNode, externalRenderPassIndex); // User node depends on origin node
            }
        }
    }


    void RenderGraph::SolveRenderPassExecutionOrder()
    {
        std::unordered_set<AEDirectedGraph<PassIndex>::Vertex> visitedPassNodes;
        std::unordered_set<AEDirectedGraph<PassIndex>::Vertex> childPushedNodes;
        std::unordered_set<AEDirectedGraph<PassIndex>::Vertex> processedPassNodes;

        std::stack<AEDirectedGraph<PassIndex>::Vertex> nodesToVisit;

        AEDirectedGraph<PassIndex>::Vertex outputNode = m_RenderPassNodes[m_OutputRenderPassIndex];
        nodesToVisit.push(outputNode);

        while (!nodesToVisit.empty())
        {
            AEDirectedGraph<PassIndex>::Vertex currentNode = nodesToVisit.top();
            nodesToVisit.pop();

            if (processedPassNodes.contains(currentNode)) { continue; }

            if (childPushedNodes.contains(currentNode))
            {
                m_ExecutionOrder.push_back(currentNode.GetData());
                processedPassNodes.insert(currentNode);
            }
            else
            {
                childPushedNodes.insert(currentNode);
                nodesToVisit.push(currentNode);
            }

            for (AEDirectedGraph<PassIndex>::Edge edge : currentNode)
            {
                AEDirectedGraph<PassIndex>::Vertex dependentNode = edge.GetRightVertex();
                nodesToVisit.push(dependentNode);
                ASSERT(!(visitedPassNodes.contains(dependentNode) && !processedPassNodes.contains(dependentNode)), "Cycle detected in render graph!")
                visitedPassNodes.insert(dependentNode);
            }

        }

    }


    void RenderGraph::BuildRenderPassObjects()
    {
        Device& device = RendererAPI::GetDevice();

        // Pass One: Create the resources for all the render passes ahead of building the render pass objects
        for (PassIndex renderPassIndex : m_ExecutionOrder)
        {
            const RenderGraphPass& pass = m_Passes[renderPassIndex];
            std::vector<RenderPassResources>& passResources = m_RenderPassResources[renderPassIndex];
            passResources.resize(m_FramesInFlight);

            RenderPassHandle& renderPass = m_RenderPasses[renderPassIndex];
            renderPass = device.CreateRenderPass();
            renderPass->BeginBuildingRenderPass();


            for (const RenderGraphPass::LocalAttachment& localAttachment : pass.GetAttachments())
            {
                // Define attachments for use in the render pass object
                // TODO: Check if the attachment is used as an input attachment in another pass.
                //       Use the render graph nodes array to look up if the pass has a consuming render pass and which attachments are being consumed
                renderPass->DefineAttachment(localAttachment.AttachmentDescription);

                TextureCreateInfo textureCreateInfo = {
                    .Format = localAttachment.AttachmentDescription.Format,
                    .Layout = localAttachment.AttachmentDescription.InitialLayout,
                    .UsageFlags = localAttachment.AttachmentDescription.ImageUsageFlags,
                    .Dimensions = pass.GetResourceDimensions(),
                    .ImageData = nullptr
                };


                // Create the texture for the attachment
                TextureHandle attachmentTexture = device.CreateTexture(textureCreateInfo);

                ASSERT(passResources.size() == m_FramesInFlight, "The number of copies of resources is expected to be the number of frames in flight!")
                for (size_t i = 0; i < m_FramesInFlight; i++)
                {
                    passResources[i].AttachmentTextures.push_back(attachmentTexture);
                }
            }

            // Define the external attachments for the render pass
            for (const RenderGraphPass::ExternalAttachment& externalAttachment : pass.GetInputAttachments())
            {
                PassIndex externalPassIndex = GetRenderPassIndex(externalAttachment.OwningPass);
                ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

                AttachmentIndex externalPassAttachmentIndex = externalAttachment.OwningPass.GetAttachment(externalAttachment.Name);
                ASSERT(externalPassAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << externalAttachment.Name << "!")

                RenderGraphPass::LocalAttachment inputAttachment = externalAttachment.OwningPass.GetAttachments()[externalPassAttachmentIndex];
                // TODO: Change the store op of the owning render pass to always be AttachmentStoreOp::Store
                // TODO: Change the final layout of the producing render pass to always be the optimal layout of the consuming render pass
                inputAttachment.AttachmentDescription.LoadOp = AttachmentLoadOp::LOAD;
                inputAttachment.AttachmentDescription.InitialLayout = externalAttachment.OptimalImageLayout;
                renderPass->DefineAttachment(inputAttachment.AttachmentDescription);
            }
        }







        // Pass Two: Define the pass contents for all the render pass objects using the information compiled in Pass One

        for (PassIndex renderPassIndex : m_ExecutionOrder)
        {
            const RenderGraphPass& pass = m_Passes[renderPassIndex];
            RenderPassHandle& renderPass = m_RenderPasses[renderPassIndex];


            LOG("Creating Render Pass: " << pass.GetDebugName());

            renderPass->BeginBuildingSubpass();


            const std::vector<RenderGraphPass::LocalAttachment>& passLocalAttachments = pass.GetAttachments();

            for (AttachmentIndex localAttachmentIndex : pass.GetColorAttachments())
            {
                const RenderGraphPass::LocalAttachment& localAttachment = passLocalAttachments[localAttachmentIndex];
                renderPass->AddColorAttachment(localAttachmentIndex, localAttachment.OptimalImageLayout);
            }


            {
                AttachmentIndex localAttachmentIndex = pass.GetDepthAttachment();
                if (localAttachmentIndex != NullAttachmentIndex)
                {
                    const RenderGraphPass::LocalAttachment& localAttachment = passLocalAttachments[localAttachmentIndex];
                    renderPass->AddDepthStencilAttachment(localAttachmentIndex, localAttachment.OptimalImageLayout);
                }
            }


            for (AttachmentIndex localAttachmentIndex : pass.GetResolveAttachments())
            {
                const RenderGraphPass::LocalAttachment& localAttachment = passLocalAttachments[localAttachmentIndex];
                renderPass->AddResolveAttachment(localAttachmentIndex, localAttachment.OptimalImageLayout);
            }


            uint32 numOfLocalAttachments = passLocalAttachments.size();
            uint32 externalPassAttachmentIndex = numOfLocalAttachments;
            for (RenderGraphPass::ExternalAttachment externalAttachment : pass.GetInputAttachments())
            {
                renderPass->AddInputAttachment(externalPassAttachmentIndex, externalAttachment.OptimalImageLayout);
                externalPassAttachmentIndex++;
            }


            renderPass->EndBuildingSubpass();
            renderPass->EndBuildingRenderPass();
        }


        // Create the frame buffers and add external textures for the render passes
        for (PassIndex renderPassIndex : m_ExecutionOrder)
        {
            RenderPassHandle renderPass = m_RenderPasses[renderPassIndex];
            const RenderGraphPass& pass = m_Passes[renderPassIndex];
            std::vector<RenderPassResources>& passResources = m_RenderPassResources[renderPassIndex];
            ASSERT(passResources.size() == m_FramesInFlight, "The number of copies of resources is expected to be the number of frames in flight!")

            for (size_t i = 0; i < m_FramesInFlight; i++)
            {
                passResources[i].Framebuffer = device.CreateFramebuffer(renderPass);

                UVec2 passResourceDimensions = pass.GetResourceDimensions();
                passResources[i].Framebuffer->BeginBuildingFramebuffer(passResourceDimensions.x, passResourceDimensions.y);

                for (const TextureHandle& attachmentTexture : m_RenderPassResources[renderPassIndex][i].AttachmentTextures)
                {
                    passResources[i].Framebuffer->AttachTexture(attachmentTexture);
                }


                // Pull external attachment textures from that render pass' resources
                for (const RenderGraphPass::ExternalAttachment& externalAttachment : pass.GetInputAttachments())
                {
                    PassIndex externalPassIndex = GetRenderPassIndex(externalAttachment.OwningPass);
                    ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

                    const RenderPassResources& externalPassResources = m_RenderPassResources[externalPassIndex][i];
                    AttachmentIndex externalPassAttachmentIndex = externalAttachment.OwningPass.GetAttachment(externalAttachment.Name);
                    ASSERT(externalPassAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << externalAttachment.Name << "!")

                    TextureHandle externalAttachmentTexture = externalPassResources.AttachmentTextures[externalPassAttachmentIndex];
                    ASSERT(passResources[i].Framebuffer->GetExtent() == externalAttachmentTexture->GetDimensions(), "External input texture does not match the dimensions of the target render pass!");

                    passResources[i].Framebuffer->AttachTexture(externalAttachmentTexture);
                }

                passResources[i].Framebuffer->EndBuildingFramebuffer();
            }
        }
    }


    PassIndex RenderGraph::GetRenderPassIndex(const RenderGraphPass& pass)
    {
        for (int i = 0; i < m_Passes.size(); i++)
        {
            if (m_Passes[i] == pass) { return i; }
        }

        return NullRenderPassIndex;
    }

}
