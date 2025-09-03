/**
* @file VulkanComputePipelineState.cpp
* @author Andrew Fagan
* @date 5/17/2025
*/

#include "VulkanComputePipelineState.h"

#include "VulkanVertexBuffer.h"
#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Error.h"

namespace Astral {


    VulkanComputePipelineState::VulkanComputePipelineState(const VulkanComputePipelineStateDesc& desc) :
        m_Device(desc.Device),
        m_ComputeDescription(desc)
    {
        CreateComputePipelineStateObject();
    }


    VulkanComputePipelineState::~VulkanComputePipelineState()
    {
        DestroyPipelineLayout();
        DestroyPipelineState();
    }


    void VulkanComputePipelineState::CreateComputePipelineStateObject()
    {
        SetComputeShaderStage();
        CreateComputePipelineLayout();

        VkComputePipelineCreateInfo pipelineCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = m_PipelineCreateInfos.ShaderState,
            .layout = m_PipelineLayout,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = 0,
        };

        vkCreateComputePipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_Pipeline);
    }


    void VulkanComputePipelineState::DestroyPipelineState()
    {
        vkDestroyPipeline(m_Device, m_Pipeline, nullptr);
    }


    void VulkanComputePipelineState::SetComputeShaderStage()
    {
        ASSERT(m_ComputeDescription.ComputeShader, "Compute shader can't be null!")

        VkShaderModule computeShaderModule = (VkShaderModule)m_ComputeDescription.ComputeShader->GetNativeHandle();

        VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_COMPUTE_BIT,
            .module = computeShaderModule,
            .pName = "main",
        };

        m_PipelineCreateInfos.ShaderState = shaderStageCreateInfo;
    }


    void VulkanComputePipelineState::CreateComputePipelineLayout()
    {
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        descriptorSetLayouts.reserve(m_ComputeDescription.DescriptorSets.size());
        m_DescriptorSetLayout.reserve(m_ComputeDescription.DescriptorSets.size());
        for (DescriptorSetHandle descriptorSet : m_ComputeDescription.DescriptorSets)
        {
            descriptorSetLayouts.push_back((VkDescriptorSetLayout)descriptorSet->GetNativeLayout());
            m_DescriptorSetLayout.push_back(descriptorSet->GetDescriptorSetLayout());
        }

        m_PushConstantRange.stageFlags = VK_SHADER_STAGE_ALL;
        m_PushConstantRange.offset = 0;
        m_PushConstantRange.size = MaxPushConstantRange;

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .setLayoutCount = (uint32)descriptorSetLayouts.size(),
            .pSetLayouts = descriptorSetLayouts.data(),
            .pushConstantRangeCount = 1,
            .pPushConstantRanges = &m_PushConstantRange,
        };

        VkResult result = vkCreatePipelineLayout(m_Device, &pipelineLayoutCreateInfo, nullptr, &m_PipelineLayout);
        ASSERT(result == VK_SUCCESS, "Failed to create pipeline layout!");
    }


    void VulkanComputePipelineState::DestroyPipelineLayout()
    {
        vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
    }

}
