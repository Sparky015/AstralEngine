/**
* @file VulkanTexture.cpp
* @author Andrew Fagan
* @date 5/22/2025
*/

#include "VulkanTexture.h"

#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Error.h"

namespace Astral {

    VulkanTexture::VulkanTexture(const VulkanTextureDesc& desc) :
        m_Device(desc.Device),
        m_Image(),
        m_ImageView(),
        m_Sampler(),
        m_ImageWidth(desc.ImageWidth),
        m_ImageHeight(desc.ImageHeight)
    {
        CreateTexture(desc);
    }


    VulkanTexture::~VulkanTexture()
    {

    }


    void VulkanTexture::CreateTexture(const VulkanTextureDesc& desc)
    {
        VkImageUsageFlags imageUsage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

        VkImageCreateInfo imageCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .imageType = VK_IMAGE_TYPE_2D,
            .format = desc.ImageFormat,
            .extent = {.width = m_ImageWidth, .height = m_ImageHeight},
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = imageUsage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .queueFamilyIndexCount = 0,
            .pQueueFamilyIndices = nullptr,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
        };

        VkResult result = vkCreateImage(m_Device, &imageCreateInfo, nullptr, &m_Image);
        ASSERT(result == VK_SUCCESS, "Failed to create image!");

        VkMemoryRequirements memoryRequirements = {};
        vkGetImageMemoryRequirements(m_Device, m_Image, &memoryRequirements);

        uint32 memoryTypeIndex = GetMemoryTypeIndex(memoryRequirements.memoryTypeBits);

