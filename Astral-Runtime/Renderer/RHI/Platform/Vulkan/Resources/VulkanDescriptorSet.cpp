/**
* @file VulkanDescriptorSet.cpp
* @author Andrew Fagan
* @date 5/18/2025
*/

#include "VulkanDescriptorSet.h"

#include "Debug/Utilities/Asserts.h"

namespace Astral {

    VulkanDescriptorSet::VulkanDescriptorSet(const VulkanDescriptorSetDesc& desc) :
        m_Device(desc.Device),
        m_VertexBuffer(desc.VertexBuffer)
    {
        CreateDescriptorPool();
        CreateDescriptorSetLayout();
        AllocateDescriptorSets();
        UpdateDescriptorSets();
    }


    VulkanDescriptorSet::~VulkanDescriptorSet()
    {
        DestroyDescriptorSets();
        DestroyDescriptorSetLayout();
        DestroyDescriptorPool();
    }


    void VulkanDescriptorSet::CreateDescriptorPool()
    {
        VkDescriptorPoolCreateInfo descriptorPoolInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .maxSets = 1,
            .poolSizeCount = 0,
            .pPoolSizes = nullptr
        };

        VkResult result = vkCreateDescriptorPool(m_Device, &descriptorPoolInfo, nullptr, &m_DescriptorPool);
        ASSERT(result == VK_SUCCESS, "Failed to create descriptor pool!");
    }


    void VulkanDescriptorSet::DestroyDescriptorPool()
    {
        vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
    }


    void VulkanDescriptorSet::CreateDescriptorSetLayout()
    {
        VkDescriptorSetLayoutBinding vertexShaderLayoutBinding = {
            .binding = 0,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = 1,
            .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        };

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .bindingCount = 1,
            .pBindings = &vertexShaderLayoutBinding
        };

        VkResult result = vkCreateDescriptorSetLayout(m_Device, &descriptorSetLayoutCreateInfo, nullptr, &m_DescriptorSetLayout);
        ASSERT(result == VK_SUCCESS, "Failed to create descriptor set layout!");
    }


    void VulkanDescriptorSet::DestroyDescriptorSetLayout()
    {
        vkDestroyDescriptorSetLayout(m_Device, m_DescriptorSetLayout, nullptr);
    }


    void VulkanDescriptorSet::AllocateDescriptorSets()
    {
        VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = nullptr,
            .descriptorPool = m_DescriptorPool,
            .descriptorSetCount = 1,
            .pSetLayouts = &m_DescriptorSetLayout
        };

        VkResult result = vkAllocateDescriptorSets(m_Device, &descriptorSetAllocateInfo, &m_DescriptorSet);
        ASSERT(result == VK_SUCCESS, "Failed to allocate descriptor sets!");
    }


    void VulkanDescriptorSet::DestroyDescriptorSets()
    {

    }


    void VulkanDescriptorSet::UpdateDescriptorSets()
    {
        VkBuffer vertexBuffer = (VkBuffer)m_VertexBuffer->GetNativeHande();

        VkDescriptorBufferInfo bufferInfo = {
            .buffer = vertexBuffer,
            .offset = 0,
            .range = m_VertexBuffer->GetSize(),
        };

        VkWriteDescriptorSet descriptorSetWrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = m_DescriptorSet,
            .dstBinding = 0,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .pBufferInfo = &bufferInfo
        };

        vkUpdateDescriptorSets(m_Device, 1, &descriptorSetWrite, 0, nullptr);
    }

}
