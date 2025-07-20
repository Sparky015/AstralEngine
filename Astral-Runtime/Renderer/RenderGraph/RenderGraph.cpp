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
#include <ranges>

namespace Astral {

    void RenderGraph::BeginBuildingRenderGraph(uint32 maxFramesInFlight, const std::string_view& debugName)
    {
        m_DebugName = debugName;
        m_MaxFramesInFlight = maxFramesInFlight;

        AddRenderGraphResourcesToHold();

        m_Passes.clear();

        m_RenderGraph = AEDirectedGraph<PassIndex>();
        m_RenderPassNodes.clear();

        m_ExecutionOrder.clear();

        m_RenderPasses.clear();
        m_RenderPassResources.clear();

        m_OutputRenderPassIndex = 0;
        m_OutputAttachmentName = "";

        m_OffscreenOutputTargets.clear();
        m_ViewportDimensions = UVec2(0);
    }


    void RenderGraph::AddPass(const RenderGraphPass& pass)
    {
        m_Passes.push_back(pass);
    }


    void RenderGraph::SetOutputAttachment(const RenderGraphPass& pass, std::string_view attachmentName, const std::vector<TextureHandle>& offscreenTargets)
    {
        m_OutputAttachmentName = attachmentName;
        m_OutputRenderPassIndex = GetRenderPassIndex(pass);
        ASSERT(m_OutputRenderPassIndex != NullRenderPassIndex, "Attempting to set output attachment from render pass not in render graph!")

        m_IsOutputRenderTarget = false;
        m_OffscreenOutputTargets = offscreenTargets;
        ASSERT(m_OffscreenOutputTargets.size() == m_MaxFramesInFlight, "Render Graph: Number of output textures does not match the number of frames in flight!")

        m_ViewportDimensions = m_OffscreenOutputTargets[0]->GetDimensions();
    }


    void RenderGraph::SetOutputAttachment(const RenderGraphPass& pass, std::string_view attachmentName, const std::vector<RenderTargetHandle>& swapchainTargets)
    {
        m_OutputAttachmentName = attachmentName;
        m_OutputRenderPassIndex = GetRenderPassIndex(pass);
        ASSERT(m_OutputRenderPassIndex != NullRenderPassIndex, "Attempting to set output attachment from render pass not in render graph!")
        ASSERT(swapchainTargets.size() == m_MaxFramesInFlight, "Render Graph: Number of output textures does not match the number of frames in flight!")

        m_IsOutputRenderTarget = true;
        for (RenderTargetHandle renderTarget : swapchainTargets)
        {
            m_OffscreenOutputTargets.push_back(renderTarget->GetAsTexture());
        }

        m_ViewportDimensions = m_OffscreenOutputTargets[0]->GetDimensions();
    }


    void RenderGraph::EndBuildingRenderGraph()
    {
        BuildRenderGraph();
        SolveRenderPassExecutionOrder();
        CompileRenderPassBarriers();
        BuildRenderPassObjects();
        BuildRenderPassResources();
    }


