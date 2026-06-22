/**
* @file MetalGraphicsPipeline.h
* @author Andrew Fagan
* @date 5/24/26
*/

#pragma once

#include "Renderer/RHI/Resources/PipelineState.h"

#include "Metal/MTLDevice.hpp"
#include "Metal/MTL4RenderPipeline.hpp"

namespace Astral {

    /**
     * @brief Information to create a metal graphics pipeline state
     */
    struct MetalGraphicsPipelineStateDesc
    {
        MTL::Device* Device;
        RenderPassHandle RenderPass;
        ShaderHandle VertexShader;
        ShaderHandle FragmentShader;
        std::vector<DescriptorSetHandle> DescriptorSets;
        const VertexBufferLayout& BufferLayout;
        bool IsAlphaBlended;
        SampleCount MSAASamples;
        CullMode CullMode;
    };

    /**
     * @brief A wrapper around a Metal graphics pipeline with extra convenience functions
     */
    class MetalGraphicsPipelineState : public PipelineState
    {
    public:

        MetalGraphicsPipelineState(const MetalGraphicsPipelineStateDesc& graphicsPipelineStateDesc);
        ~MetalGraphicsPipelineState() override;

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


        /**
         * @brief Creates the Metal compute pipeline state
         * @param graphicsPipelineStateDesc The description of the pipeline to create
         */
        void CreatePipelineState(const MetalGraphicsPipelineStateDesc& graphicsPipelineStateDesc);

        /**
         * @brief Releases the compute pipeline state
         */
        void ReleasePipelineState();

        void PopulatePipelineColorAttachmentDescriptor(const MetalGraphicsPipelineStateDesc& graphicsPipelineStateDesc);

        void PopulatePipelineVertexDescriptor(const MetalGraphicsPipelineStateDesc& graphicsPipelineStateDesc);

        void PopulateFunctionDescriptors(const MetalGraphicsPipelineStateDesc& graphicsPipelineStateDesc);

        /**
         * @brief Creates the descriptor set layout of the pipeline
         */
        void CreateDescriptorSetLayout(const MetalGraphicsPipelineStateDesc& graphicsPipelineStateDesc);


        MTL::Device* m_Device;
        MTL4::RenderPipelineDescriptor* m_PipelineDescriptor;
        MTL::RenderPipelineState* m_Pipeline;

        std::vector<DescriptorSetLayout> m_DescriptorSetLayout;

    };

}