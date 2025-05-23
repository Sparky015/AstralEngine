/**
* @file VulkanTexture.h
* @author Andrew Fagan
* @date 5/22/2025
*/


#pragma once

#include "Renderer/RHI/Resources/Texture.h"

#include <vulkan/vulkan_core.h>

#include "Core/CoreMacroDefinitions.h"

namespace Astral {

    struct VulkanTextureDesc
    {
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

    private:

        void CreateTexture(const VulkanTextureDesc& desc);
        void CreateTextureImageView();
        void UploadDataToTexture(const VulkanTextureDesc& desc);

        uint32 GetMemoryTypeIndex(uint32 memoryTypeBitsMask);
        uint32 GetBytesPerTexFormat(VkFormat format);

        VkDevice m_Device;
        VkPhysicalDeviceMemoryProperties m_PhysicalDeviceMemoryProperties;
        uint32 m_ImageWidth;
        uint32 m_ImageHeight;

        VkImage m_Image;
        VkDeviceMemory m_ImageMemory;
        VkImageView m_ImageView;
        VkSampler m_Sampler;

    };

}
