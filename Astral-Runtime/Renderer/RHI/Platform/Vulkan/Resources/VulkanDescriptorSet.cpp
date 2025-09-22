/**
* @file VulkanDescriptorSet.cpp
* @author Andrew Fagan
* @date 5/18/2025
*/

#include "VulkanDescriptorSet.h"

#include "Debug/Utilities/Asserts.h"
#include "Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.h"

namespace Astral {

    VulkanDescriptorSet::VulkanDescriptorSet(const VulkanDescriptorSetDesc& desc) :
        m_Device(desc.Device),
        m_DescriptorSetLayoutBindings(),
        m_NumberOfBindings(0),
        m_Buffers(),
        m_Textures(),
        m_DescriptorPool(VK_NULL_HANDLE),
        m_DescriptorSet(VK_NULL_HANDLE),
        m_VkDescriptorSetLayout(VK_NULL_HANDLE)
    {
    }


    VulkanDescriptorSet::~VulkanDescriptorSet()
    {
        Invalidate();
    }


    void VulkanDescriptorSet::BeginBuildingSet()
    {
        Invalidate();
    }


    void VulkanDescriptorSet::AddDescriptorStorageBuffer(BufferHandle bufferHandle, ShaderStage bindStage)
    {
        VkShaderStageFlags stageFlags = GetVkShaderStageFromShaderStage(bindStage);

        VkDescriptorSetLayoutBinding storageBufferDescriptorLayoutBinding = {
            .binding = m_NumberOfBindings,
            .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            .descriptorCount = 1,
            .stageFlags = stageFlags,
        };
        m_NumberOfBindings++;

        m_DescriptorSetLayoutBindings.push_back(storageBufferDescriptorLayoutBinding);
        m_Buffers.push_back(bufferHandle);
        m_DescriptorSetLayout.Descriptors.push_back(Descriptor::STORAGE_BUFFER);
    }


    void VulkanDescriptorSet::AddDescriptorUniformBuffer(BufferHandle bufferHandle, ShaderStage bindStage)
    {
        VkShaderStageFlags stageFlags = GetVkShaderStageFromShaderStage(bindStage);

        VkDescriptorSetLayoutBinding uniformBufferDescriptorLayoutBinding = {
            .binding = m_NumberOfBindings,
            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = 1,
            .stageFlags = stageFlags,
        };
        m_NumberOfBindings++;

        m_DescriptorSetLayoutBindings.push_back(uniformBufferDescriptorLayoutBinding);
        m_Buffers.push_back(bufferHandle);
        m_DescriptorSetLayout.Descriptors.push_back(Descriptor::UNIFORM_BUFFER);
    }


    void VulkanDescriptorSet::AddDescriptorImageSampler(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout)
    {
        ASSERT(textureHandle != nullptr, "A null texture can not be added to a descriptor set!")

        VkShaderStageFlags stageFlags = GetVkShaderStageFromShaderStage(bindStage);

        VkDescriptorSetLayoutBinding imageSamplerDescriptorLayoutBinding = {
            .binding = m_NumberOfBindings,
            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = 1,
            .stageFlags = stageFlags,
        };
        m_NumberOfBindings++;

        m_DescriptorSetLayoutBindings.push_back(imageSamplerDescriptorLayoutBinding);
        m_Textures.push_back(textureHandle);
        m_ImageDescriptorLayouts.push_back(imageLayout);
        m_DescriptorSetLayout.Descriptors.push_back(Descriptor::IMAGE_SAMPLER);
    }


    void VulkanDescriptorSet::AddDescriptorStorageImage(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout)
    {
        ASSERT(textureHandle != nullptr, "A null texture can not be added to a descriptor set!")

        VkShaderStageFlags stageFlags = GetVkShaderStageFromShaderStage(bindStage);

        VkDescriptorSetLayoutBinding storageImageDescriptorLayoutBinding = {
            .binding = m_NumberOfBindings,
            .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
            .descriptorCount = 1,
            .stageFlags = stageFlags,
        };
        m_NumberOfBindings++;

        m_DescriptorSetLayoutBindings.push_back(storageImageDescriptorLayoutBinding);
        m_Textures.push_back(textureHandle);
        m_ImageDescriptorLayouts.push_back(imageLayout);
        m_DescriptorSetLayout.Descriptors.push_back(Descriptor::STORAGE_IMAGE);
    }


