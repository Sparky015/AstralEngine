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
    };

    /**
     * @brief A wrapper around a Metal compute pipeline with extra convenience functions
     */
    class MetalComputePipelineState : public PipelineState // TODO
    {
    public:

        MetalComputePipelineState(const MetalComputePipelineStateDesc& computePipelineStateDesc); // TODO
        ~MetalComputePipelineState() override; // TODO

        /**
         * @brief Gets the pipeline type of this pipeline
         * @return The pipeline type of this pipeline
         */
        PipelineType GetPipelineType() override;

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


    };

}