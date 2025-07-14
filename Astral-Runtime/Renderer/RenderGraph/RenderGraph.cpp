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


    void RenderGraph::SetOutputAttachment(const RenderGraphPass& pass, std::string_view attachmentName, const std::vector<TextureHandle>& offscreenTargets)
    {
        m_OutputAttachmentName = attachmentName;
        m_OutputRenderPassIndex = GetRenderPassIndex(pass);
        ASSERT(m_OutputRenderPassIndex != NullRenderPassIndex, "Attempting to set output attachment from render pass not in render graph!")

        m_OffscreenOutputTargets = offscreenTargets;
        ASSERT(m_OffscreenOutputTargets.size() == m_FramesInFlight, "Render Graph: Number of output textures does not match the number of frames in flight!")
    }


    void RenderGraph::SetOutputAttachment(const RenderGraphPass& pass, std::string_view attachmentName, const std::vector<RenderTargetHandle>& swapchainTargets)
    {
        m_OutputAttachmentName = attachmentName;
        m_OutputRenderPassIndex = GetRenderPassIndex(pass);
        ASSERT(m_OutputRenderPassIndex != NullRenderPassIndex, "Attempting to set output attachment from render pass not in render graph!")

        // m_OffscreenOutputTargets = swapchainTargets; // TODO: Make a way to use swapchain images directly
        ASSERT(m_OffscreenOutputTargets.size() == m_FramesInFlight, "Render Graph: Number of output textures does not match the number of frames in flight!")
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
            m_ExecutionContext.ReadAttachments = renderPassResource.ReadAttachmentDescriptorSet;


            RendererAPI::BeginLabel(commandBuffer, pass.GetDebugName(), Vec4(1.0 , 0.0, 1.0, 1.0));
            rhiRenderPass->BeginRenderPass(commandBuffer, renderPassResource.Framebuffer);

            pass.Execute();

            rhiRenderPass->EndRenderPass(commandBuffer);
            RendererAPI::EndLabel(commandBuffer);
        }

        // Transition all attachment layouts to their initial starting layout
        PipelineBarrier pipelineBarrier = {};
        pipelineBarrier.SourceStageMask = PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        pipelineBarrier.DestinationStageMask = PIPELINE_STAGE_VERTEX_SHADER_BIT | PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        pipelineBarrier.DependencyFlags = DependencyFlags::BY_REGION_BIT;
        for (size_t i = 0; i < m_ExecutionOrder.size(); i++)
        {
            PassIndex renderPassIndex = m_ExecutionOrder[i];
            const RenderGraphPass& pass = m_Passes[renderPassIndex];
            RenderPassResources& renderPassResource = m_RenderPassResources[renderPassIndex][swapchainImageIndex];

            for (size_t j = 0; j < renderPassResource.AttachmentTextures.size(); j++)
            {
                TextureHandle attachmentTexture = renderPassResource.AttachmentTextures[j];

                ImageMemoryBarrier imageMemoryBarrier = {};
                imageMemoryBarrier.SourceAccessMask = 0;
                imageMemoryBarrier.DestinationAccessMask = ACCESS_FLAGS_SHADER_READ_BIT | ACCESS_FLAGS_COLOR_ATTACHMENT_WRITE_BIT | ACCESS_FLAGS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                imageMemoryBarrier.OldLayout = ImageLayout::UNDEFINED;
                imageMemoryBarrier.NewLayout = pass.GetAttachments()[j].InitialLayout;
                imageMemoryBarrier.SourceQueueFamilyIndex = QueueFamilyIgnored;
                imageMemoryBarrier.DestinationQueueFamilyIndex = QueueFamilyIgnored;
                imageMemoryBarrier.Image = attachmentTexture;
                imageMemoryBarrier.ImageSubresourceRange = {
                    .AspectMask = attachmentTexture->GetImageAspect(),
                    .BaseMipLevel = 0,
                    .LevelCount = 1,
                    .BaseArrayLayer = 0,
                    .LayerCount = 1
                };

                pipelineBarrier.ImageMemoryBarriers.push_back(imageMemoryBarrier);
            }
        }
        RendererAPI::SetPipelineBarrier(commandBuffer, pipelineBarrier);


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
            const RenderGraphPass& consumingPass = m_Passes[i];

            for (RenderGraphPass::ExternalAttachment externalAttachment : consumingPass.GetInputAttachments())
            {
                std::string_view& inputAttachmentName = externalAttachment.Name;
                RenderGraphPass& producerPassScopeLocal = externalAttachment.OwningPass;
                AttachmentIndex localAttachmentIndex = producerPassScopeLocal.GetAttachment(inputAttachmentName);
                ASSERT(localAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << inputAttachmentName << "!")

                PassIndex externalRenderPassIndex = GetRenderPassIndex(producerPassScopeLocal);
                ASSERT(externalRenderPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

                AEDirectedGraph<PassIndex>::Vertex& originPassNode = m_RenderPassNodes[externalRenderPassIndex];
                AEDirectedGraph<PassIndex>::Vertex& userPassNode = m_RenderPassNodes[i];
                userPassNode.AddEdge(originPassNode, externalRenderPassIndex); // User node depends on origin node



                // Match the final layout of the producing render pass to the optimal layout of the consuming render pass

                // TODO: Create debug view of a render pass attachment that shows when what render passes write to it and what passes read from it

                AttachmentDescription& producerAttachmentDesc = producerPassScopeLocal.GetAttachments()[localAttachmentIndex].AttachmentDescription;
                producerAttachmentDesc.StoreOp = AttachmentStoreOp::STORE;
                producerAttachmentDesc.FinalLayout = externalAttachment.OptimalImageLayout;
                // TODO: Fix the two copies of the RenderGraphPass between the render graph owned copy and the input passes saved by other render passes using the local scope RenderGraphPass copy
                AttachmentDescription& producerAttachmentDescOwned = m_Passes[externalRenderPassIndex].GetAttachments()[localAttachmentIndex].AttachmentDescription;
                producerAttachmentDescOwned.StoreOp = AttachmentStoreOp::STORE;
                producerAttachmentDescOwned.FinalLayout = externalAttachment.OptimalImageLayout;
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

        // ----------------------------------------------------------------------------------------------------------------------------------
        // Pass One: Create the resources for all the render passes ahead of building the render pass objects

        Device& device = RendererAPI::GetDevice();

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
                renderPass->DefineAttachment(localAttachment.AttachmentDescription);


                if (renderPassIndex != m_OutputRenderPassIndex && localAttachment.Name != m_OutputAttachmentName)
                {
                    TextureCreateInfo textureCreateInfo = {
                        .Format = localAttachment.AttachmentDescription.Format,
                        .Layout = localAttachment.AttachmentDescription.InitialLayout,
                        .UsageFlags = localAttachment.AttachmentDescription.ImageUsageFlags,
                        .Dimensions = pass.GetResourceDimensions(),
                        .ImageData = nullptr
                    };


                    // Create the textures for the attachment

                    ASSERT(passResources.size() == m_FramesInFlight, "The number of copies of resources is expected to be the number of frames in flight!")
                    for (size_t i = 0; i < m_FramesInFlight; i++)
                    {
                        TextureHandle attachmentTexture = device.CreateTexture(textureCreateInfo);
                        passResources[i].AttachmentTextures.push_back(attachmentTexture);
                        RendererAPI::NameObject(attachmentTexture, std::string(localAttachment.Name) + "_" + std::to_string(i));
                    }
                }
                else
                {
                    for (size_t i = 0; i < m_FramesInFlight; i++)
                    {
                        TextureHandle attachmentTexture = m_OffscreenOutputTargets[i];
                        passResources[i].AttachmentTextures.push_back(attachmentTexture);
                        RendererAPI::NameObject(attachmentTexture, std::string(localAttachment.Name) + "_" + std::to_string(i));
                    }
                }

            }


            //      Note: The render pass object doesn't need to know about inputs coming from another render pass

            // Define the external attachments for the render pass
            // for (const RenderGraphPass::ExternalAttachment& externalAttachment : pass.GetInputAttachments())
            // {
            //     PassIndex externalPassIndex = GetRenderPassIndex(externalAttachment.OwningPass);
            //     ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")
            //
            //     AttachmentIndex externalPassAttachmentIndex = externalAttachment.OwningPass.GetAttachment(externalAttachment.Name);
            //     ASSERT(externalPassAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << externalAttachment.Name << "!")
            //
            //     RenderGraphPass::LocalAttachment inputAttachment = externalAttachment.OwningPass.GetAttachments()[externalPassAttachmentIndex];
            //     // TODO: Change the store op of the owning render pass to always be AttachmentStoreOp::Store
            //     // TODO: Change the final layout of the producing render pass to always be the optimal layout of the consuming render pass
            //     inputAttachment.AttachmentDescription.LoadOp = AttachmentLoadOp::LOAD;
            //     inputAttachment.AttachmentDescription.InitialLayout = externalAttachment.OptimalImageLayout;
            //     renderPass->DefineAttachment(inputAttachment.AttachmentDescription);
            // }
        }





        // ----------------------------------------------------------------------------------------------------------------------------------
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


            //      Note: The render pass object doesn't need to know about inputs coming from another render pass

            // uint32 numOfLocalAttachments = passLocalAttachments.size();
            // uint32 externalPassAttachmentIndex = numOfLocalAttachments;
            // for (RenderGraphPass::ExternalAttachment externalAttachment : pass.GetInputAttachments())
            // {
            //     renderPass->AddInputAttachment(externalPassAttachmentIndex, externalAttachment.OptimalImageLayout);
            //     externalPassAttachmentIndex++;
            // }


            renderPass->EndBuildingSubpass();
            renderPass->EndBuildingRenderPass();
        }



        // ----------------------------------------------------------------------------------------------------------------------------------
        // Build the framebuffers and descriptor sets for each render pass


        for (PassIndex renderPassIndex : m_ExecutionOrder)
        {
            RenderPassHandle renderPass = m_RenderPasses[renderPassIndex];
            const RenderGraphPass& pass = m_Passes[renderPassIndex];
            std::vector<RenderPassResources>& passResources = m_RenderPassResources[renderPassIndex];
            ASSERT(passResources.size() == m_FramesInFlight, "The number of copies of resources is expected to be the number of frames in flight!")

            for (size_t i = 0; i < m_FramesInFlight; i++)
            {
                // Create the framebuffers for the render pass
                passResources[i].Framebuffer = device.CreateFramebuffer(renderPass);

                UVec2 passResourceDimensions = pass.GetResourceDimensions();
                passResources[i].Framebuffer->BeginBuildingFramebuffer(passResourceDimensions.x, passResourceDimensions.y);

                for (const TextureHandle& attachmentTexture : m_RenderPassResources[renderPassIndex][i].AttachmentTextures)
                {
                    passResources[i].Framebuffer->AttachTexture(attachmentTexture);
                }

                passResources[i].Framebuffer->EndBuildingFramebuffer();
                RendererAPI::NameObject(passResources[i].Framebuffer, std::string(pass.GetDebugName()) + "_Framebuffer_" + std::to_string(i));


                // Pull external attachment textures from the producer render pass' resources and add them to a descriptor set to read from
                passResources[i].ReadAttachmentDescriptorSet = device.CreateDescriptorSet();
                passResources[i].ReadAttachmentDescriptorSet->BeginBuildingSet();
                for (const RenderGraphPass::ExternalAttachment& externalAttachment : pass.GetInputAttachments())
                {
                    PassIndex externalPassIndex = GetRenderPassIndex(externalAttachment.OwningPass);
                    ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

                    const RenderPassResources& externalPassResources = m_RenderPassResources[externalPassIndex][i];
                    AttachmentIndex externalPassAttachmentIndex = externalAttachment.OwningPass.GetAttachment(externalAttachment.Name);
                    ASSERT(externalPassAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << externalAttachment.Name << "!")

                    TextureHandle externalAttachmentTexture = externalPassResources.AttachmentTextures[externalPassAttachmentIndex];
                    ASSERT(passResources[i].Framebuffer->GetExtent() == externalAttachmentTexture->GetDimensions(), "External input texture does not match the dimensions of the target render pass!");

                    passResources[i].ReadAttachmentDescriptorSet->AddDescriptorImageSampler(externalAttachmentTexture, ShaderStage::FRAGMENT);
                }
                passResources[i].ReadAttachmentDescriptorSet->EndBuildingSet();
                RendererAPI::NameObject(passResources[i].ReadAttachmentDescriptorSet, std::string(pass.GetDebugName()) + "_ReadDescriptorSet_" + std::to_string(i));
            }
        }


        // ----------------------------------------------------------------------------------------------------------------------------------
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
