/**
* @file VulkanTexture.h
* @author Andrew Fagan
* @date 5/22/2025
*/


#pragma once

#include "Renderer/RHI/Resources/Texture.h"

#include <vulkan/vulkan_core.h>

#include "VulkanBuffer.h"
#include "Core/CoreMacroDefinitions.h"

namespace Astral {

    struct VulkanTextureDesc
    {
        VulkanDevice& VulkanDevice;
        VkDevice Device;
        unsigned char* ImageData;
        VkFormat ImageFormat;
        uint32 ImageWidth;
        uint32 ImageHeight;
        VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
    };

    class VulkanTexture : public Texture
    {
    public:
        explicit VulkanTexture(const VulkanTextureDesc& desc);
        ~VulkanTexture() override;

        int GetHeight() override { return m_ImageHeight; }
        int GetWidth() override { return m_ImageWidth; }

        void* GetSampler() override { return m_Sampler; }
        void* GetNativeHandle() override { return m_ImageView; }

    private:

        void CreateTexture(const VulkanTextureDesc& desc);
        void DestroyTexture();

        void AllocateTextureMemory();
        void FreeTextureMemory();

        void CreateImageView(const VulkanTextureDesc& desc);
        void DestroyImageView();

        void UploadDataToTexture(const VulkanTextureDesc& desc);

        void TransitionImageLayout(const VulkanTextureDesc& desc, VkImageLayout oldLayout, VkImageLayout newLayout);
        void CopyFromStagingBuffer(VulkanBuffer& stagingBuffer, const VulkanTextureDesc& desc);

        void CreateImageSampler();
        void DestroyImageSampler();

        uint32 GetMemoryTypeIndex(uint32 memoryTypeBitsMask);
        uint32 GetBytesPerTexFormat(VkFormat format);



        VkDevice m_Device;
        VkPhysicalDeviceMemoryProperties m_PhysicalDeviceMemoryProperties;
        uint32 m_ImageWidth;
        uint32 m_ImageHeight;
        VkFormat m_Format;

        VkImage m_Image;
        VkDeviceMemory m_ImageMemory;
        VkImageView m_ImageView;
        VkSampler m_Sampler;

    };

}
