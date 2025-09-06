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
        return (RenderPass ? RenderPass->GetNativeHandle() : nullptr) == (other.RenderPass ? other.RenderPass->GetNativeHandle() : nullptr) &&
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


    void PipelineStateCache::SetDescriptorSetStack(const DescriptorSetHandle& descriptorSet)
    {
        m_DescriptorSetStack = std::vector<DescriptorSetHandle>();
        m_DescriptorSetStack.push_back(descriptorSet);
    }


    void PipelineStateCache::SetDescriptorSetStack(const std::vector<DescriptorSetHandle>& descriptorSets)
    {
        m_DescriptorSetStack = descriptorSets;
    }


    PipelineStateHandle PipelineStateCache::GetGraphicsPipeline(const RenderPassHandle& renderPass, Material& material, Mesh& mesh, uint32 subpassIndex, SampleCount msaaSampleCount)
    {
        // Build pipeline configuration struct

        GraphicsPipelineStateConfiguration pipelineStateConfiguration = {
            .RenderPass = renderPass,
            .VertexShader = mesh.VertexShader,
            .FragmentShader = material.FragmentShader,
            .ShaderDataLayout = material.DescriptorSet ? material.DescriptorSet->GetDescriptorSetLayout() : DescriptorSetLayout(),
            .VertexBufferLayout = mesh.VertexBuffer->GetBufferLayout(),
            .IsAlphaBlended = material.IsAlphaBlended,
            .MSAASampleCount = msaaSampleCount,
        };

        // If the pipeline was created already, return it
        if (m_GraphicsPipelineCache.contains(pipelineStateConfiguration)) { return m_GraphicsPipelineCache[pipelineStateConfiguration]; }


        Device& device = RendererAPI::GetDevice();

        // Pipeline doesn't exist yet, so we create it now

        // Set up descriptor set layouts of the pipeline
        std::vector<DescriptorSetHandle> descriptorSets = m_DescriptorSetStack;
        if (material.DescriptorSet) { descriptorSets.push_back(material.DescriptorSet); }

        GraphicsPipelineStateCreateInfo pipelineStateObjectCreateInfo = {
            .RenderPass = renderPass,
            .VertexShader = mesh.VertexShader,
            .FragmentShader = material.FragmentShader,
            .DescriptorSets = descriptorSets,
            .BufferLayout = mesh.VertexBuffer->GetBufferLayout(),
            .SubpassIndex = subpassIndex,
            .IsAlphaBlended = material.IsAlphaBlended,
            .MSAASamples = pipelineStateConfiguration.MSAASampleCount
        };

        PipelineStateHandle pipelineStateObject = device.CreateGraphicsPipelineState(pipelineStateObjectCreateInfo);
        m_GraphicsPipelineCache[pipelineStateConfiguration] = pipelineStateObject;

        return pipelineStateObject;
    }

    PipelineStateHandle PipelineStateCache::GetComputePipeline(ShaderHandle computeShader, DescriptorSetHandle descriptorSet)
    {
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
        std::vector<DescriptorSetHandle> descriptorSets = m_DescriptorSetStack;
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
