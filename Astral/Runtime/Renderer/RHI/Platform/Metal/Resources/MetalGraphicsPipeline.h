/**
* @file MetalGraphicsPipeline.h
* @author Andrew Fagan
* @date 5/24/26
*/

#pragma once

#include "Renderer/RHI/Resources/PipelineState.h"

#include "Metal/MTLDevice.hpp"

namespace Astral {

    /**
     * @brief Information to create a metal graphics pipeline state
     */
    struct MetalGraphicsPipelineStateDesc
    {
        MTL::Device* Device;
    };

    /**
     * @brief A wrapper around a Metal graphics pipeline with extra convenience functions
     */
    class MetalGraphicsPipelineState : public PipelineState // TODO
    {
    public:

        MetalGraphicsPipelineState(const MetalGraphicsPipelineStateDesc& graphicsPipelineStateDesc); // TODO
        ~MetalGraphicsPipelineState() override; // TODO

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