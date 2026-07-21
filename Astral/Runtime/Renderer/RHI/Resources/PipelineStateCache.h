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
#include "Core/Hashing/Hashes.h"

namespace Astral {

    struct GraphicsPipelineStateConfiguration
    {
        RenderPassHandle RenderPass;
        ShaderHandle VertexShader;
        ShaderHandle FragmentShader;
        DescriptorSetLayout ShaderDataLayout;
        VertexBufferLayout VertexBufferLayout;
        bool IsAlphaBlended;
        CullMode CullMode;
        SampleCount MSAASampleCount = SampleCount::SAMPLE_1_BIT;

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

        /**
         * @brief Retrieves a graphics pipeline from the cache with the same parameters given or creates a new graphics pipeline if one does not exist.
         * @param graphicsPipelineStateConfiguration The configuration of the pipeline to get
         * @param descriptorSetStack A stack of descriptor sets that form the desired pipeline descriptor set layout
         */
        PipelineStateHandle GetGraphicsPipeline(const GraphicsPipelineStateConfiguration& graphicsPipelineStateConfiguration, const std::vector<DescriptorSetHandle>& descriptorSetStack);

        /**
         * @brief Retrieves a graphics pipeline from the cache with the same parameters given or creates a new graphics pipeline if one does not exist.
         * @param computeShader The compute shader for the pipeline
         * @param descriptorSet The descriptor set being used in addition to the PipelineStateCache's descriptor set stack
         * @param descriptorSetStack
         * @warning The given descriptor set argument will be added on top of the PipelineStateCache's descriptor set stack
         */
        PipelineStateHandle GetComputePipeline(ShaderHandle computeShader, DescriptorSetHandle descriptorSet, const std::vector<DescriptorSetHandle>& descriptorSetStack);

    private:

        GraphicsPipelineStateConfiguration m_GraphicsPipelineStateConfigurationCache{}; // Cached memory for checking if pipeline already exists
        std::unordered_map<GraphicsPipelineStateConfiguration, PipelineStateHandle> m_GraphicsPipelineCache;
        std::unordered_map<ComputePipelineStateConfiguration, PipelineStateHandle> m_ComputePipelineCache;
        std::shared_mutex m_PipelineCacheMutex;
    };

}