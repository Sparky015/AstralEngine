/**
* @file VulkanPipelineStateObject.h
* @author Andrew Fagan
* @date 5/17/2025
*/


#pragma once

#include "Renderer/RHI/Resources/PipelineStateObject.h"

#include <vulkan/vulkan_core.h>

#include "Renderer/RHI/Resources/Shader.h"
#include "Renderer/RHI/Resources/VertexBufferLayout.h"

namespace Astral {

    struct VulkanPipelineStateObjectDesc
    {
        VkDevice Device;
        VkRenderPass RenderPass;
        ShaderHandle VertexShader;
        ShaderHandle FragmentShader;
        int32 WindowWidth;
        int32 WindowHeight;
        std::vector<DescriptorSetHandle> DescriptorSets;
        VertexBufferLayout VertexBufferLayout;
    };

    class VulkanPipelineStateObject : public PipelineStateObject
    {
    public:
        explicit VulkanPipelineStateObject(const VulkanPipelineStateObjectDesc& desc);
        ~VulkanPipelineStateObject() override;

        void Bind(CommandBufferHandle commandBufferHandle) override;
        void BindDescriptorSet(CommandBufferHandle commandBufferHandle, DescriptorSetHandle descriptorSetHandle, uint32 binding) override;

        void SetViewportAndScissor(CommandBufferHandle commandBufferHandle, UVec2 dimensions) override;

        void* GetPipelineLayout() override {return m_PipelineLayout; }
        void* GetHandleHandle() override { return m_Pipeline; }

    private:

        void CreatePipelineStateObject();
        void DestroyPipelineStateObject();

        void SetShaderStages();
        void SetVertexInputState();
        void SetInputAssemblyState();
        void SetViewportState();
        void SetRasterizerState();
        void SetMultisampleState();
        void SetDepthStencilState();
        void SetColorBlendState();
        void SetDynamicState();

        void CreatePipelineLayout();
        void DestroyPipelineLayout();

        VulkanPipelineStateObjectDesc m_Description;

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
            VkPipelineColorBlendAttachmentState ColorBlendAttachmentState;
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