    void VulkanDescriptorSet::AddDescriptorSubpassInputAttachment(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout)
    {
        ASSERT(textureHandle != nullptr, "A null texture can not be added to a descriptor set!")

        VkShaderStageFlags stageFlags = GetVkShaderStageFromShaderStage(bindStage);

        VkDescriptorSetLayoutBinding inputAttachmentDescriptorLayoutBinding = {
            .binding = m_NumberOfBindings,
            .descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
            .descriptorCount = 1,
            .stageFlags = stageFlags,
        };
        m_NumberOfBindings++;

        m_DescriptorSetLayoutBindings.push_back(inputAttachmentDescriptorLayoutBinding);
        m_Textures.push_back(textureHandle);
        m_ImageDescriptorLayouts.push_back(imageLayout);
        m_DescriptorSetLayout.Descriptors.push_back(Descriptor::SUBPASS_INPUT_ATTACHMENT);
    }


    void VulkanDescriptorSet::EndBuildingSet()
    {
        CreateDescriptorPool();
        CreateDescriptorSetLayout();
        AllocateDescriptorSets();
        UpdateDescriptorSets();
    }


    void VulkanDescriptorSet::UpdateStorageBufferBinding(uint32 binding, BufferHandle newBufferHandle)
    {
        ASSERT(binding < m_DescriptorSetLayoutBindings.size(), "Specified binding is out of range of the existing descriptor set")
        const VkDescriptorSetLayoutBinding& targetDescriptorSetLayout = m_DescriptorSetLayoutBindings[binding];
        VkDescriptorType descriptorType = targetDescriptorSetLayout.descriptorType;
        ASSERT(descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, "Expected existing binding to be a storage buffer when updating a binding using UpdateStorageBufferBinding")

        // Finds the target binding buffer by counting the number of storage buffers or uniform buffers before the target binding
        uint32 bufferIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            const VkDescriptorSetLayoutBinding& descriptorSetLayout = m_DescriptorSetLayoutBindings[i];
            if (descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER || descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) { bufferIndex++; }
        }

        VkDescriptorBufferInfo bufferInfo = {};

        m_Buffers[bufferIndex] = newBufferHandle;

        VkBuffer buffer = (VkBuffer)newBufferHandle->GetNativeHandle();

        bufferInfo.buffer = buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = newBufferHandle->GetUsedSize();