    void RenderGraph::Execute(CommandBufferHandle commandBuffer, uint32 swapchainImageIndex)
    {
        UpdateRenderGraphResourcesHold();
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


            RendererAPI::BeginLabel(commandBuffer, pass.GetName(), Vec4(1.0 , 0.0, 1.0, 1.0));
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


    void RenderGraph::ResizeResources(const std::vector<TextureHandle>& offscreenTargets)
    {
        m_OffscreenOutputTargets = offscreenTargets;
        ASSERT(m_OffscreenOutputTargets.size() == m_MaxFramesInFlight, "Render Graph: Number of output textures does not match the number of frames in flight!")
        m_ViewportDimensions = m_OffscreenOutputTargets[0]->GetDimensions();

        AddRenderGraphResourcesToHold();

        m_RenderPassResources.clear();
        BuildRenderPassResources();
    }


    void RenderGraph::ResizeResources(const std::vector<RenderTargetHandle>& swapchainTargets)
    {
        std::vector<TextureHandle> renderTargetTextures;

        for (RenderTargetHandle renderTarget : swapchainTargets)
        {
            renderTargetTextures.push_back(renderTarget->GetAsTexture());
        }

        ResizeResources(renderTargetTextures);
    }


    void RenderGraph::ClearResourceHold()
    {
        m_RenderPassesHold.clear();
        m_RenderPassResourcesHold.clear();
        m_FramesTillClear.clear();
    }


    void RenderGraph::BuildRenderGraph()
    {
        // Converting each external attachment to all render passes to point to the render graph owned render pass instances
        for (size_t renderPassIndex = 0; renderPassIndex < m_Passes.size(); renderPassIndex++)
        {
            for (RenderGraphPass::ExternalAttachment& externalAttachment : m_Passes[renderPassIndex].GetReadInputAttachments())
            {
                PassIndex externalPassIndex = GetRenderPassIndex(*externalAttachment.OwningPass);
                ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")
                externalAttachment.OwningPass = &m_Passes[externalPassIndex];
            }
        }

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

            for (const RenderGraphPass::ExternalAttachment& externalAttachment : consumingPass.GetReadInputAttachments())
            {
                const std::string_view& inputAttachmentName = externalAttachment.Name;
                RenderGraphPass& producerPassScopeLocal = *externalAttachment.OwningPass;
                AttachmentIndex localAttachmentIndex = producerPassScopeLocal.GetLocalAttachment(inputAttachmentName);
                ASSERT(localAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << inputAttachmentName << "!")

                PassIndex externalRenderPassIndex = GetRenderPassIndex(producerPassScopeLocal);
                ASSERT(externalRenderPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

                AEDirectedGraph<PassIndex>::Vertex& originPassNode = m_RenderPassNodes[externalRenderPassIndex];
                AEDirectedGraph<PassIndex>::Vertex& userPassNode = m_RenderPassNodes[i];
                userPassNode.AddEdge(originPassNode, externalRenderPassIndex); // User node depends on origin node
            }

            for (const RenderGraphPass::ExternalAttachment& externalAttachment : consumingPass.GetWriteInputAttachments())
            {
                const std::string_view& inputAttachmentName = externalAttachment.Name;
                RenderGraphPass& producerPassScopeLocal = *externalAttachment.OwningPass;
                AttachmentIndex localAttachmentIndex = producerPassScopeLocal.GetLocalAttachment(inputAttachmentName);
                ASSERT(localAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << inputAttachmentName << "!")

                PassIndex externalRenderPassIndex = GetRenderPassIndex(producerPassScopeLocal);
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


    void RenderGraph::CompileRenderPassBarriers()
    {
        // Make the producer pass' final layout the consuming pass' optimal layout
        for (PassIndex consumingPassIndex : m_ExecutionOrder)
        {
            const RenderGraphPass& consumingPass = m_Passes[consumingPassIndex];

            for (const RenderGraphPass::ExternalAttachment& externalAttachment : consumingPass.GetReadInputAttachments())
            {
                const std::string_view& inputAttachmentName = externalAttachment.Name;
                RenderGraphPass& producerPassScopeLocal = *externalAttachment.OwningPass;
                AttachmentIndex localAttachmentIndex = producerPassScopeLocal.GetLocalAttachment(inputAttachmentName);
                ASSERT(localAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << inputAttachmentName << "!")

                // Match the final layout of the producing render pass to the optimal layout of the consuming render pass
                AttachmentDescription& producerAttachmentDesc = producerPassScopeLocal.GetAttachments()[localAttachmentIndex].AttachmentDescription;
                producerAttachmentDesc.StoreOp = AttachmentStoreOp::STORE;
                producerAttachmentDesc.FinalLayout = externalAttachment.OptimalImageLayout;
            }
        }
    }


    void RenderGraph::BuildRenderPassObjects()
    {

        // ----------------------------------------------------------------------------------------------------------------------------------
        // Pass One: Define the attachments for all the render passes ahead of building the render pass objects

        Device& device = RendererAPI::GetDevice();
        m_RenderPasses.resize(m_ExecutionOrder.size(), nullptr);

        for (PassIndex renderPassIndex : m_ExecutionOrder)
        {
            RenderGraphPass& pass = m_Passes[renderPassIndex];
            RenderPassHandle& renderPass = m_RenderPasses[renderPassIndex];
            renderPass = device.CreateRenderPass();
            renderPass->BeginBuildingRenderPass();

            for (RenderGraphPass::LocalAttachment& localAttachment : pass.GetAttachments())
            {
                // Define attachments for use in the render pass object

                if (renderPassIndex == m_OutputRenderPassIndex && localAttachment.Name == m_OutputAttachmentName)
                {
                    localAttachment.AttachmentDescription.InitialLayout = m_OffscreenOutputTargets[0]->GetLayout();
                    localAttachment.InitialLayout = m_OffscreenOutputTargets[0]->GetLayout();
                    localAttachment.AttachmentDescription.Format = m_OffscreenOutputTargets[0]->GetFormat();

                    if (m_IsOutputRenderTarget && m_OffscreenOutputTargets[0]->GetLayout() == ImageLayout::UNDEFINED)
                    {
                        localAttachment.InitialLayout = ImageLayout::PRESENT_SRC_KHR;
                    }
                }

                renderPass->DefineAttachment(localAttachment.AttachmentDescription);
            }



            //      Note: The rhi render pass object doesn't need to know about read inputs coming from another render pass

            // Define the external write attachments for the render pass
            for (RenderGraphPass::ExternalAttachment& externalAttachment : pass.GetWriteInputAttachments())
            {
                PassIndex externalPassIndex = GetRenderPassIndex(*externalAttachment.OwningPass);
                ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

                AttachmentIndex externalPassAttachmentIndex = externalAttachment.OwningPass->GetLocalAttachment(externalAttachment.Name);
                ASSERT(externalPassAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << externalAttachment.Name << "!")

                RenderGraphPass::LocalAttachment inputAttachment = externalAttachment.OwningPass->GetAttachments()[externalPassAttachmentIndex];
                inputAttachment.AttachmentDescription.LoadOp = AttachmentLoadOp::LOAD;
                inputAttachment.AttachmentDescription.InitialLayout = externalAttachment.OptimalImageLayout;
                renderPass->DefineAttachment(inputAttachment.AttachmentDescription);
            }
        }





        // ----------------------------------------------------------------------------------------------------------------------------------
        // Pass Two: Define the pass contents for all the render pass objects using the information compiled in Pass One


        for (PassIndex renderPassIndex : m_ExecutionOrder)
        {
            const RenderGraphPass& pass = m_Passes[renderPassIndex];
            RenderPassHandle& renderPass = m_RenderPasses[renderPassIndex];


            LOG("Creating Render Pass: " << pass.GetName());

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


            //      Note: The rhi render pass object doesn't need to know about read inputs coming from another render pass

            uint32 numOfLocalAttachments = passLocalAttachments.size();
            uint32 attachmentIndex = numOfLocalAttachments;
            for (RenderGraphPass::ExternalAttachment externalAttachment : pass.GetWriteInputAttachments())
            {
                PassIndex externalPassIndex = GetRenderPassIndex(*externalAttachment.OwningPass);
                ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

                AttachmentIndex externalPassAttachmentIndex = externalAttachment.OwningPass->GetLocalAttachment(externalAttachment.Name);
                ASSERT(externalPassAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << externalAttachment.Name << "!")

                RenderGraphPass::LocalAttachment inputAttachment = externalAttachment.OwningPass->GetAttachments()[externalPassAttachmentIndex];

                if (inputAttachment.AttachmentType == AttachmentType::COLOR)
                {
                    renderPass->AddColorAttachment(attachmentIndex, externalAttachment.OptimalImageLayout);
                }
                else if (inputAttachment.AttachmentType == AttachmentType::RESOLVE)
                {
                    renderPass->AddResolveAttachment(attachmentIndex, externalAttachment.OptimalImageLayout);
                }
                else if (inputAttachment.AttachmentType == AttachmentType::DEPTH_STENCIL)
                {
                    renderPass->AddDepthStencilAttachment(attachmentIndex, externalAttachment.OptimalImageLayout);
                }

                attachmentIndex++;
            }

            renderPass->EndBuildingSubpass();
            renderPass->EndBuildingRenderPass();
        }

        // ----------------------------------------------------------------------------------------------------------------------------------
    }


    void RenderGraph::BuildRenderPassResources()
    {
        // ----------------------------------------------------------------------------------------------------------------------------------
        // Build the textures, framebuffers, and descriptor sets for each render pass

        Device& device = RendererAPI::GetDevice();

        m_RenderPassResources.resize(m_ExecutionOrder.size());

        for (PassIndex renderPassIndex : m_ExecutionOrder)
        {
            RenderPassHandle renderPass = m_RenderPasses[renderPassIndex];
            const RenderGraphPass& pass = m_Passes[renderPassIndex];
            std::vector<RenderPassResources>& passResources = m_RenderPassResources[renderPassIndex];
            passResources.resize(m_MaxFramesInFlight);
            ASSERT(passResources.size() == m_MaxFramesInFlight, "The number of copies of resources is expected to be the number of frames in flight!")



            // Textures

            Vec2 passResourceDimensions = pass.GetWriteAttachmentDimensions();
            if (passResourceDimensions.x < 0 && passResourceDimensions.y < 0)
            {
                passResourceDimensions /= -1;
                passResourceDimensions *= m_ViewportDimensions;
            }

            for (const RenderGraphPass::LocalAttachment& localAttachment : pass.GetAttachments())
            {
                // Define attachments for use in the render pass object


                if (renderPassIndex != m_OutputRenderPassIndex || localAttachment.Name != m_OutputAttachmentName)
                {
                    TextureCreateInfo textureCreateInfo = {
                        .Format = localAttachment.AttachmentDescription.Format,
                        .Layout = localAttachment.AttachmentDescription.InitialLayout,
                        .UsageFlags = localAttachment.AttachmentDescription.ImageUsageFlags,
                        .Dimensions = passResourceDimensions,
                        .ImageData = nullptr
                    };


                    // Create the textures for the attachment

                    ASSERT(passResources.size() == m_MaxFramesInFlight, "The number of copies of resources is expected to be the number of frames in flight!")
                    for (size_t i = 0; i < m_MaxFramesInFlight; i++)
                    {
                        TextureHandle attachmentTexture = device.CreateTexture(textureCreateInfo);
                        passResources[i].AttachmentTextures.push_back(attachmentTexture);
                        RendererAPI::NameObject(attachmentTexture, std::string(localAttachment.Name) + "_" + std::to_string(i));
                    }
                }
                else
                {
                    for (size_t i = 0; i < m_MaxFramesInFlight; i++)
                    {
                        TextureHandle attachmentTexture = m_OffscreenOutputTargets[i];
                        passResources[i].AttachmentTextures.push_back(attachmentTexture);
                        RendererAPI::NameObject(attachmentTexture, std::string(localAttachment.Name) + "_" + std::to_string(i));
                    }
                }
            }



            // Framebuffers

            for (size_t i = 0; i < m_MaxFramesInFlight; i++)
            {
                // Create the framebuffers for the render pass
                passResources[i].Framebuffer = device.CreateFramebuffer(renderPass);
                passResources[i].Framebuffer->BeginBuildingFramebuffer(passResourceDimensions.x, passResourceDimensions.y);

                for (const TextureHandle& attachmentTexture : m_RenderPassResources[renderPassIndex][i].AttachmentTextures)
                {
                    passResources[i].Framebuffer->AttachTexture(attachmentTexture);
                }

                passResources[i].Framebuffer->EndBuildingFramebuffer();
                RendererAPI::NameObject(passResources[i].Framebuffer, std::string(pass.GetName()) + "_Framebuffer_" + std::to_string(i));


            // Descriptor Sets

                // Pull external attachment textures from the producer render pass' resources and add them to a descriptor set to read from
                passResources[i].ReadAttachmentDescriptorSet = device.CreateDescriptorSet();
                passResources[i].ReadAttachmentDescriptorSet->BeginBuildingSet();
                for (const RenderGraphPass::ExternalAttachment& externalAttachment : pass.GetReadInputAttachments())
                {
                    PassIndex externalPassIndex = GetRenderPassIndex(*externalAttachment.OwningPass);
                    ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

                    const RenderPassResources& externalPassResources = m_RenderPassResources[externalPassIndex][i];
                    AttachmentIndex externalPassAttachmentIndex = externalAttachment.OwningPass->GetLocalAttachment(externalAttachment.Name);
                    ASSERT(externalPassAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << externalAttachment.Name << "!")

                    TextureHandle externalAttachmentTexture = externalPassResources.AttachmentTextures[externalPassAttachmentIndex];


                    ASSERT(passResources[i].Framebuffer->GetExtent() == (UVec2)passResourceDimensions, "External input texture does not match the dimensions of the target render pass!");

                    passResources[i].ReadAttachmentDescriptorSet->AddDescriptorImageSampler(externalAttachmentTexture, ShaderStage::FRAGMENT);
                }
                passResources[i].ReadAttachmentDescriptorSet->EndBuildingSet();
                RendererAPI::NameObject(passResources[i].ReadAttachmentDescriptorSet, std::string(pass.GetName()) + "_ReadDescriptorSet_" + std::to_string(i));
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


    void RenderGraph::AddRenderGraphResourcesToHold()
    {
        ASSERT(m_RenderPassResourcesHold.size() < 20, "The render graph resources hold is not being cleared correctly!")

        m_RenderPassesHold.push_back(m_RenderPasses);
        m_RenderPassResourcesHold.push_back(m_RenderPassResources);
        m_FramesTillClear.push_back(m_MaxFramesInFlight);
    }


    void RenderGraph::UpdateRenderGraphResourcesHold()
    {
        std::vector<uint32> indicesToRemove;

        // Queueing indices to remove because if it was removed immediately, resources would be
        // shifted and an index might be skipped and the loop would index out of bounds if a removal occured
        for (size_t i = 0; i < m_RenderPassResourcesHold.size(); i++)
        {
            uint32& framesTillClear = m_FramesTillClear[i];

            if (framesTillClear == 0)
            {
                indicesToRemove.push_back(i);
            }
            else
            {
                framesTillClear--;
            }
        }

        // Remove indices in reverse so that the indices remain correct
        for (uint32 indexToRemove : indicesToRemove | std::views::reverse)
        {
            // Remove the render graph resources from hold
            m_RenderPassResourcesHold.erase(m_RenderPassResourcesHold.begin() + indexToRemove);
            m_RenderPassesHold.erase(m_RenderPassesHold.begin() + indexToRemove);
            m_FramesTillClear.erase(m_FramesTillClear.begin() + indexToRemove);
        }
    }

}