        VkMemoryAllocateInfo memoryAllocateInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .pNext = nullptr,
            .allocationSize = memoryRequirements.size,
            .memoryTypeIndex = memoryTypeIndex,
        };

        result = vkAllocateMemory(m_Device, &memoryAllocateInfo, nullptr, &m_ImageMemory);
        ASSERT(result == VK_SUCCESS, "Failed to allocate memory!");

        result = vkBindImageMemory(m_Device, m_Image, m_ImageMemory, 0);
        ASSERT(result == VK_SUCCESS, "Failed to bind image!");
    }


    void VulkanTexture::CreateTextureImageView()
    {
    }


    void VulkanTexture::UploadDataToTexture(const VulkanTextureDesc& desc)
    {
        uint32 bytesPerPixel = GetBytesPerTexFormat(desc.ImageFormat);
        uint32 layerSize = m_ImageWidth * m_ImageHeight * bytesPerPixel;
        uint32 layerCount = 1;
        uint32 imageSize = layerCount * layerSize;



    }


    uint32 VulkanTexture::GetMemoryTypeIndex(uint32 memoryTypeBitsMask)
    {
        VkMemoryPropertyFlagBits requestedMemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

        for (uint32 i = 0; i < m_PhysicalDeviceMemoryProperties.memoryTypeCount; i++)
        {
            const VkMemoryType& memoryType = m_PhysicalDeviceMemoryProperties.memoryTypes[i];
            uint32 currentBitmask = (1 << i);
            bool isCurrentMemoryTypeSupported = (memoryTypeBitsMask & currentBitmask);
            bool hasRequiredMemoryProperties = ((memoryType.propertyFlags & requestedMemoryPropertyFlags) == requestedMemoryPropertyFlags);
            if (isCurrentMemoryTypeSupported && hasRequiredMemoryProperties) { return i; }
        }

        ASTRAL_ERROR("Failed to find a suitable memory type for Buffer object!");
    }


    uint32 VulkanTexture::GetBytesPerTexFormat(VkFormat format)
    {
        switch (format)
        {
            // Single channel 8-bit
            case VK_FORMAT_R8_UNORM: return 1;
            case VK_FORMAT_R8_SNORM: return 1;
            case VK_FORMAT_R8_UINT: return 1;
            case VK_FORMAT_R8_SINT: return 1;
            case VK_FORMAT_R8_SRGB: return 1;

            // Dual channel 8-bit
            case VK_FORMAT_R8G8_UNORM: return 2;
            case VK_FORMAT_R8G8_SNORM: return 2;
            case VK_FORMAT_R8G8_UINT: return 2;
            case VK_FORMAT_R8G8_SINT: return 2;
            case VK_FORMAT_R8G8_SRGB: return 2;

            // Triple channel 8-bit (rarely used due to alignment)
            case VK_FORMAT_R8G8B8_UNORM: return 3;
            case VK_FORMAT_R8G8B8_SNORM: return 3;
            case VK_FORMAT_R8G8B8_UINT: return 3;
            case VK_FORMAT_R8G8B8_SINT: return 3;
            case VK_FORMAT_R8G8B8_SRGB: return 3;
            case VK_FORMAT_B8G8R8_UNORM: return 3;
            case VK_FORMAT_B8G8R8_SNORM: return 3;
            case VK_FORMAT_B8G8R8_UINT: return 3;
            case VK_FORMAT_B8G8R8_SINT: return 3;
            case VK_FORMAT_B8G8R8_SRGB: return 3;

            // Quad channel 8-bit (most common for textures)
            case VK_FORMAT_R8G8B8A8_UNORM: return 4;
            case VK_FORMAT_R8G8B8A8_SNORM: return 4;
            case VK_FORMAT_R8G8B8A8_UINT: return 4;
            case VK_FORMAT_R8G8B8A8_SINT: return 4;
            case VK_FORMAT_R8G8B8A8_SRGB: return 4;
            case VK_FORMAT_B8G8R8A8_UNORM: return 4;
            case VK_FORMAT_B8G8R8A8_SNORM: return 4;
            case VK_FORMAT_B8G8R8A8_UINT: return 4;
            case VK_FORMAT_B8G8R8A8_SINT: return 4;
            case VK_FORMAT_B8G8R8A8_SRGB: return 4;

            // 16-bit single channel
            case VK_FORMAT_R16_UNORM: return 2;
            case VK_FORMAT_R16_SNORM: return 2;
            case VK_FORMAT_R16_UINT: return 2;
            case VK_FORMAT_R16_SINT: return 2;
            case VK_FORMAT_R16_SFLOAT: return 2;

            // 16-bit dual channel (great for normal maps)
            case VK_FORMAT_R16G16_UNORM: return 4;
            case VK_FORMAT_R16G16_SNORM: return 4;
            case VK_FORMAT_R16G16_UINT: return 4;
            case VK_FORMAT_R16G16_SINT: return 4;
            case VK_FORMAT_R16G16_SFLOAT: return 4;

            // 16-bit triple channel
            case VK_FORMAT_R16G16B16_UNORM: return 6;
            case VK_FORMAT_R16G16B16_SNORM: return 6;
            case VK_FORMAT_R16G16B16_UINT: return 6;
            case VK_FORMAT_R16G16B16_SINT: return 6;
            case VK_FORMAT_R16G16B16_SFLOAT: return 6;

            // 16-bit quad channel (HDR textures)
            case VK_FORMAT_R16G16B16A16_UNORM: return 8;
            case VK_FORMAT_R16G16B16A16_SNORM: return 8;
            case VK_FORMAT_R16G16B16A16_UINT: return 8;
            case VK_FORMAT_R16G16B16A16_SINT: return 8;
            case VK_FORMAT_R16G16B16A16_SFLOAT: return 8;

            // 32-bit single channel
            case VK_FORMAT_R32_UINT: return 4;
            case VK_FORMAT_R32_SINT: return 4;
            case VK_FORMAT_R32_SFLOAT: return 4;

            // 32-bit dual channel
            case VK_FORMAT_R32G32_UINT: return 8;
            case VK_FORMAT_R32G32_SINT: return 8;
            case VK_FORMAT_R32G32_SFLOAT: return 8;

            // 32-bit triple channel
            case VK_FORMAT_R32G32B32_UINT: return 12;
            case VK_FORMAT_R32G32B32_SINT: return 12;
            case VK_FORMAT_R32G32B32_SFLOAT: return 12;

            // 32-bit quad channel (render targets)
            case VK_FORMAT_R32G32B32A32_UINT: return 16;
            case VK_FORMAT_R32G32B32A32_SINT: return 16;
            case VK_FORMAT_R32G32B32A32_SFLOAT: return 16;

            // Packed formats (mobile-friendly)
            case VK_FORMAT_R5G6B5_UNORM_PACK16: return 2;
            case VK_FORMAT_B5G6R5_UNORM_PACK16: return 2;
            case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return 2;
            case VK_FORMAT_B4G4R4A4_UNORM_PACK16: return 2;
            case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return 2;
            case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return 2;
            case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return 2;

            // Depth/Stencil formats (essential for 3D)
            case VK_FORMAT_D16_UNORM: return 2;
            case VK_FORMAT_D32_SFLOAT: return 4;
            case VK_FORMAT_D24_UNORM_S8_UINT: return 4;
            case VK_FORMAT_D32_SFLOAT_S8_UINT: return 5;  // 4 for depth + 1 for stencil
            case VK_FORMAT_D16_UNORM_S8_UINT: return 3;   // 2 for depth + 1 for stencil
            case VK_FORMAT_S8_UINT: return 1;

            // Special formats
            case VK_FORMAT_B10G11R11_UFLOAT_PACK32: return 4;  // HDR without alpha
            case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: return 4;   // Shared exponent HDR

            // Undefined
            case VK_FORMAT_UNDEFINED: ASTRAL_ERROR("Undefined format cannot be applied to texture!");

            default: ASTRAL_ERROR("Unsupported format given to texture!");  // Unknown format
        }
    }

}
