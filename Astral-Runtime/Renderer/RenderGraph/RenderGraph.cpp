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
        PROFILE_SCOPE("RenderGraph::BeginBuildingRenderGraph")

        m_DebugName = debugName;
        m_MaxFramesInFlight = maxFramesInFlight;

        AddRenderGraphResourcesToHold();

        m_Passes.clear();

        m_RenderGraph = DirectedGraph<PassIndex>();
        m_RenderPassNodes.clear();

        m_ExecutionOrder.clear();

        m_RenderPasses.clear();
        m_RenderPassResources.clear();

        m_OutputRenderPassIndex = 0;
        m_OutputAttachmentName = "";

        m_OffscreenOutputTargets.clear();
        m_ViewportDimensions = UVec2(0);

        m_GPUFrameTimeQueryPools.clear();
        m_GPUFrameTimeQueryPools.resize(maxFramesInFlight);

        for (QueryPoolHandle& queryPoolHandle : m_GPUFrameTimeQueryPools)
        {
            queryPoolHandle = RendererAPI::GetDevice().CreateQueryPool(QueryType::TIMESTAMP, 2);
        }
    }


    void RenderGraph::AddPass(const RenderGraphPass& pass)
    {
        m_Passes.push_back(pass);
    }


    void RenderGraph::AddOutputPass(const RenderGraphPass& pass)
    {
        m_Passes.push_back(pass);
        m_OutputRenderPassIndex = m_Passes.size() - 1;
    }


    void RenderGraph::SetOutputAttachment(const RenderGraphPass& pass, std::string_view attachmentName, const std::vector<TextureHandle>& offscreenTargets)
    {
        m_OutputAttachmentName = attachmentName;

        m_IsOutputRenderTarget = false;
        m_OffscreenOutputTargets = offscreenTargets;
        ASSERT(m_OffscreenOutputTargets.size() == m_MaxFramesInFlight, "Render Graph: Number of output textures does not match the number of frames in flight!")

        m_OutputAttachmentPass = GetRenderPassIndex(pass);
        ASSERT(m_OutputAttachmentPass != NullRenderPassIndex, "Attempting to set output attachment from render pass not in render graph!")

        m_ViewportDimensions = m_OffscreenOutputTargets[0]->GetDimensions();
    }


    void RenderGraph::SetOutputAttachment(const RenderGraphPass& pass, std::string_view attachmentName, const std::vector<RenderTargetHandle>& swapchainTargets)
    {
        m_OutputAttachmentName = attachmentName;
        ASSERT(swapchainTargets.size() == m_MaxFramesInFlight, "Render Graph: Number of output textures does not match the number of frames in flight!")

        m_IsOutputRenderTarget = true;
        for (RenderTargetHandle renderTarget : swapchainTargets)
        {
            m_OffscreenOutputTargets.push_back(renderTarget->GetAsTexture());
        }

        m_OutputAttachmentPass = GetRenderPassIndex(pass);
        ASSERT(m_OutputAttachmentPass != NullRenderPassIndex, "Attempting to set output attachment from render pass not in render graph!")

        m_ViewportDimensions = m_OffscreenOutputTargets[0]->GetDimensions();
    }


    void RenderGraph::EndBuildingRenderGraph()
    {
        PROFILE_SCOPE("RenderGraph::EndBuildingRenderGraph")

        BuildRenderGraph();
        SolveRenderPassExecutionOrder();
        CompileRenderPassBarriers();
        BuildRenderPassObjects();
        BuildRenderPassResources();
    }


    void RenderGraph::Execute(CommandBufferHandle commandBuffer, uint32 swapchainImageIndex)
    {
        PROFILE_SCOPE("RenderGraph::Execute")

        UpdateRenderGraphResourcesHold();
        m_ExecutionContext.CommandBuffer = commandBuffer;


        commandBuffer->BeginLabel(m_DebugName, Vec4(1.0 , 1.0, 0, 1.0));


        QueryPoolHandle frameQueryPool = m_GPUFrameTimeQueryPools[swapchainImageIndex];

        if (!frameQueryPool->NeedsReset())
        {
            std::array<uint64, 2> frameTimeResults;
            bool isFrameTimesAvailable = frameQueryPool->GetTimestampQueryResults(frameTimeResults);
            if (isFrameTimesAvailable)
            {
                uint64 frameTimeInNanoseconds = frameTimeResults[1] - frameTimeResults[0];
                double frameTimeInMilliseconds = frameTimeInNanoseconds * RendererAPI::GetDevice().GetTimestampPeriod() / 1000000.0f;
                m_LatestGPUFrameTime = frameTimeInMilliseconds;
            }
        }

        commandBuffer->ResetQueryPool(frameQueryPool);
        commandBuffer->WriteTimestamp(frameQueryPool, PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0);


        for (size_t i = 0; i < m_ExecutionOrder.size(); i++)
        {
            PassIndex renderPassIndex = m_ExecutionOrder[i];
            const RenderGraphPass& pass = m_Passes[renderPassIndex];
            RenderPassHandle rhiRenderPass = m_RenderPasses[renderPassIndex];
            RenderPassResources& renderPassResource = m_RenderPassResources[renderPassIndex][swapchainImageIndex];


            m_ExecutionContext.RenderPass = rhiRenderPass;
            m_ExecutionContext.ReadAttachments = renderPassResource.ReadAttachmentDescriptorSet;

            TransitionReadAttachmentLayouts(commandBuffer, pass, swapchainImageIndex);


            commandBuffer->BeginLabel(pass.GetName(), Vec4(1.0 , 0.0, 1.0, 1.0));
            commandBuffer->BeginRenderPass(rhiRenderPass, renderPassResource.Framebuffer);

            pass.Execute();

            commandBuffer->EndRenderPass();
            commandBuffer->EndLabel();
        }

        // Transition all attachment layouts to their initial starting layout
        PipelineBarrier pipelineBarrier = {};
        pipelineBarrier.SourceStageMask = PIPELINE_STAGE_VERTEX_SHADER_BIT | PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
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

                if (attachmentTexture->GetLayout() == pass.GetAttachments()[j].InitialLayout) { continue; }

                ImageMemoryBarrier imageMemoryBarrier = {};
                imageMemoryBarrier.SourceAccessMask = ACCESS_FLAGS_SHADER_READ_BIT | ACCESS_FLAGS_COLOR_ATTACHMENT_WRITE_BIT | ACCESS_FLAGS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                imageMemoryBarrier.DestinationAccessMask = ACCESS_FLAGS_SHADER_READ_BIT | ACCESS_FLAGS_COLOR_ATTACHMENT_WRITE_BIT | ACCESS_FLAGS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                imageMemoryBarrier.OldLayout = attachmentTexture->GetLayout();
                imageMemoryBarrier.NewLayout = pass.GetAttachments()[j].InitialLayout;
                imageMemoryBarrier.SourceQueueFamilyIndex = QueueFamilyIgnored;
                imageMemoryBarrier.DestinationQueueFamilyIndex = QueueFamilyIgnored;
                imageMemoryBarrier.Image = attachmentTexture;
                imageMemoryBarrier.ImageSubresourceRange = {
                    .AspectMask = attachmentTexture->GetImageAspect(),
                    .BaseMipLevel = 0,
                    .LevelCount = attachmentTexture->GetNumMipLevels(),
                    .BaseArrayLayer = 0,
                    .LayerCount = attachmentTexture->GetNumLayers()
                };

                pipelineBarrier.ImageMemoryBarriers.push_back(imageMemoryBarrier);
            }
        }
        commandBuffer->SetPipelineBarrier(pipelineBarrier);

        commandBuffer->WriteTimestamp(frameQueryPool, PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 1);

        commandBuffer->EndLabel();

        m_ExecutionContext = {};
    }


    void RenderGraph::ResizeResources(const std::vector<TextureHandle>& offscreenTargets)
    {
        PROFILE_SCOPE("RenderGraph::ResizeResources")

        m_OffscreenOutputTargets = offscreenTargets;
        ASSERT(m_OffscreenOutputTargets.size() == m_MaxFramesInFlight, "Render Graph: Number of output textures does not match the number of frames in flight!")
        m_ViewportDimensions = m_OffscreenOutputTargets[0]->GetDimensions();

        AddRenderGraphResourcesToHold();

        m_RenderPassResources.clear();
        BuildRenderPassResources();
    }


    void RenderGraph::ResizeResources(const std::vector<RenderTargetHandle>& swapchainTargets)
    {
        PROFILE_SCOPE("RenderGraph::ResizeResources")

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


    double RenderGraph::GetLatestGPUFrameTime() const
    {
        return m_LatestGPUFrameTime;
    }


    void RenderGraph::BuildRenderGraph()
    {
        PROFILE_SCOPE("RenderGraph::BuildRenderGraph")

        // Converting each external attachment to all render passes to point to the render graph owned render pass instances
        for (size_t renderPassIndex = 0; renderPassIndex < m_Passes.size(); renderPassIndex++)
        {
            for (RenderGraphPass::ExternalAttachment& externalAttachment : m_Passes[renderPassIndex].GetReadInputAttachments())
            {
                PassIndex externalPassIndex = GetRenderPassIndex(*externalAttachment.OwningPass);
                ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")
                externalAttachment.OwningPass = &m_Passes[externalPassIndex];
            }

            for (RenderGraphPass::ExternalAttachment& externalAttachment : m_Passes[renderPassIndex].GetWriteInputAttachments())
            {
                PassIndex externalPassIndex = GetRenderPassIndex(*externalAttachment.OwningPass);
                ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")
                externalAttachment.OwningPass = &m_Passes[externalPassIndex];
            }

            for (RenderGraphPass*& dependentPass : m_Passes[renderPassIndex].GetExplicitDependencies())
            {
                PassIndex externalPassIndex = GetRenderPassIndex(*dependentPass);
                ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")
                dependentPass = &m_Passes[externalPassIndex];
            }

        }

        // Add all the render passes as a node in the render graph
        for (size_t renderPassIndex = 0; renderPassIndex < m_Passes.size(); renderPassIndex++)
        {
            DirectedGraph<PassIndex>::Vertex renderPassNode = m_RenderGraph.AddVertex(renderPassIndex);
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

                DirectedGraph<PassIndex>::Vertex& producingPassNode = m_RenderPassNodes[externalRenderPassIndex];
                DirectedGraph<PassIndex>::Vertex& consumingPassNode = m_RenderPassNodes[i];
                consumingPassNode.AddEdge(producingPassNode, externalRenderPassIndex); // User node depends on origin node
            }

            for (const RenderGraphPass::ExternalAttachment& externalAttachment : consumingPass.GetWriteInputAttachments())
            {
                const std::string_view& inputAttachmentName = externalAttachment.Name;
                RenderGraphPass& producerPassScopeLocal = *externalAttachment.OwningPass;
                AttachmentIndex localAttachmentIndex = producerPassScopeLocal.GetLocalAttachment(inputAttachmentName);
                ASSERT(localAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << inputAttachmentName << "!")

                PassIndex externalRenderPassIndex = GetRenderPassIndex(producerPassScopeLocal);
                ASSERT(externalRenderPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

                DirectedGraph<PassIndex>::Vertex& producingPassNode = m_RenderPassNodes[externalRenderPassIndex];
                DirectedGraph<PassIndex>::Vertex& consumingPassNode = m_RenderPassNodes[i];
                consumingPassNode.AddEdge(producingPassNode, externalRenderPassIndex); // User node depends on origin node
            }

            for (RenderGraphPass* dependentPass : consumingPass.GetExplicitDependencies())
            {
                PassIndex externalRenderPassIndex = GetRenderPassIndex(*dependentPass);
                ASSERT(externalRenderPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

                DirectedGraph<PassIndex>::Vertex& earlierPassNode = m_RenderPassNodes[externalRenderPassIndex];
                DirectedGraph<PassIndex>::Vertex& dependentPassNode = m_RenderPassNodes[i];
                dependentPassNode.AddEdge(earlierPassNode, externalRenderPassIndex); // User node depends on origin node
            }
        }
    }


    void RenderGraph::SolveRenderPassExecutionOrder()
    {
        PROFILE_SCOPE("RenderGraph::SolveRenderPassExecutionOrder")

        // NOTE: For this render graph, number of out degrees equals number of dependencies

        // First a DFS to find all the nodes with 0 out degrees
        std::stack<DirectedGraph<PassIndex>::Vertex> nodesToVisit;
        std::unordered_set<DirectedGraph<PassIndex>::Vertex> visitedNodes;

        std::unordered_map<DirectedGraph<PassIndex>::Vertex, uint32> outDegreesOfNodes;
        std::unordered_map<DirectedGraph<PassIndex>::Vertex, std::vector<DirectedGraph<PassIndex>::Vertex>> directDependentsOfNode;
        std::queue<DirectedGraph<PassIndex>::Vertex> nodesWithZeroOutDegrees;

        DirectedGraph<PassIndex>::Vertex outputNode = m_RenderPassNodes[m_OutputRenderPassIndex];
        nodesToVisit.push(outputNode);

        while (!nodesToVisit.empty())
        {
            DirectedGraph<PassIndex>::Vertex currentNode = nodesToVisit.top();
            nodesToVisit.pop();

            if (visitedNodes.contains(currentNode)) { continue; }
            visitedNodes.insert(currentNode);

            // Push all nodes that share an edge with the current node and count the out degrees
            uint32 outDegrees = 0;
            for (DirectedGraph<PassIndex>::Edge edge : currentNode)
            {
                DirectedGraph<PassIndex>::Vertex dependencyPassNode = edge.GetRightVertex();
                nodesToVisit.push(dependencyPassNode);
                outDegrees++;

                directDependentsOfNode[dependencyPassNode].push_back(currentNode);
            }
            if (outDegrees == 0) { nodesWithZeroOutDegrees.push(currentNode); }
            outDegreesOfNodes[currentNode] = outDegrees;
        }



        // Kahn's Algorithm for the Topological Sort

        while (!nodesWithZeroOutDegrees.empty())
        {
            DirectedGraph<PassIndex>::Vertex currentNode = nodesWithZeroOutDegrees.front();
            nodesWithZeroOutDegrees.pop();

            m_ExecutionOrder.push_back(currentNode.GetData());

            // Decrement the out degrees for in degrees of the current node
            for (DirectedGraph<PassIndex>::Vertex& dependentNode : directDependentsOfNode[currentNode])
            {
                outDegreesOfNodes[dependentNode]--;
                if (outDegreesOfNodes[dependentNode] <= 0) { nodesWithZeroOutDegrees.push(dependentNode); }
            }
        }

        ASSERT(m_ExecutionOrder.size() == m_RenderGraph.NumOfVertices(), "Cycle detected in render graph!")
    }


    void RenderGraph::CompileRenderPassBarriers()
    {
        PROFILE_SCOPE("RenderGraph::CompileRenderPassBarriers")

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
            }
        }
    }


    void RenderGraph::TransitionReadAttachmentLayouts(CommandBufferHandle commandBuffer, const RenderGraphPass& pass, uint32 swapchainImageIndex)
    {
        // Transition all read input attachment layouts to SHADER_READ_ONLY_OPTIMAL if it is not already in it
        PipelineBarrier pipelineBarrier = {};
        pipelineBarrier.SourceStageMask = PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
        pipelineBarrier.DestinationStageMask = PIPELINE_STAGE_VERTEX_SHADER_BIT | PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        pipelineBarrier.DependencyFlags = DependencyFlags::BY_REGION_BIT;

        for (const RenderGraphPass::ExternalAttachment& readInputAttachment : pass.GetReadInputAttachments())
        {
            PassIndex externalPassIndex = GetRenderPassIndex(*readInputAttachment.OwningPass);
            ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

            const RenderPassResources& externalPassResources = m_RenderPassResources[externalPassIndex][swapchainImageIndex];

            AttachmentIndex externalPassAttachmentIndex = readInputAttachment.OwningPass->GetLocalAttachment(readInputAttachment.Name);
            ASSERT(externalPassAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << readInputAttachment.Name << "!")

            TextureHandle externalAttachmentTexture = externalPassResources.AttachmentTextures[externalPassAttachmentIndex];

            if (externalAttachmentTexture->GetLayout() == ImageLayout::SHADER_READ_ONLY_OPTIMAL) { continue; }

            ImageMemoryBarrier imageMemoryBarrier = {};
            imageMemoryBarrier.SourceAccessMask = ACCESS_FLAGS_COLOR_ATTACHMENT_WRITE_BIT | ACCESS_FLAGS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            imageMemoryBarrier.DestinationAccessMask = ACCESS_FLAGS_SHADER_READ_BIT;
            imageMemoryBarrier.OldLayout = externalAttachmentTexture->GetLayout();
            imageMemoryBarrier.NewLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL;
            imageMemoryBarrier.SourceQueueFamilyIndex = QueueFamilyIgnored;
            imageMemoryBarrier.DestinationQueueFamilyIndex = QueueFamilyIgnored;
            imageMemoryBarrier.Image = externalAttachmentTexture;
            imageMemoryBarrier.ImageSubresourceRange = {
                .AspectMask = externalAttachmentTexture->GetImageAspect(),
                .BaseMipLevel = 0,
                .LevelCount = externalAttachmentTexture->GetNumMipLevels(),
                .BaseArrayLayer = 0,
                .LayerCount = externalAttachmentTexture->GetNumLayers()
            };

            pipelineBarrier.ImageMemoryBarriers.push_back(imageMemoryBarrier);
        }

        commandBuffer->SetPipelineBarrier(pipelineBarrier);
    }


    void RenderGraph::BuildRenderPassObjects()
    {
        PROFILE_SCOPE("RenderGraph::BuildRenderPassObjects")

        // ----------------------------------------------------------------------------------------------------------------------------------
        // Pass One: Define the attachments for all the render passes ahead of building the render pass objects

        Device& device = RendererAPI::GetDevice();
        m_RenderPasses.resize(m_Passes.size(), nullptr); // TODO: Change this to resize to the amount in execution order and change indexing of the vector to reflect that

        for (PassIndex renderPassIndex : m_ExecutionOrder)
        {
            RenderGraphPass& pass = m_Passes[renderPassIndex];
            RenderPassHandle& renderPass = m_RenderPasses[renderPassIndex];
            renderPass = device.CreateRenderPass();
            renderPass->BeginBuildingRenderPass();

            for (RenderGraphPass::LocalAttachment& localAttachment : pass.GetAttachments())
            {

                // Manage layout transitions
                localAttachment.AttachmentDescription.InitialLayout = localAttachment.LastKnownLayout;
                localAttachment.AttachmentDescription.FinalLayout = localAttachment.OptimalImageLayout;

                if (renderPassIndex == m_OutputAttachmentPass && localAttachment.Name == m_OutputAttachmentName)
                {
                    localAttachment.AttachmentDescription.InitialLayout = m_OffscreenOutputTargets[0]->GetLayout();
                    localAttachment.InitialLayout = m_OffscreenOutputTargets[0]->GetLayout();
                    localAttachment.AttachmentDescription.Format = m_OffscreenOutputTargets[0]->GetFormat();

                    if (m_IsOutputRenderTarget && m_OffscreenOutputTargets[0]->GetLayout() == ImageLayout::UNDEFINED)
                    {
                        localAttachment.InitialLayout = ImageLayout::PRESENT_SRC_KHR;
                    }
                }

                localAttachment.LastKnownLayout = localAttachment.AttachmentDescription.FinalLayout;


                // Define attachments for use in the render pass object
                renderPass->DefineAttachment(localAttachment.AttachmentDescription);

            }


            // Track the layout transitions for read attachments
            for (RenderGraphPass::ExternalAttachment& externalAttachment : pass.GetReadInputAttachments())
            {
                PassIndex externalPassIndex = GetRenderPassIndex(*externalAttachment.OwningPass);
                ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

                AttachmentIndex externalPassAttachmentIndex = externalAttachment.OwningPass->GetLocalAttachment(externalAttachment.Name);
                ASSERT(externalPassAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << externalAttachment.Name << "!")


                RenderGraphPass::LocalAttachment& inputAttachment = externalAttachment.OwningPass->GetAttachments()[externalPassAttachmentIndex];

                // Manage layout transitions
                inputAttachment.AttachmentDescription.InitialLayout = inputAttachment.LastKnownLayout;
                inputAttachment.AttachmentDescription.FinalLayout = externalAttachment.OptimalImageLayout;
                inputAttachment.LastKnownLayout = inputAttachment.AttachmentDescription.FinalLayout;
            }


            // Define the external write attachments for the render pass
            for (RenderGraphPass::ExternalAttachment& externalAttachment : pass.GetWriteInputAttachments())
            {
                PassIndex externalPassIndex = GetRenderPassIndex(*externalAttachment.OwningPass);
                ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

                AttachmentIndex externalPassAttachmentIndex = externalAttachment.OwningPass->GetLocalAttachment(externalAttachment.Name);
                ASSERT(externalPassAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << externalAttachment.Name << "!")


                RenderGraphPass::LocalAttachment& inputAttachment = externalAttachment.OwningPass->GetAttachments()[externalPassAttachmentIndex];

                if (inputAttachment.AttachmentType == AttachmentType::RESOLVE)
                {
                    inputAttachment.AttachmentType = AttachmentType::COLOR; // Force color attachment when using LinkWriteInputAttachment
                }
                
                // Manage layout transitions
                inputAttachment.AttachmentDescription.LoadOp = AttachmentLoadOp::LOAD;
                inputAttachment.AttachmentDescription.InitialLayout = inputAttachment.LastKnownLayout;
                inputAttachment.AttachmentDescription.FinalLayout = externalAttachment.OptimalImageLayout;
                inputAttachment.LastKnownLayout = inputAttachment.AttachmentDescription.FinalLayout;
                renderPass->DefineAttachment(inputAttachment.AttachmentDescription);
            }
        }





        // ----------------------------------------------------------------------------------------------------------------------------------
        // Pass Two: Define the pass contents for all the render pass objects using the information compiled in Pass One


        for (PassIndex renderPassIndex : m_ExecutionOrder)
        {
            const RenderGraphPass& pass = m_Passes[renderPassIndex];
            RenderPassHandle& renderPass = m_RenderPasses[renderPassIndex];


            AE_LOG("Creating Render Pass: " << pass.GetName());

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

            RendererAPI::NameObject(renderPass, pass.GetName());
        }

        // ----------------------------------------------------------------------------------------------------------------------------------
    }


    void RenderGraph::BuildRenderPassResources()
    {
        PROFILE_SCOPE("RenderGraph::BuildRenderPassResources")

        // ----------------------------------------------------------------------------------------------------------------------------------
        // Build the textures, framebuffers, and descriptor sets for each render pass

        Device& device = RendererAPI::GetDevice();

        m_RenderPassResources.resize(m_Passes.size()); // TODO: Change this to resize to the amount in execution order and change indexing of the vector to reflect that

        for (PassIndex renderPassIndex : m_ExecutionOrder)
        {
            RenderPassHandle renderPass = m_RenderPasses[renderPassIndex];
            const RenderGraphPass& pass = m_Passes[renderPassIndex];
            std::vector<RenderPassResources>& passResources = m_RenderPassResources[renderPassIndex];
            passResources.resize(m_MaxFramesInFlight);

            // Textures

            Vec3 passResourceDimensions = pass.GetWriteAttachmentDimensions();

            if (passResourceDimensions.x < 0 && passResourceDimensions.y < 0)
            {
                passResourceDimensions /= -1;
                passResourceDimensions.x *= (float)m_ViewportDimensions.x;
                passResourceDimensions.y *= (float)m_ViewportDimensions.y;
                passResourceDimensions.z = 1;
            }

            for (const RenderGraphPass::LocalAttachment& localAttachment : pass.GetAttachments())
            {
                // Define attachments for use in the render pass object


                if (renderPassIndex != m_OutputAttachmentPass || localAttachment.Name != m_OutputAttachmentName)
                {
                    TextureCreateInfo textureCreateInfo = {
                        .Format = localAttachment.AttachmentDescription.Format,
                        .Layout = localAttachment.InitialLayout,
                        .UsageFlags = localAttachment.AttachmentDescription.ImageUsageFlags,
                        .Dimensions = passResourceDimensions,
                        .ImageData = nullptr,
                        .LayerCount = localAttachment.AttachmentDescription.LayerCount,
                        .MipMapCount = localAttachment.AttachmentDescription.MipMapCount,
                        .MSAASampleCount = localAttachment.AttachmentDescription.MSAASamples
                    };


                    // Create the textures for the attachment

                    ASSERT(passResources.size() == m_MaxFramesInFlight, "The number of copies of resources is expected to be the number of frames in flight!")
                    for (size_t i = 0; i < m_MaxFramesInFlight; i++)
                    {
                        TextureHandle attachmentTexture;
                        if (localAttachment.AttachmentDescription.TextureType == TextureType::IMAGE_2D)
                        {
                            attachmentTexture = device.CreateTexture(textureCreateInfo);
                        }
                        else if (localAttachment.AttachmentDescription.TextureType == TextureType::IMAGE_2D_ARRAY)
                        {
                            attachmentTexture = device.Create2DTextureArray(textureCreateInfo);
                        }
                        else if (localAttachment.AttachmentDescription.TextureType == TextureType::IMAGE_3D)
                        {
                            attachmentTexture = device.Create3DTexture(textureCreateInfo);
                        }
                        else if (localAttachment.AttachmentDescription.TextureType == TextureType::IMAGE_1D)
                        {
                            attachmentTexture = device.Create1DTexture(textureCreateInfo);
                        }
                        else if (localAttachment.AttachmentDescription.TextureType == TextureType::CUBEMAP)
                        {
                            attachmentTexture = device.CreateCubemap(textureCreateInfo);
                        }

                        passResources[i].AttachmentTextures.push_back(attachmentTexture);
                        RendererAPI::NameObject(attachmentTexture, std::string(localAttachment.Name) + "_" + std::to_string(i) + "_Batch_" + std::to_string(m_ResourceBatchNumber));
                    }
                }
                else
                {
                    for (size_t i = 0; i < m_MaxFramesInFlight; i++)
                    {
                        TextureHandle attachmentTexture = m_OffscreenOutputTargets[i];
                        passResources[i].AttachmentTextures.push_back(attachmentTexture);
                        RendererAPI::NameObject(attachmentTexture, std::string(localAttachment.Name) + "_" + std::to_string(i) + "_Batch_" + std::to_string(m_ResourceBatchNumber));
                    }
                }
            }



            // Note:
            // The textures that are being used in the framebuffers and descriptor sets should already be created
            // even though we are creating the frame buffers and descriptor sets at the same time as the textures
            // because the order of creation is in the order of the execution of the render passes. Therefore, the
            // resources used in this render pass should have been created in an earlier loop.


            // Framebuffers
            for (size_t i = 0; i < m_MaxFramesInFlight; i++)
            {
                // Create the framebuffers for the render pass
                passResources[i].Framebuffer = device.CreateFramebuffer(renderPass);
                passResources[i].Framebuffer->BeginBuildingFramebuffer(passResourceDimensions.x, passResourceDimensions.y, passResourceDimensions.z);

                for (const TextureHandle& attachmentTexture : m_RenderPassResources[renderPassIndex][i].AttachmentTextures)
                {
                    passResources[i].Framebuffer->AttachTexture(attachmentTexture);
                }

                for (const RenderGraphPass::ExternalAttachment& externalAttachment : pass.GetWriteInputAttachments())
                {
                    PassIndex externalPassIndex = GetRenderPassIndex(*externalAttachment.OwningPass);
                    ASSERT(externalPassIndex != NullRenderPassIndex, "Render pass does not exist in render graph!")

                    const RenderPassResources& externalPassResources = m_RenderPassResources[externalPassIndex][i];
                    AttachmentIndex externalPassAttachmentIndex = externalAttachment.OwningPass->GetLocalAttachment(externalAttachment.Name);
                    ASSERT(externalPassAttachmentIndex != NullAttachmentIndex, "External render pass does not contain attachment by name " << externalAttachment.Name << "!")

                    TextureHandle externalAttachmentTexture = externalPassResources.AttachmentTextures[externalPassAttachmentIndex];

                    passResources[i].Framebuffer->AttachTexture(externalAttachmentTexture);
                }

                passResources[i].Framebuffer->EndBuildingFramebuffer();
                RendererAPI::NameObject(passResources[i].Framebuffer, std::string(pass.GetName()) + "_Framebuffer_" + std::to_string(i) + "_Batch_" + std::to_string(m_ResourceBatchNumber));


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
                RendererAPI::NameObject(passResources[i].ReadAttachmentDescriptorSet, std::string(pass.GetName()) + "_ReadDescriptorSet_" + std::to_string(i) + "_Batch_" + std::to_string(m_ResourceBatchNumber));
            }
        }

        m_ResourceBatchNumber++;
    }


    PassIndex RenderGraph::GetRenderPassIndex(const RenderGraphPass& pass)
    {
        for (int i = 0; i < m_Passes.size(); i++)
        {
            if (m_Passes[i].GetName() == pass.GetName()) { return i; }
        }

        return NullRenderPassIndex;
    }


    void RenderGraph::AddRenderGraphResourcesToHold()
    {
        PROFILE_SCOPE("RenderGraph::AddRenderGraphResourcesToHold")

        ASSERT(m_RenderPassResourcesHold.size() < 20, "The render graph resources hold is not being cleared correctly!")

        m_RenderPassesHold.push_back(m_RenderPasses);
        m_RenderPassResourcesHold.push_back(m_RenderPassResources);
        m_FramesTillClear.push_back(m_MaxFramesInFlight);
    }


    void RenderGraph::UpdateRenderGraphResourcesHold()
    {
        PROFILE_SCOPE("RenderGraph::UpdateRenderGraphResourcesHold")

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
