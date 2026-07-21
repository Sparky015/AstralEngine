/**
* @file PipelineStateCache.cpp
* @author Andrew Fagan
* @date 6/27/2025
*/

#include "PipelineStateCache.h"

#include "Renderer/RHI/RendererAPI.h"

namespace Astral {

    bool GraphicsPipelineStateConfiguration::operator==(const GraphicsPipelineStateConfiguration& other) const
    {
        return (RenderPass ? RenderPass.get() : nullptr) == (other.RenderPass ? other.RenderPass.get() : nullptr) &&
               (VertexShader ? VertexShader->GetNativeHandle() : nullptr) == (other.VertexShader ? other.VertexShader->GetNativeHandle() : nullptr) &&
               (FragmentShader ? FragmentShader->GetNativeHandle() : nullptr) == (other.FragmentShader ? other.FragmentShader->GetNativeHandle() : nullptr) &&
               ShaderDataLayout.Descriptors == other.ShaderDataLayout.Descriptors &&
               VertexBufferLayout == other.VertexBufferLayout;
    }


    bool ComputePipelineStateConfiguration::operator==(const ComputePipelineStateConfiguration& other) const
    {
        return (ComputeShader ? ComputeShader->GetNativeHandle() : nullptr) == (other.ComputeShader ? other.ComputeShader->GetNativeHandle() : nullptr) &&
               ShaderDataLayout.Descriptors == other.ShaderDataLayout.Descriptors;
    }


    PipelineStateHandle PipelineStateCache::GetGraphicsPipeline(const GraphicsPipelineStateConfiguration& graphicsPipelineStateConfiguration, const std::vector<DescriptorSetHandle>& descriptorSetStack)
    {
        std::shared_lock readerLock(m_PipelineCacheMutex);

        // If the pipeline was created already, return it
        if (m_GraphicsPipelineCache.contains(graphicsPipelineStateConfiguration)) { return m_GraphicsPipelineCache[graphicsPipelineStateConfiguration]; }


        // Pipeline doesn't exist yet, so we create it now

        GraphicsPipelineStateCreateInfo pipelineStateObjectCreateInfo = {
            .RenderPass = graphicsPipelineStateConfiguration.RenderPass,
            .VertexShader = graphicsPipelineStateConfiguration.VertexShader,
            .FragmentShader = graphicsPipelineStateConfiguration.FragmentShader,
            .DescriptorSets = descriptorSetStack,
            .BufferLayout = graphicsPipelineStateConfiguration.VertexBufferLayout,
            .SubpassIndex = 0,
            .IsAlphaBlended = graphicsPipelineStateConfiguration.IsAlphaBlended,
            .MSAASamples = graphicsPipelineStateConfiguration.MSAASampleCount,
            .CullMode = graphicsPipelineStateConfiguration.CullMode
        };

        Device& device = RendererAPI::GetDevice();

        readerLock.unlock();
        std::unique_lock writerLock(m_PipelineCacheMutex);

        PipelineStateHandle pipelineStateObject = device.CreateGraphicsPipelineState(pipelineStateObjectCreateInfo);
        m_GraphicsPipelineCache[graphicsPipelineStateConfiguration] = pipelineStateObject;

        return pipelineStateObject;
    }


    PipelineStateHandle PipelineStateCache::GetComputePipeline(ShaderHandle computeShader, DescriptorSetHandle descriptorSet, const std::vector<DescriptorSetHandle>&
                                                               descriptorSetStack)
    {
        std::unique_lock lock(m_PipelineCacheMutex);

        // Build compute pipeline configuration struct

        ComputePipelineStateConfiguration pipelineStateConfiguration = {
            .ComputeShader = computeShader,
            .ShaderDataLayout = descriptorSet->GetDescriptorSetLayout(),
        };

        // If the pipeline was created already, return it
        if (m_ComputePipelineCache.contains(pipelineStateConfiguration)) { return m_ComputePipelineCache[pipelineStateConfiguration]; }


        Device& device = RendererAPI::GetDevice();

        // Pipeline doesn't exist yet, so we create it now

        // Set up descriptor set layouts of the pipeline
        std::vector<DescriptorSetHandle> descriptorSets = descriptorSetStack;
        if (descriptorSet) { descriptorSets.push_back(descriptorSet); }

        ComputePipelineStateCreateInfo pipelineStateObjectCreateInfo = {
            .ComputeShader = computeShader,
            .DescriptorSets = descriptorSets,
        };

        PipelineStateHandle pipelineStateObject = device.CreateComputePipelineState(pipelineStateObjectCreateInfo);
        m_ComputePipelineCache[pipelineStateConfiguration] = pipelineStateObject;

        return pipelineStateObject;
    }
}
