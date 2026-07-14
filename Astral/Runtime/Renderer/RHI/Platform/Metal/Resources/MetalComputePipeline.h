/**
* @file MetalComputePipeline.h
* @author Andrew Fagan
* @date 5/24/26
*/

#pragma once

#include "Renderer/RHI/Resources/PipelineState.h"

#include "Metal/MTLDevice.hpp"

namespace Astral {

    /**
     * @brief Information to create a metal compute pipeline state
     */
    struct MetalComputePipelineStateDesc
    {
        MTL::Device* Device;
        ShaderHandle ComputeShader;
        std::vector<DescriptorSetHandle> DescriptorSets;
    };

    /**
     * @brief A wrapper around a Metal compute pipeline with extra convenience functions
     */
    class MetalComputePipelineState : public PipelineState
    {
    public:

        MetalComputePipelineState(const MetalComputePipelineStateDesc& computePipelineStateDesc);
        ~MetalComputePipelineState() override;

        /**
         * @brief Gets the pipeline type of this pipeline
         * @return The pipeline type of this pipeline
         */
        PipelineType GetPipelineType() override;

        /**
         * @brief Gets the vertex shader compiled by this pipeline
         * @return The vertex shader compiled by this pipeline or nullptr if the pipeline did not use a vertex shader
         */
        ShaderHandle GetCompiledVertexShader() override;

        /**
         * @brief Gets the fragment shader compiled by this pipeline
         * @return The fragment shader compiled by this pipeline or nullptr if the pipeline did not use a fragment shader
         */
        ShaderHandle GetCompiledFragmentShader() override;

        /**
         * @brief Gets the compute shader compiled by this pipeline
         * @return The compute shader compiled by this pipeline or nullptr if the pipeline did not use a compute shader
         */
        ShaderHandle GetCompiledComputeShader() override;

        /**
         * @brief Gets the descriptor set layouts of the pipeline
         * @return The descriptor set layouts of the pipeline
         */
        const std::vector<DescriptorSetLayout>& GetDescriptorSetLayout() const override;

        /**
         * @brief Gets the native pipeline layout
         * @return The native pipeline layout
         */
        void* GetPipelineLayout() override;

        /**
         * @brief Gets the native pipeline object
         * @return The native pipeline object
         */
        void* GetNativeHandle() override;

    private:

        /**
         * @brief Creates the Metal compute pipeline state
         * @param computePipelineStateDesc The description of the pipeline to create
         */
        void CreatePipelineState(const MetalComputePipelineStateDesc& computePipelineStateDesc);

        /**
         * @brief Releases the compute pipeline state
         */
        void ReleasePipelineState();

        /**
         * @brief Creates the descriptor set layout of the pipeline
         */
        void CreateDescriptorSetLayout(const MetalComputePipelineStateDesc& computePipelineStateDesc);


        MTL::Device* m_Device;
        MTL::ComputePipelineState* m_Pipeline;

        std::vector<DescriptorSetLayout> m_DescriptorSetLayout;
        ShaderHandle m_CompiledComputeShader;
    };

}