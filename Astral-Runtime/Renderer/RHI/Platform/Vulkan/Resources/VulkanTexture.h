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
#include "Renderer/RHI/Common/ImageUsageFlags.h"
#include "Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.h"

namespace Astral {

    struct VulkanTextureDesc
    {
        VulkanDevice& VulkanDevice;
        VkDevice Device;
        VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
        unsigned char* ImageData;
        ImageFormat ImageFormat;
        ImageLayout ImageLayout;
        ImageUsageFlags ImageUsageFlags;
        uint32 ImageWidth;
        uint32 ImageHeight;
    };

    class VulkanTexture : public Texture
    {
    public:
        explicit VulkanTexture(const VulkanTextureDesc& desc);
        ~VulkanTexture() override;

        int GetHeight() override { return m_ImageHeight; }
        int GetWidth() override { return m_ImageWidth; }
        UVec2 GetDimensions() override { return UVec2(m_ImageWidth, m_ImageHeight); }

        ImageLayout GetLayout() override { return ConvertVkImageLayoutToImageLayout(m_CurrentLayout); }
        ImageFormat GetFormat() override { return ConvertVkFormatToImageFormat(m_Format); }

        void* GetSampler() override { return m_Sampler; }
        void* GetNativeHandle() override { return m_ImageView; }

    private:

        void CreateTexture(ImageUsageFlags imageUsageFlags);
        void DestroyTexture();

        void AllocateTextureMemory();
        void FreeTextureMemory();

        void CreateImageView();
        void DestroyImageView();

        void UploadDataToTexture(uint8* data);

        void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
        void CopyFromStagingBuffer(VulkanBuffer& stagingBuffer);

        void CreateImageSampler();
        void DestroyImageSampler();

        uint32 GetMemoryTypeIndex(uint32 memoryTypeBitsMask);
        uint32 GetBytesPerTexFormat(VkFormat format);


        VulkanDevice& m_DeviceManager;
        VkDevice m_Device;
        VkPhysicalDeviceMemoryProperties m_PhysicalDeviceMemoryProperties;
        uint32 m_ImageWidth;
        uint32 m_ImageHeight;
        VkFormat m_Format;
        VkImageLayout m_CurrentLayout;

        VkImage m_Image;
        VkDeviceMemory m_ImageMemory;
        VkImageView m_ImageView;
        VkSampler m_Sampler;

    };

}
