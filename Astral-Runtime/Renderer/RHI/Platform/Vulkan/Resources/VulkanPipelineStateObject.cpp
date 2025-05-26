/**
* @file VulkanPipelineStateObject.cpp
* @author Andrew Fagan
* @date 5/17/2025
*/

#include "VulkanPipelineStateObject.h"

#include "VulkanVertexBuffer.h"
#include "Debug/Utilities/Asserts.h"

namespace Astral {

    VulkanPipelineStateObject::VulkanPipelineStateObject(const VulkanPipelineStateObjectDesc& desc) :
        m_Description(desc)
    {
        CreatePipelineStateObject();
    }


    VulkanPipelineStateObject::~VulkanPipelineStateObject()
    {
        DestroyPipelineStateObject();
    }


    void VulkanPipelineStateObject::Bind(CommandBufferHandle commandBufferHandle)
    {
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);
    }


    void VulkanPipelineStateObject::BindDescriptorSet(CommandBufferHandle commandBufferHandle,
        DescriptorSetHandle descriptorSetHandle)
    {
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        VkDescriptorSet descriptorSet = (VkDescriptorSet)descriptorSetHandle->GetNativeHandle();
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout,
                    0, 1, &descriptorSet, 0, nullptr);
    }


    void VulkanPipelineStateObject::CreatePipelineStateObject()
    {
        SetShaderStages();
        SetVertexInputState();
        SetInputAssemblyState();
        SetViewportState();
        SetRasterizerState();
        SetMultisampleState();
        SetColorBlendState();
        CreatePipelineLayoutState();


        VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = sizeof(m_PipelineCreateInfos.ShaderStates) / sizeof(VkPipelineShaderStageCreateInfo),
            .pStages = m_PipelineCreateInfos.ShaderStates,
            .pVertexInputState = &m_PipelineCreateInfos.VertexInputState,
            .pInputAssemblyState = &m_PipelineCreateInfos.InputAssemblyState,
            .pViewportState = &m_PipelineCreateInfos.ViewportState,
            .pRasterizationState = &m_PipelineCreateInfos.RasterizationState,
            .pMultisampleState = &m_PipelineCreateInfos.MultisampleState,
            .pColorBlendState = &m_PipelineCreateInfos.ColorBlendState,
            .layout = m_PipelineLayout,
            .renderPass = m_Description.RenderPass,
            .subpass = 0,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = -1
        };

        VkResult result = vkCreateGraphicsPipelines(m_Description.Device, VK_NULL_HANDLE, 1, &pipelineCreateInfo,
                                                nullptr, &m_Pipeline);
        ASSERT(result == VK_SUCCESS, "Failed to create graphics pipeline!");
    }


    void VulkanPipelineStateObject::DestroyPipelineStateObject()
    {
        vkDestroyPipelineLayout(m_Description.Device, m_PipelineLayout, nullptr);
        vkDestroyPipeline(m_Description.Device, m_Pipeline, nullptr);
    }


    void VulkanPipelineStateObject::SetShaderStages()
    {
        VkShaderModule vertexShaderModule = (VkShaderModule)m_Description.VertexShader->GetNativeHandle();
        VkShaderModule fragmentShaderModule = (VkShaderModule)m_Description.FragmentShader->GetNativeHandle();

        VkPipelineShaderStageCreateInfo shaderStageCreateInfo[2] = {
         {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage = VK_SHADER_STAGE_VERTEX_BIT,
                .module = vertexShaderModule,
                .pName = "main",
            },
         {
             .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
             .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
             .module = fragmentShaderModule,
             .pName = "main",
            }
        };

        m_PipelineCreateInfos.ShaderStates[0] = shaderStageCreateInfo[0];
        m_PipelineCreateInfos.ShaderStates[1] = shaderStageCreateInfo[1];
    }


    void VulkanPipelineStateObject::SetVertexInputState()
    {
        // TODO: Change this to a more robust method to track binding and attribute descriptions (probably through the existing BufferLayout class)
        auto bindingDescription = VulkanVertexBuffer::GetBindingDescriptions();
        auto attributeDescriptions = VulkanVertexBuffer::GetAttributeDescriptions();
        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescription.size()),
            .pVertexBindingDescriptions = bindingDescription.data(),
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
            .pVertexAttributeDescriptions = attributeDescriptions.data(),
        };

        m_PipelineCreateInfos.VertexInputState = vertexInputInfo;
    }


    void VulkanPipelineStateObject::SetInputAssemblyState()
    {
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE
        };

        m_PipelineCreateInfos.InputAssemblyState = inputAssembly;
    }


    void VulkanPipelineStateObject::SetViewportState()
    {
        VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float)m_Description.WindowWidth,
            .height = (float)m_Description.WindowHeight,
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };

        VkRect2D scissor = {
            .offset = {0, 0},
            .extent = {(uint32)m_Description.WindowWidth, (uint32)m_Description.WindowHeight}
        };

        m_PipelineCreateInfos.Viewport = viewport;
        m_PipelineCreateInfos.Scissor = scissor;

        VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = 1,
            .pViewports = &m_PipelineCreateInfos.Viewport,
            .scissorCount = 1,
            .pScissors = &m_PipelineCreateInfos.Scissor,
        };

        m_PipelineCreateInfos.ViewportState = viewportStateCreateInfo;
    }


    void VulkanPipelineStateObject::SetRasterizerState()
    {
        VkPipelineRasterizationStateCreateInfo rasterizer = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = VK_CULL_MODE_NONE,
            .frontFace = VK_FRONT_FACE_CLOCKWISE,
            .lineWidth = 1.0f
        };

        m_PipelineCreateInfos.RasterizationState = rasterizer;
    }


    void VulkanPipelineStateObject::SetMultisampleState()
    {
        VkPipelineMultisampleStateCreateInfo multisample = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable = VK_FALSE,
            .minSampleShading = 1.0f
        };

        m_PipelineCreateInfos.MultisampleState = multisample;
    }


    void VulkanPipelineStateObject::SetColorBlendState()
    {
        VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
            .blendEnable = VK_FALSE,
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT,
        };

        m_PipelineCreateInfos.ColorBlendAttachmentState = colorBlendAttachmentState;

        VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &m_PipelineCreateInfos.ColorBlendAttachmentState,
        };

        m_PipelineCreateInfos.ColorBlendState = colorBlendStateCreateInfo;
    }


    void VulkanPipelineStateObject::CreatePipelineLayoutState()
    {

        VkDescriptorSetLayout descriptorSetLayout = (VkDescriptorSetLayout)m_Description.DescriptorSet->GetLayout();

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 1,
            .pSetLayouts = &descriptorSetLayout
        };

        VkResult result = vkCreatePipelineLayout(m_Description.Device, &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout);
        ASSERT(result == VK_SUCCESS, "Failed to create pipeline layout!");
    }

}
