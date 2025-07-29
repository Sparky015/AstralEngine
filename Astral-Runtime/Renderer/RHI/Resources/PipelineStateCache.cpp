/**
* @file PipelineStateCache.cpp
* @author Andrew Fagan
* @date 6/27/2025
*/

#include "PipelineStateCache.h"

#include "Renderer/RHI/RendererAPI.h"

namespace Astral {

    bool PipelineStateConfiguration::operator==(const PipelineStateConfiguration& other) const
    {
        return (RenderPass ? RenderPass->GetNativeHandle() : nullptr) == (other.RenderPass ? other.RenderPass->GetNativeHandle() : nullptr) &&
               (VertexShader ? VertexShader->GetNativeHandle() : nullptr) == (other.VertexShader ? other.VertexShader->GetNativeHandle() : nullptr) &&
               (FragmentShader ? FragmentShader->GetNativeHandle() : nullptr) == (other.FragmentShader ? other.FragmentShader->GetNativeHandle() : nullptr) &&
               ShaderDataLayout.Descriptors == other.ShaderDataLayout.Descriptors &&
               VertexBufferLayout == other.VertexBufferLayout;
    }


    void PipelineStateCache::SetSceneDescriptorSet(DescriptorSetHandle sceneDescriptorSet)
    {
        m_SceneDescriptorSet = sceneDescriptorSet;
    }


    PipelineStateObjectHandle PipelineStateCache::GetPipeline(RenderPassHandle renderPass, Material& material, Mesh& mesh, uint32 subpassIndex)
    {
        // Build pipeline configuration struct

        PipelineStateConfiguration pipelineStateConfiguration = {
            .RenderPass = renderPass,
            .VertexShader = mesh.VertexShader,
            .FragmentShader = material.FragmentShader,
            .ShaderDataLayout = material.DescriptorSet->GetDescriptorSetLayout(),
            .VertexBufferLayout = mesh.VertexBuffer->GetBufferLayout()
        };

        // If the pipeline was created already, return it
        if (m_PipelineCache.contains(pipelineStateConfiguration)) { return m_PipelineCache[pipelineStateConfiguration]; }


        // Pipeline doesn't exist yet, so we create it now
        ASSERT(m_SceneDescriptorSet != nullptr, "Scene descriptor set was not given to the pipeline cache!")
        Device& device = RendererAPI::GetDevice();

        std::vector<DescriptorSetHandle> descriptorSets;
        descriptorSets.push_back(m_SceneDescriptorSet);
        if (material.DescriptorSet) { descriptorSets.push_back(material.DescriptorSet); }

        PipelineStateObjectCreateInfo pipelineStateObjectCreateInfo = {
            .RenderPass = renderPass,
            .VertexShader = mesh.VertexShader,
            .FragmentShader = material.FragmentShader,
            .DescriptorSets = descriptorSets,
            .BufferLayout = mesh.VertexBuffer->GetBufferLayout(),
            .SubpassIndex = subpassIndex
        };

        PipelineStateObjectHandle pipelineStateObject = device.CreatePipelineStateObject(pipelineStateObjectCreateInfo);
        m_PipelineCache[pipelineStateConfiguration] = pipelineStateObject;

        return pipelineStateObject;
    }

}
