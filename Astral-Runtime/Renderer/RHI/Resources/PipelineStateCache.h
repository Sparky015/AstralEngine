/**
* @file PipelineStateCache.h
* @author Andrew Fagan
* @date 6/27/2025
*/


#pragma once

#include "PipelineState.h"
#include "Renderer/RHI/Resources/Renderpass.h"
#include "Renderer/Common/Mesh.h"
#include "Renderer/Common/Material.h"
#include "Core/Utility/Hashing/Hashes.h"

namespace Astral {

    struct PipelineStateConfiguration
    {
        RenderPassHandle RenderPass;
        ShaderHandle VertexShader;
        ShaderHandle FragmentShader;
        DescriptorSetLayout ShaderDataLayout;
        VertexBufferLayout VertexBufferLayout;
        bool IsAlphaBlended;

        bool operator==(const PipelineStateConfiguration& other) const;
    };

    class PipelineStateCache
    {
    public:
        void SetDescriptorSetStack(const DescriptorSetHandle& descriptorSet);
        void SetDescriptorSetStack(const std::vector<DescriptorSetHandle>& descriptorSets);

        PipelineStateHandle GetPipeline(RenderPassHandle renderPass, Material& material, Mesh& mesh, uint32 subpassIndex);

    private:

        std::vector<DescriptorSetHandle> m_DescriptorSetStack{};
        std::unordered_map<PipelineStateConfiguration, PipelineStateHandle> m_PipelineCache;
    };

}