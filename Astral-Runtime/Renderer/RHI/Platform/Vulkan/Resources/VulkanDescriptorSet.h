/**
* @file VulkanDescriptorSet.h
* @author Andrew Fagan
* @date 5/18/2025
*/


#pragma once

#include "Renderer/RHI/Resources/DescriptorSet.h"

#include <vulkan/vulkan_core.h>
#include <queue>

namespace Astral {

    struct VulkanDescriptorSetDesc
    {
        VkDevice Device;
    };

    class VulkanDescriptorSet : public DescriptorSet
    {
    public:
        VulkanDescriptorSet(const VulkanDescriptorSetDesc& desc);
        ~VulkanDescriptorSet() override;

        void BeginBuildingSet() override;
        void AddDescriptorStorageBuffer(BufferHandle bufferHandle, ShaderStage bindStage) override;
        void AddDescriptorUniformBuffer(BufferHandle bufferHandle, ShaderStage bindStage) override;
        void AddDescriptorImageSampler(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) override;
        void AddDescriptorStorageImage(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) override;
        void AddDescriptorSubpassInputAttachment(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) override;
        void EndBuildingSet() override;

        void UpdateStorageBufferBinding(uint32 binding, BufferHandle newBufferHandle) override;
        void UpdateUniformBinding(uint32 binding, BufferHandle newBufferHandle) override;
        void UpdateImageSamplerBinding(uint32 binding, TextureHandle newTextureHandle, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) override;
        void UpdateImageSamplerBinding(uint32 binding, TextureHandle newTextureHandle, uint32 mipLevel, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) override;
        void UpdateStorageImageBinding(uint32 binding, TextureHandle newTextureHandle, uint32 mipLevel, ImageLayout imageLayout) override;
        void UpdateSubpassInputAttachmentBinding(uint32 binding, TextureHandle newTextureHandle) override;

        BufferHandle GetStorageBuffer(uint32 binding) override;
        BufferHandle GetUniformBuffer(uint32 binding) override;
        TextureHandle GetImageSampler(uint32 binding) override;
        TextureHandle GetStorageImage(uint32 binding) override;
        TextureHandle GetSubpassInputAttachment(uint32 binding) override;

        const DescriptorSetLayout& GetDescriptorSetLayout() override { return m_DescriptorSetLayout; }

        void* GetNativeLayout() override { return m_VkDescriptorSetLayout; }
        void* GetNativeHandle() override { return m_DescriptorSet; }

        void Invalidate();

    private:

        VkShaderStageFlags GetVkShaderStageFromShaderStage(ShaderStage shaderStage);

        void CreateDescriptorPool();
        void DestroyDescriptorPool();

        void CreateDescriptorSetLayout();
        void DestroyDescriptorSetLayout();

        void AllocateDescriptorSets();
        void FreeDescriptorSet();

        void UpdateDescriptorSets();

        VkDevice m_Device;

        std::vector<ImageLayout> m_ImageDescriptorLayouts;
        std::vector<VkDescriptorSetLayoutBinding> m_DescriptorSetLayoutBindings;
        uint32 m_NumberOfBindings;

        std::vector<BufferHandle> m_Buffers;
        std::vector<TextureHandle> m_Textures;

        VkDescriptorPool m_DescriptorPool;
        VkDescriptorSet m_DescriptorSet;
        VkDescriptorSetLayout m_VkDescriptorSetLayout;

        DescriptorSetLayout m_DescriptorSetLayout;
    };

}
