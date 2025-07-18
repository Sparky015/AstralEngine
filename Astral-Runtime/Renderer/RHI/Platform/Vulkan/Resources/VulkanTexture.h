/**
* @file VulkanTexture.h
* @author Andrew Fagan
* @date 5/22/2025
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Renderer/RHI/Common/ImageUsageFlags.h"
#include "Renderer/RHI/Resources/Texture.h"
#include "Renderer/RHI/Platform/Vulkan/Common/VkEnumConversions.h"
#include "VulkanBuffer.h"

#include <vulkan/vulkan_core.h>

namespace Astral {

    /**
    * @brief Information to create a vulkan texture from
    */
    struct VulkanTextureDesc
    {
        VulkanDevice* VulkanDevice; // Will be valid as long as the rendering context is valid
        VkDevice Device;
        VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
        unsigned char* ImageData;
        ImageFormat ImageFormat;
        ImageLayout ImageLayout;
        ImageUsageFlags ImageUsageFlags;
        uint32 ImageWidth;
        uint32 ImageHeight;
    };

    /**
     * @brief Vulkan Backend RHI Texture Object
     */
    class VulkanTexture : public Texture
    {
    public:
        explicit VulkanTexture(const VulkanTextureDesc& desc);
        explicit VulkanTexture(VkDevice device, VkImage image, VkImageView imageView, ImageLayout layout, ImageFormat format, uint32 width, uint32 height);
        ~VulkanTexture() override;

        /**
         * @brief  Gets the image width
         * @return The image width
         */
        int GetHeight() override { return m_ImageHeight; }

        /**
        * @brief  Gets the image height
        * @return The image height
        */
        int GetWidth() override { return m_ImageWidth; }

        /**
         * @brief  Gets the image dimensions
         * @return The image dimensions
         */
        UVec2 GetDimensions() override { return UVec2(m_ImageWidth, m_ImageHeight); }

        /**
         * @brief  Gets the image aspects of the texture
         * @return The image aspects of the texture
         */
        ImageAspectFlags GetImageAspect() override { return m_ImageAspect; }

        /**
         * @brief  Gets the image layout of the texture
         * @return The image layout of the texture
         */
        ImageLayout GetLayout() override { return ConvertVkImageLayoutToImageLayout(m_CurrentLayout); }

        /**
         * @brief Updates the layout of the texture to the specified layout
         * @param imageLayout The image layout to update the texture to
         */
        void UpdateLayout(ImageLayout imageLayout) override;

        /**
         * @brief  Gets the image format of the texture
         * @return The image format of the texture
         */
        ImageFormat GetFormat() override { return ConvertVkFormatToImageFormat(m_Format); }

        /**
         * @brief  Gets the image sampler of the texture
         * @return The image sampler of the texture
         * @note   The void pointer maps to the native vulkan sampler handle (VkSampler)
         */
        void* GetSampler() override { return m_Sampler; }

        /**
         * @brief  Gets the native image handle of the texture
         * @return The native image handle of the texture
         * @note   The void pointer maps to the native vulkan image handle (VkImage)
         */
        void* GetNativeImage() override { return m_Image; }

        /**
         * @brief  Gets the native image view handle of the texture
         * @return The native image view of the texture
         * @note   The void pointer maps to the native vulkan image view handle (VkImageView)
         */
        void* GetNativeImageView() override { return m_ImageView; }

    private:

        /**
         * @brief Creates the VkImage for the texture
         * @param imageUsageFlags The image usage flags for the texture
         */
        void CreateTexture(ImageUsageFlags imageUsageFlags);

        /**
         * @brief Destroys the VkImage for the texture
         */
        void DestroyTexture();

        /**
         * @brief Allocates the device memory for the texture
         */
        void AllocateTextureMemory();

        /**
         * @brief Frees the device memory for the texture
         */
        void FreeTextureMemory();

        /**
         * @brief Creates the image view for the texture (VkImageView)
         */
        void CreateImageView(ImageUsageFlags imageUsageFlags);

        /**
         * @brief Destroys the image view for the texture (VkImageView)
         */
        void DestroyImageView();

        /**
         * @brief Uploads data to the device local texture through a staging buffer
         * @param data The data to upload
         */
        void UploadDataToTexture(uint8* data);

        /**
         * @brief Transitions the current layout to the new specified layout
         * @param currentLayout The current layout
         * @param newLayout The new layout
         */
        void TransitionImageLayout(VkImageLayout currentLayout, VkImageLayout newLayout);

        /**
         * @brief Copies the texture data from the staging buffer to the device local texture
         * @param stagingBuffer The staging buffer that contains the texture data
         */
        void CopyFromStagingBuffer(VulkanBuffer& stagingBuffer);

        /**
         * @brief Creates the VkImageSampler for the texture
         */
        void CreateImageSampler();

        /**
         * @brief Destroys the VkImageSampler for the texture
         */
        void DestroyImageSampler();

        /**
         * @brief Gets the index of the requested memory type in the physical device memory properties
         */
        uint32 GetMemoryTypeIndex(uint32 memoryTypeBitsMask);

        /**
         * @brief Gets the number of bytes per texel in the format
         * @note This only support uncompressed formats
         * TODO: Add support for compressed formats
         * @return The number of bytes per texel in the format
         */
        uint32 GetBytesPerTexFormat(VkFormat format);


        VulkanDevice* m_DeviceManager;
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
        ImageAspectFlags m_ImageAspect;

        bool m_IsSwapchainOwned;
    };

}
