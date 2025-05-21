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
        m_Device(desc.Device),
        m_RenderPass(desc.RenderPass),
        m_VertexShader(desc.VertexShader),
        m_FragmentShader(desc.FragmentShader),
        m_WindowWidth(desc.WindowWidth),
        m_WindowHeight(desc.WindowHeight),
        m_DescriptorSet(desc.DescriptorSet)
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
        VkShaderModule vertexShaderModule = (VkShaderModule)m_VertexShader->GetNativeHandle();
        VkShaderModule fragmentShaderModule = (VkShaderModule)m_FragmentShader->GetNativeHandle();

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

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE
        };

        VkViewport viewport = {
            .x = 0.0f,
            .y = 0.0f,
            .width = (float)m_WindowWidth,
            .height = (float)m_WindowHeight,
            .minDepth = 0.0f,
            .maxDepth = 1.0f
        };

        VkRect2D scissor = {
            .offset = {0, 0},
            .extent = {m_WindowWidth, m_WindowHeight}
        };

        VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = 1,
            .pViewports = &viewport,
            .scissorCount = 1,
            .pScissors = &scissor,
        };

        VkPipelineRasterizationStateCreateInfo rasterizer = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = VK_CULL_MODE_NONE,
            .frontFace = VK_FRONT_FACE_CLOCKWISE,
            .lineWidth = 1.0f
        };

        VkPipelineMultisampleStateCreateInfo multisample = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable = VK_FALSE,
            .minSampleShading = 1.0f
        };

        VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {
            .blendEnable = VK_FALSE,
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT,
        };

        VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &colorBlendAttachmentState,
        };

        VkDescriptorSetLayout descriptorSetLayout = (VkDescriptorSetLayout)m_DescriptorSet->GetLayout();

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 1,
            .pSetLayouts = &descriptorSetLayout
        };

        VkResult result = vkCreatePipelineLayout(m_Device, &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout);
        ASSERT(result == VK_SUCCESS, "Failed to create pipeline layout!");

        VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = sizeof(shaderStageCreateInfo) / sizeof(VkPipelineShaderStageCreateInfo),
            .pStages = shaderStageCreateInfo,
            .pVertexInputState = &vertexInputInfo,
            .pInputAssemblyState = &inputAssembly,
            .pViewportState = &viewportStateCreateInfo,
            .pRasterizationState = &rasterizer,
            .pMultisampleState = &multisample,
            .pColorBlendState = &colorBlendStateCreateInfo,
            .layout = m_PipelineLayout,
            .renderPass = m_RenderPass,
            .subpass = 0,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = -1
        };

        result = vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineCreateInfo,
                                                nullptr, &m_Pipeline);
        ASSERT(result == VK_SUCCESS, "Failed to create graphics pipeline!");
    }


    void VulkanPipelineStateObject::DestroyPipelineStateObject()
    {
        vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
        vkDestroyPipeline(m_Device, m_Pipeline, nullptr);
    }

}
