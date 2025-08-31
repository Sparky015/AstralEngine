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
    };

    struct VulkanComputePipelineStateDesc
    {
        VkDevice Device;
        ShaderHandle ComputeShader;
        std::vector<DescriptorSetHandle> DescriptorSets;
    };

    class VulkanPipelineState : public PipelineState
    {
    public:
        explicit VulkanPipelineState(const VulkanGraphicsPipelineStateDesc& desc);
        explicit VulkanPipelineState(const VulkanComputePipelineStateDesc& desc);
        ~VulkanPipelineState() override;

        void Bind(CommandBufferHandle commandBufferHandle) override;
        void BindDescriptorSet(CommandBufferHandle commandBufferHandle, DescriptorSetHandle descriptorSetHandle, uint32 binding) override;

        void SetViewportAndScissor(CommandBufferHandle commandBufferHandle, UVec2 dimensions) override;

        void* GetPipelineLayout() override {return m_PipelineLayout; }
        void* GetHandleHandle() override { return m_Pipeline; }

    private:

        void SetGraphicsShaderStages();
        void SetVertexInputState();
        void SetInputAssemblyState();
        void SetViewportState();
        void SetRasterizerState();
        void SetMultisampleState();
        void SetDepthStencilState();
        void SetColorBlendState();
        void SetDynamicState();

        void SetComputeShaderStage();

        void CreateGraphicsPipelineLayout();
        void CreateComputePipelineLayout();
        void DestroyPipelineLayout();

        void CreateGraphicsPipelineStateObject();
        void CreateComputePipelineStateObject();
        void DestroyPipelineState();


        VkDevice m_Device;
        VulkanGraphicsPipelineStateDesc m_GraphicsDescription;
        VulkanComputePipelineStateDesc m_ComputeDescription;

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
        };

        PipelineCreateInfos m_PipelineCreateInfos;

        VkPipeline m_Pipeline;
        VkPipelineLayout m_PipelineLayout;
        VkPushConstantRange m_PushConstantRange;

        UVec2 m_ViewportDimensions;
    };

}
