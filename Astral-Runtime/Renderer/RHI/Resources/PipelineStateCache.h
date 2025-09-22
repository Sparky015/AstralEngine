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

    struct GraphicsPipelineStateConfiguration
    {
        RenderPassHandle RenderPass;
        ShaderHandle VertexShader;
        ShaderHandle FragmentShader;
        DescriptorSetLayout ShaderDataLayout;
        VertexBufferLayout VertexBufferLayout;
        bool IsAlphaBlended;
        SampleCount MSAASampleCount;

        bool operator==(const GraphicsPipelineStateConfiguration& other) const;
    };

    struct ComputePipelineStateConfiguration
    {
        ShaderHandle ComputeShader;
        DescriptorSetLayout ShaderDataLayout;

        bool operator==(const ComputePipelineStateConfiguration& other) const;
    };

    class PipelineStateCache
    {
    public:
        void SetDescriptorSetStack(const DescriptorSetHandle& descriptorSet);
        void SetDescriptorSetStack(const std::vector<DescriptorSetHandle>& descriptorSets);

        /**
         * @brief Retrieves a graphics pipeline from the cache with the same parameters given or creates a new graphics pipeline if one does not exist.
         * @param renderPass The render pass of the graphics pipeline
         * @param material A material that contains the fragment shader being used and the descriptor set being used
         * @param mesh A mesh that contains the vertex shader being used and the vertex buffer layout being used
         * @param subpassIndex The index of the subpass the pipeline is being used in
         * @param cullMode
         * @param msaaSampleCount
         * @warning The material's descriptor set will be added on top of the PipelineStateCache's descriptor set stack
         */
        PipelineStateHandle GetGraphicsPipeline(const RenderPassHandle& renderPass, Material& material, Mesh& mesh, uint32 subpassIndex,
                                                CullMode cullMode, SampleCount msaaSampleCount = SampleCount::SAMPLE_1_BIT);

        /**
         * @brief Retrieves a graphics pipeline from the cache with the same parameters given or creates a new graphics pipeline if one does not exist.
         * @param computeShader The compute shader for the pipeline
         * @param descriptorSet The descriptor set being used in addition to the PipelineStateCache's descriptor set stack
         * @warning The given descriptor set argument will be added on top of the PipelineStateCache's descriptor set stack
         */
        PipelineStateHandle GetComputePipeline(ShaderHandle computeShader, DescriptorSetHandle descriptorSet);

    private:

        std::vector<DescriptorSetHandle> m_DescriptorSetStack{};
        std::unordered_map<GraphicsPipelineStateConfiguration, PipelineStateHandle> m_GraphicsPipelineCache;
        std::unordered_map<ComputePipelineStateConfiguration, PipelineStateHandle> m_ComputePipelineCache;
    };

}