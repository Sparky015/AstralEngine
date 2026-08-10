/**
* @file VulkanPipelineState.h
* @author Andrew Fagan
* @date 5/17/2025
*/


#pragma once

#include "Renderer/RHI/Resources/PipelineState.h"
#include "Renderer/RHI/Resources/Shader.h"
#include "Renderer/RHI/Resources/VertexBufferLayout.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    /**
     * @brief Information to create a vulkan graphics pipeline state
     */
    struct VulkanGraphicsPipelineStateDesc
    {
        VkDevice Device;
        RenderPassHandle RenderPass;
        ShaderHandle VertexShader;
        ShaderHandle FragmentShader;
        int32 WindowWidth;
        int32 WindowHeight;
        std::vector<DescriptorSetHandle> DescriptorSets;
        VertexBufferLayout VertexBufferLayout;
        uint32 SubpassIndex;
        bool IsAlphaBlended;
        SampleCount MSAASamples;
        CullMode CullMode;
    };

    /**
     * @brief A wrapper around a Vulkan graphics pipeline with extra convenience functions
     */
    class VulkanPipelineState : public PipelineState
    {
    public:
        explicit VulkanPipelineState(const VulkanGraphicsPipelineStateDesc& desc);
        ~VulkanPipelineState() override;

        /**
         * @brief Gets the pipeline type of this pipeline
         * @return The pipeline type of this pipeline
         */
        PipelineType GetPipelineType() override { return PipelineType::GRAPHICS; }

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
        const std::vector<DescriptorSetLayout>& GetDescriptorSetLayout() const override { return m_DescriptorSetLayout; }

        /**
         * @brief Gets the native pipeline layout
         * @return The native pipeline layout
         */
        void* GetPipelineLayout() override { return m_PipelineLayout; }

        /**
         * @brief Gets the native pipeline object
         * @return The native pipeline object
         */
        void* GetNativeHandle() override { return m_Pipeline; }

    private:

        /**
         * @brief Populates the pipeline shader stage create infos
         */
        void SetGraphicsShaderStages();

        /**
         * @brief Populates the pipeline vertex input State create info
         */
        void SetVertexInputState();

        /**
         * @brief Populates the pipeline input assembly create info
         */
        void SetInputAssemblyState();

        /**
         * @brief Populates the pipeline viewport state create info
         */
        void SetViewportState();

        /**
         * @brief Populates the pipeline rasterizer state create info
         */
        void SetRasterizerState();

        /**
         * @brief Populates the pipeline multisample state create info
         */
        void SetMultisampleState();

        /**
         * @brief Populates the pipeline depth stencil state create info
         */
        void SetDepthStencilState();

        /**
         * @brief Populates the pipeline color blend state create info
         */
        void SetColorBlendState();

        /**
         * @brief Populates the pipeline dynamic state create info
         */
        void SetDynamicState();

        /**
         * @brief Populates the pipeline rendering create info
         */
        void SetRenderingCreateInfo();

        /**
         * @brief Creates the pipeline layout based on the descriptor set layouts
         */
        void CreateGraphicsPipelineLayout();

        /**
         * @brief Destroys the pipeline layout
         */
        void DestroyPipelineLayout();

        /**
         * @brief Creates the pipeline state object based on the previously set create infos
         */
        void CreateGraphicsPipelineStateObject();

        /**
         * @brief Destroys the pipeline state object
         */
        void DestroyPipelineState();

        VkDevice m_Device;
        VulkanGraphicsPipelineStateDesc m_GraphicsDescription;

        struct PipelineCreateInfos
        {
            VkPipelineShaderStageCreateInfo ShaderStates[2];
            VkVertexInputBindingDescription VertexBindingDescription;
            std::vector<VkVertexInputAttributeDescription> VertexAttributeDescriptions;
            VkPipelineVertexInputStateCreateInfo VertexInputState;
            VkPipelineInputAssemblyStateCreateInfo InputAssemblyState;
            VkViewport Viewport;
            VkRect2D Scissor;
            VkPipelineViewportStateCreateInfo ViewportState;
            VkPipelineRasterizationStateCreateInfo RasterizationState;
            VkPipelineMultisampleStateCreateInfo MultisampleState;
            VkPipelineDepthStencilStateCreateInfo DepthStencilState;
            std::vector<VkPipelineColorBlendAttachmentState> ColorBlendAttachmentStates;
            VkPipelineColorBlendStateCreateInfo ColorBlendState;
            VkPipelineLayoutCreateInfo PipelineLayout;
            VkDynamicState DynamicStateSpecs[2];
            VkPipelineDynamicStateCreateInfo DynamicState;
            std::vector<VkFormat> RenderingCreateInfoColorAttachmentFormats{};
            VkPipelineRenderingCreateInfo RenderingCreateInfo;
        };

        PipelineCreateInfos m_PipelineCreateInfos;

        VkPipeline m_Pipeline;
        VkPipelineLayout m_PipelineLayout;
        std::vector<DescriptorSetLayout> m_DescriptorSetLayout{};
        VkPushConstantRange m_PushConstantRange;

        UVec2 m_ViewportDimensions;

        ShaderHandle m_CompiledVertexShader;
        ShaderHandle m_CompiledFragmentShader;
    };

}
