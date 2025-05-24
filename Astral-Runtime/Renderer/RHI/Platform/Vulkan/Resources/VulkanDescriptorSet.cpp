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
        m_DescriptorSetLayoutBindings(),
        m_NumberOfBindings(0),
        m_Buffers(),
        m_Textures(),
        m_DescriptorPool(),
        m_DescriptorSet(),
        m_DescriptorSetLayout()
    {
        CreateDescriptorPool();
    }


    VulkanDescriptorSet::~VulkanDescriptorSet()
    {
        DestroyDescriptorSets();
        DestroyDescriptorSetLayout();
        DestroyDescriptorPool();
    }


    void VulkanDescriptorSet::BeginBuildingSet()
    {
        m_NumberOfBindings = 0;
        m_DescriptorSetLayoutBindings.clear();
        while (!m_Buffers.empty()) m_Buffers.pop();
        while (!m_Textures.empty()) m_Textures.pop();
    }


    void VulkanDescriptorSet::AddDescriptorStorageBuffer(BufferHandle bufferHandle, ShaderStage bindStage)
    {
        VkShaderStageFlags stageFlags;
        if (bindStage == ShaderStage::VERTEX)
        {
            stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        }
        else if (bindStage == ShaderStage::FRAGMENT)
        {
            stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        }
        else
        {
            stageFlags = VK_SHADER_STAGE_ALL;
        }

        VkDescriptorSetLayoutBinding storageBufferDescriptorLayoutBinding = {
            .binding = m_NumberOfBindings,
            .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            .descriptorCount = 1,
            .stageFlags = stageFlags,
        };
        m_NumberOfBindings++;

        m_DescriptorSetLayoutBindings.push_back(storageBufferDescriptorLayoutBinding);
        m_Buffers.push(bufferHandle);
    }


    void VulkanDescriptorSet::AddDescriptorUniformBuffer(BufferHandle bufferHandle, ShaderStage bindStage)
    {
        VkShaderStageFlags stageFlags;
        if (bindStage == ShaderStage::VERTEX)
        {
            stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        }
        else if (bindStage == ShaderStage::FRAGMENT)
        {
            stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        }
        else
        {
            stageFlags = VK_SHADER_STAGE_ALL;
        }

        VkDescriptorSetLayoutBinding uniformBufferDescriptorLayoutBinding = {
            .binding = m_NumberOfBindings,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = 1,
            .stageFlags = stageFlags,
        };
        m_NumberOfBindings++;

        m_DescriptorSetLayoutBindings.push_back(uniformBufferDescriptorLayoutBinding);
        m_Buffers.push(bufferHandle);
    }


    void VulkanDescriptorSet::AddDescriptorImageSampler(TextureHandle textureHandle, ShaderStage bindStage)
    {
        VkShaderStageFlags stageFlags;
        if (bindStage == ShaderStage::VERTEX)
        {
            stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        }
        else if (bindStage == ShaderStage::FRAGMENT)
        {
            stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        }
        else
        {
            stageFlags = VK_SHADER_STAGE_ALL;
        }

        VkDescriptorSetLayoutBinding imageSamplerDescriptorLayoutBinding = {
            .binding = m_NumberOfBindings,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = 1,
            .stageFlags = stageFlags,
        };
        m_NumberOfBindings++;

        m_DescriptorSetLayoutBindings.push_back(imageSamplerDescriptorLayoutBinding);
        m_Textures.push(textureHandle);
    }


    void VulkanDescriptorSet::EndBuildingSet()
    {
        CreateDescriptorSetLayout();
        AllocateDescriptorSets();
        UpdateDescriptorSets();
    }


    void VulkanDescriptorSet::CreateDescriptorPool()
    {
        // TODO: Find the exact usages and create the pool at VulkanDescriptorSet::EndBuildingSet
        VkDescriptorPoolSize poolSizes[3] = {
            {
                .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                .descriptorCount = 2
            },
            {
                .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount = 2
            },
            {
                .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount = 2
            }
        };

        VkDescriptorPoolCreateInfo descriptorPoolInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .maxSets = 1,
            .poolSizeCount = 3,
            .pPoolSizes = poolSizes
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
        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .bindingCount = m_NumberOfBindings,
            .pBindings = m_DescriptorSetLayoutBindings.data(),
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
        vkFreeDescriptorSets(m_Device, m_DescriptorPool, 1, &m_DescriptorSet);
    }


    void VulkanDescriptorSet::UpdateDescriptorSets()
    {
        for (uint32 i = 0; i < m_DescriptorSetLayoutBindings.size(); i++)
        {
            const VkDescriptorSetLayoutBinding& descriptorSetLayout = m_DescriptorSetLayoutBindings[i];
            VkDescriptorType descriptorType = descriptorSetLayout.descriptorType;

            if (descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER || descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
            {
                VkDescriptorBufferInfo bufferInfo = {};

                BufferHandle bufferHandle = m_Buffers.front();
                m_Buffers.pop();
                VkBuffer buffer = (VkBuffer)bufferHandle->GetNativeHandle();

                bufferInfo.buffer = buffer;
                bufferInfo.offset = 0;
                bufferInfo.range = bufferHandle->GetUsedSize();


                VkWriteDescriptorSet descriptorSetWrite = {
                    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .pNext = nullptr,
                    .dstSet = m_DescriptorSet,
                    .dstBinding = i,
                    .dstArrayElement = 0,
                    .descriptorCount = 1,
                    .descriptorType = descriptorType,
                    .pBufferInfo = &bufferInfo
                };

                vkUpdateDescriptorSets(m_Device, 1, &descriptorSetWrite, 0, nullptr);
            }
            else if (descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
            {
                VkDescriptorImageInfo imageInfo = {};

                TextureHandle textureHandle = m_Textures.front();
                m_Textures.pop();
                VkImageView imageView = (VkImageView)textureHandle->GetNativeHandle();
                VkSampler sampler = (VkSampler)textureHandle->GetSampler();


                imageInfo.sampler = sampler;
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = imageView;

                VkWriteDescriptorSet descriptorSetWrite = {
                    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                    .pNext = nullptr,
                    .dstSet = m_DescriptorSet,
                    .dstBinding = i,
                    .dstArrayElement = 0,
                    .descriptorCount = 1,
                    .descriptorType = descriptorType,
                    .pImageInfo = &imageInfo
                };

                vkUpdateDescriptorSets(m_Device, 1, &descriptorSetWrite, 0, nullptr);
            }
        }


    }

}