        VkWriteDescriptorSet descriptorSetWrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = m_DescriptorSet,
            .dstBinding = binding,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = descriptorType,
            .pBufferInfo = &bufferInfo
        };

        vkUpdateDescriptorSets(m_Device, 1, &descriptorSetWrite, 0, nullptr);
    }


    void VulkanDescriptorSet::UpdateUniformBinding(uint32 binding, BufferHandle newBufferHandle)
    {
        ASSERT(binding < m_DescriptorSetLayoutBindings.size(), "Specified binding is out of range of the existing descriptor set")
        const VkDescriptorSetLayoutBinding& targetDescriptorSetLayout = m_DescriptorSetLayoutBindings[binding];
        VkDescriptorType descriptorType = targetDescriptorSetLayout.descriptorType;
        ASSERT(descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, "Expected existing binding to be a uniform buffer when updating a binding using UpdateUniformBufferBinding")

        // Finds the target binding buffer by counting the number of storage buffers or uniform buffers before the target binding
        uint32 bufferIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            const VkDescriptorSetLayoutBinding& descriptorSetLayout = m_DescriptorSetLayoutBindings[i];
            if (descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER || descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) { bufferIndex++; }
        }

        VkDescriptorBufferInfo bufferInfo = {};

        m_Buffers[bufferIndex] = newBufferHandle;

        VkBuffer buffer = (VkBuffer)newBufferHandle->GetNativeHandle();

        bufferInfo.buffer = buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = newBufferHandle->GetUsedSize();


        VkWriteDescriptorSet descriptorSetWrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = m_DescriptorSet,
            .dstBinding = binding,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = descriptorType,
            .pBufferInfo = &bufferInfo
        };

        vkUpdateDescriptorSets(m_Device, 1, &descriptorSetWrite, 0, nullptr);
    }


    void VulkanDescriptorSet::UpdateImageSamplerBinding(uint32 binding, TextureHandle newTextureHandle, ImageLayout imageLayout)
    {
        ASSERT(binding < m_DescriptorSetLayoutBindings.size(), "Specified binding is out of range of the existing descriptor set")
        const VkDescriptorSetLayoutBinding& targetDescriptorSetLayout = m_DescriptorSetLayoutBindings[binding];
        VkDescriptorType descriptorType = targetDescriptorSetLayout.descriptorType;
        ASSERT(descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, "Expected existing binding to be a image sampler when updating a binding using UpdateImageSamplerBinding")

        // Finds the target binding buffer by counting the number of storage buffers or uniform buffers before the target binding
        uint32 textureIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            const VkDescriptorSetLayoutBinding& descriptorSetLayout = m_DescriptorSetLayoutBindings[i];
            if (descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
                descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ||
                descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
            { textureIndex++; }
        }


        m_Textures[textureIndex] = newTextureHandle;

        VkDescriptorImageInfo imageInfo = {};
        VkImageView imageView = (VkImageView)newTextureHandle->GetNativeImageView();
        VkSampler sampler = (VkSampler)newTextureHandle->GetNativeSampler();

        imageInfo.sampler = sampler;
        imageInfo.imageLayout = ConvertImageLayoutToVkImageLayout(imageLayout);
        imageInfo.imageView = imageView;

        VkWriteDescriptorSet descriptorSetWrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = m_DescriptorSet,
            .dstBinding = binding,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = descriptorType,
            .pImageInfo = &imageInfo
        };

        vkUpdateDescriptorSets(m_Device, 1, &descriptorSetWrite, 0, nullptr);
    }


    void VulkanDescriptorSet::UpdateImageSamplerBinding(uint32 binding, TextureHandle newTextureHandle, uint32 mipLevel, ImageLayout imageLayout)
    {
        ASSERT(binding < m_DescriptorSetLayoutBindings.size(), "Specified binding is out of range of the existing descriptor set")
        const VkDescriptorSetLayoutBinding& targetDescriptorSetLayout = m_DescriptorSetLayoutBindings[binding];
        VkDescriptorType descriptorType = targetDescriptorSetLayout.descriptorType;
        ASSERT(descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, "Expected existing binding to be a image sampler when updating a binding using UpdateImageSamplerBinding")

        // Finds the target binding buffer by counting the number of storage buffers or uniform buffers before the target binding
        uint32 textureIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            const VkDescriptorSetLayoutBinding& descriptorSetLayout = m_DescriptorSetLayoutBindings[i];
            if (descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
                descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ||
                descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
            { textureIndex++; }
        }


        m_Textures[textureIndex] = newTextureHandle;

        VkDescriptorImageInfo imageInfo = {};
        VkImageView imageView = (VkImageView)newTextureHandle->GetNativeMipMapImageView(mipLevel);
        VkSampler sampler = (VkSampler)newTextureHandle->GetNativeSampler();

        imageInfo.sampler = sampler;
        imageInfo.imageLayout = ConvertImageLayoutToVkImageLayout(imageLayout);
        imageInfo.imageView = imageView;

        VkWriteDescriptorSet descriptorSetWrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = m_DescriptorSet,
            .dstBinding = binding,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = descriptorType,
            .pImageInfo = &imageInfo
        };

        vkUpdateDescriptorSets(m_Device, 1, &descriptorSetWrite, 0, nullptr);
    }


    void VulkanDescriptorSet::UpdateStorageImageBinding(uint32 binding, TextureHandle newTextureHandle, uint32 mipLevel, ImageLayout imageLayout)
    {
        ASSERT(binding < m_DescriptorSetLayoutBindings.size(), "Specified binding is out of range of the existing descriptor set")
        const VkDescriptorSetLayoutBinding& targetDescriptorSetLayout = m_DescriptorSetLayoutBindings[binding];
        VkDescriptorType descriptorType = targetDescriptorSetLayout.descriptorType;
        ASSERT(descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, "Expected existing binding to be a storage image when updating a binding using UpdateStorageImageBinding")

        // Finds the target binding buffer by counting the number of storage buffers or uniform buffers before the target binding
        uint32 textureIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            const VkDescriptorSetLayoutBinding& descriptorSetLayout = m_DescriptorSetLayoutBindings[i];
            if (descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
                descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ||
                descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
            { textureIndex++; }
        }


        m_Textures[textureIndex] = newTextureHandle;

        VkDescriptorImageInfo imageInfo = {};
        VkImageView imageView = (VkImageView)newTextureHandle->GetNativeMipMapImageView(mipLevel);
        VkSampler sampler = (VkSampler)newTextureHandle->GetNativeSampler();

        imageInfo.sampler = sampler;
        imageInfo.imageLayout = ConvertImageLayoutToVkImageLayout(imageLayout);
        imageInfo.imageView = imageView;

        VkWriteDescriptorSet descriptorSetWrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = m_DescriptorSet,
            .dstBinding = binding,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = descriptorType,
            .pImageInfo = &imageInfo
        };

        vkUpdateDescriptorSets(m_Device, 1, &descriptorSetWrite, 0, nullptr);
    }


    void VulkanDescriptorSet::UpdateSubpassInputAttachmentBinding(uint32 binding, TextureHandle newTextureHandle)
    {
        ASSERT(binding < m_DescriptorSetLayoutBindings.size(), "Specified binding is out of range of the existing descriptor set")
        const VkDescriptorSetLayoutBinding& targetDescriptorSetLayout = m_DescriptorSetLayoutBindings[binding];
        VkDescriptorType descriptorType = targetDescriptorSetLayout.descriptorType;
        ASSERT(descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, "Expected existing binding to be a input attachment when updating a binding using UpdateSubpassInputAttachmentBinding")

        // Finds the target binding buffer by counting the number of storage buffers or uniform buffers before the target binding
        uint32 textureIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            const VkDescriptorSetLayoutBinding& descriptorSetLayout = m_DescriptorSetLayoutBindings[i];
            if (descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
                descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ||
                descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
            { textureIndex++; }
        }


        m_Textures[textureIndex] = newTextureHandle;

        VkDescriptorImageInfo imageInfo = {};
        VkImageView imageView = (VkImageView)newTextureHandle->GetNativeImageView();
        VkSampler sampler = (VkSampler)newTextureHandle->GetNativeSampler();

        imageInfo.sampler = sampler;
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = imageView;

        VkWriteDescriptorSet descriptorSetWrite = {
            .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
            .pNext = nullptr,
            .dstSet = m_DescriptorSet,
            .dstBinding = binding,
            .dstArrayElement = 0,
            .descriptorCount = 1,
            .descriptorType = descriptorType,
            .pImageInfo = &imageInfo
        };

        vkUpdateDescriptorSets(m_Device, 1, &descriptorSetWrite, 0, nullptr);
    }


    BufferHandle VulkanDescriptorSet::GetStorageBuffer(uint32 binding)
    {
        ASSERT(binding < m_DescriptorSetLayoutBindings.size(), "Specified binding is out of range of the existing descriptor set")
        const VkDescriptorSetLayoutBinding& targetDescriptorSetLayout = m_DescriptorSetLayoutBindings[binding];
        VkDescriptorType descriptorType = targetDescriptorSetLayout.descriptorType;
        ASSERT(descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, "Expected existing binding to be a storage buffer when calling GetStorageBuffer")

        // Finds the target binding buffer by counting the number of storage buffers or uniform buffers before the target binding
        uint32 bufferIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            const VkDescriptorSetLayoutBinding& descriptorSetLayout = m_DescriptorSetLayoutBindings[i];
            if (descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER || descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) { bufferIndex++; }
        }

        return m_Buffers[bufferIndex];
    }


    BufferHandle VulkanDescriptorSet::GetUniformBuffer(uint32 binding)
    {
        ASSERT(binding < m_DescriptorSetLayoutBindings.size(), "Specified binding is out of range of the existing descriptor set")
        const VkDescriptorSetLayoutBinding& targetDescriptorSetLayout = m_DescriptorSetLayoutBindings[binding];
        VkDescriptorType descriptorType = targetDescriptorSetLayout.descriptorType;
        ASSERT(descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, "Expected existing binding to be a uniform buffer when calling GetUniformBuffer")

        // Finds the target binding buffer by counting the number of storage buffers or uniform buffers before the target binding
        uint32 bufferIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            const VkDescriptorSetLayoutBinding& descriptorSetLayout = m_DescriptorSetLayoutBindings[i];
            if (descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER || descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) { bufferIndex++; }
        }

        return m_Buffers[bufferIndex];
    }


    TextureHandle VulkanDescriptorSet::GetImageSampler(uint32 binding)
    {
        ASSERT(binding < m_DescriptorSetLayoutBindings.size(), "Specified binding is out of range of the existing descriptor set")
        const VkDescriptorSetLayoutBinding& targetDescriptorSetLayout = m_DescriptorSetLayoutBindings[binding];
        VkDescriptorType descriptorType = targetDescriptorSetLayout.descriptorType;
        ASSERT(descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, "Expected existing binding to be a combined image sampler when calling GetImageSampler")

        // Finds the target binding image by counting the number of combined image samplers, storage images, and input attachments before the target binding
        uint32 textureIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            const VkDescriptorSetLayoutBinding& descriptorSetLayout = m_DescriptorSetLayoutBindings[i];
            if (descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
                descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ||
                descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
            { textureIndex++; }
        }

        return m_Textures[textureIndex];
    }


    TextureHandle VulkanDescriptorSet::GetStorageImage(uint32 binding)
    {
        ASSERT(binding < m_DescriptorSetLayoutBindings.size(), "Specified binding is out of range of the existing descriptor set")
        const VkDescriptorSetLayoutBinding& targetDescriptorSetLayout = m_DescriptorSetLayoutBindings[binding];
        VkDescriptorType descriptorType = targetDescriptorSetLayout.descriptorType;
        ASSERT(descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, "Expected existing binding to be a storage image when calling GetStorageImage")

        // Finds the target binding image by counting the number of combined image samplers, storage images, and input attachments before the target binding
        uint32 textureIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            const VkDescriptorSetLayoutBinding& descriptorSetLayout = m_DescriptorSetLayoutBindings[i];
            if (descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
                descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ||
                descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
            { textureIndex++; }
        }

        return m_Textures[textureIndex];
    }


    TextureHandle VulkanDescriptorSet::GetSubpassInputAttachment(uint32 binding)
    {
        ASSERT(binding < m_DescriptorSetLayoutBindings.size(), "Specified binding is out of range of the existing descriptor set")
        const VkDescriptorSetLayoutBinding& targetDescriptorSetLayout = m_DescriptorSetLayoutBindings[binding];
        VkDescriptorType descriptorType = targetDescriptorSetLayout.descriptorType;
        ASSERT(descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, "Expected existing binding to be a input attachment when calling GetSubpassInputAttachment")

        // Finds the target binding image by counting the number of combined image samplers, storage images, and input attachments before the target binding
        uint32 textureIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            const VkDescriptorSetLayoutBinding& descriptorSetLayout = m_DescriptorSetLayoutBindings[i];
            if (descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
                descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ||
                descriptorSetLayout.descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
            { textureIndex++; }
        }

        return m_Textures[textureIndex];
    }


    void VulkanDescriptorSet::Invalidate()
    {
        // if (m_DescriptorSet) { FreeDescriptorSet(); m_DescriptorSet = VK_NULL_HANDLE; } // This needs to have the free set bit in the descriptor pool
        if (m_DescriptorPool) { DestroyDescriptorPool(); m_DescriptorPool = VK_NULL_HANDLE; }
        if (m_VkDescriptorSetLayout) { DestroyDescriptorSetLayout(); m_VkDescriptorSetLayout = VK_NULL_HANDLE; }
        m_DescriptorSetLayoutBindings.clear();

        m_Buffers.clear();
        m_Textures.clear();
        m_NumberOfBindings = 0;
    }


    VkShaderStageFlags VulkanDescriptorSet::GetVkShaderStageFromShaderStage(ShaderStage shaderStage)
    {
        VkShaderStageFlags stageFlags;

        switch (shaderStage)
        {
            case ShaderStage::VERTEX: stageFlags = VK_SHADER_STAGE_VERTEX_BIT; break;
            case ShaderStage::FRAGMENT: stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; break;
            case ShaderStage::COMPUTE: stageFlags = VK_SHADER_STAGE_COMPUTE_BIT; break;
            default: stageFlags = VK_SHADER_STAGE_ALL;
        }

        return stageFlags;
    }


    void VulkanDescriptorSet::CreateDescriptorPool()
    {
        uint32 numStorageBuffers = 0;
        uint32 numUniformBuffers = 0;
        uint32 numImageSamplers = 0;
        uint32 numStorageImages = 0;
        uint32 numInputAttachmentSamplers = 0;

        for (const VkDescriptorSetLayoutBinding& layoutBinding : m_DescriptorSetLayoutBindings)
        {
            switch (layoutBinding.descriptorType)
            {
                case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: numStorageBuffers++; break;
                case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER: numUniformBuffers++; break;
                case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: numImageSamplers++; break;
                case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE: numStorageImages++; break;
                case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: numInputAttachmentSamplers++; break;
                default: AE_ERROR("Unsupported descriptor type!")
            }
        }

        VkDescriptorPoolSize poolSizes[5] = {
            {
                .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                .descriptorCount = numStorageBuffers > 0 ? numStorageBuffers : 1
            },
            {
                .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount = numUniformBuffers > 0 ? numUniformBuffers : 1
            },
            {
                .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount = numImageSamplers > 0 ? numImageSamplers : 1
            },
            {
                .type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                .descriptorCount = numStorageImages > 0 ? numStorageImages : 1
            },
            {
                .type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
                .descriptorCount = numInputAttachmentSamplers > 0 ? numInputAttachmentSamplers : 1
            }
        };

        VkDescriptorPoolCreateInfo descriptorPoolInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .maxSets = 1,
            .poolSizeCount = 5,
            .pPoolSizes = poolSizes
        };

        VkResult result = vkCreateDescriptorPool(m_Device, &descriptorPoolInfo, nullptr, &m_DescriptorPool);
        ASSERT(result == VK_SUCCESS, "Failed to create descriptor pool!");
    }


    void VulkanDescriptorSet::DestroyDescriptorPool()
    {
        vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
        m_DescriptorPool = VK_NULL_HANDLE;
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

        VkResult result = vkCreateDescriptorSetLayout(m_Device, &descriptorSetLayoutCreateInfo, nullptr, &m_VkDescriptorSetLayout);
        ASSERT(result == VK_SUCCESS, "Failed to create descriptor set layout!");
    }


    void VulkanDescriptorSet::DestroyDescriptorSetLayout()
    {
        vkDestroyDescriptorSetLayout(m_Device, m_VkDescriptorSetLayout, nullptr);
        m_VkDescriptorSetLayout = VK_NULL_HANDLE;
    }


    void VulkanDescriptorSet::AllocateDescriptorSets()
    {
        VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .pNext = nullptr,
            .descriptorPool = m_DescriptorPool,
            .descriptorSetCount = 1,
            .pSetLayouts = &m_VkDescriptorSetLayout
        };

        VkResult result = vkAllocateDescriptorSets(m_Device, &descriptorSetAllocateInfo, &m_DescriptorSet);
        ASSERT(result == VK_SUCCESS, "Failed to allocate descriptor sets!");
    }


    void VulkanDescriptorSet::FreeDescriptorSet()
    {
        vkFreeDescriptorSets(m_Device, m_DescriptorPool, 1, &m_DescriptorSet);
        m_DescriptorSet = VK_NULL_HANDLE;
    }


    void VulkanDescriptorSet::UpdateDescriptorSets()
    {
        uint32 bufferIndex = 0;
        uint32 samplerIndex = 0;
        for (uint32 i = 0; i < m_DescriptorSetLayoutBindings.size(); i++)
        {
            const VkDescriptorSetLayoutBinding& descriptorSetLayout = m_DescriptorSetLayoutBindings[i];
            VkDescriptorType descriptorType = descriptorSetLayout.descriptorType;

            if (descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER || descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
            {
                VkDescriptorBufferInfo bufferInfo = {};

                BufferHandle bufferHandle = m_Buffers[bufferIndex];
                bufferIndex++;

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
            else if (descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
                    descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ||
                    descriptorType == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
            {
                VkDescriptorImageInfo imageInfo = {};

                TextureHandle textureHandle = m_Textures[samplerIndex];
                ImageLayout imageLayout = m_ImageDescriptorLayouts[samplerIndex];
                samplerIndex++;

                VkImageView imageView = (VkImageView)textureHandle->GetNativeImageView();
                VkSampler sampler = (VkSampler)textureHandle->GetNativeSampler();


                imageInfo.sampler = sampler;
                imageInfo.imageLayout = ConvertImageLayoutToVkImageLayout(imageLayout);
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
