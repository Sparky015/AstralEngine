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

    enum class TextureType : uint32
    {
        IMAGE_1D,
        IMAGE_2D,
        IMAGE_3D,
        CUBEMAP
    };

    /**
    * @brief Information to create a vulkan texture from
    */
    struct VulkanTextureDesc
    {
        VulkanDevice* VulkanDevice; // Will be valid as long as the rendering context is valid
        VkDevice Device;
        VkPhysicalDeviceMemoryProperties PhysicalDeviceMemoryProperties;
        unsigned char* ImageData;
        uint32 ImageDataLength;
        ImageFormat ImageFormat;
        ImageLayout ImageLayout;
        ImageUsageFlags ImageUsageFlags;
        uint32 ImageWidth;
        uint32 ImageHeight;
        uint32 NumLayers;
        uint32 NumMipLevels;
        bool GenerateMipMaps;
        TextureType TextureType;
        SampleCount MSAASampleCount;
        SamplerFilter SamplerFilter;
        SamplerAddressMode SamplerAddressMode;
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
         * @brief  Gets the number of layers that the texture has
         * @return The number of layers that the texture has
         */
        uint32 GetNumLayers() override { return m_NumLayers; }

        /**
         * @brief  Gets the number of mipmaps that the texture has
         * @return The number of mipmaps that the texture has
         */
        uint32 GetNumMipLevels() override { return m_NumMipLevels; }

        /**
         * @brief  Gets the native image view of a specific layer in the texture
         * @return The native image view of a specific layer in the texture or nullptr if the layer num is not valid
         * @note   The void pointer maps to the native vulkan image view handle (VkImageView)
         */
        void* GetNativeLayerImageView(uint32 layer) override
        {
            if (layer > m_NumLayers) { return nullptr; }
            if (m_NumLayers == 1) { return m_ImageView; }
            return m_LayerImageViews[layer];
        }

        /**
         * @brief  Gets the native image view of a specific mip level in the texture
         * @return The native image view of a specific mip level in the texture
         * @note   The void pointer maps to the native vulkan image view handle (VkImageView)
         */
        void* GetNativeMipMapImageView(uint32 mipLevel) override;

        /**
         * @brief  Gets the native image view handle of the texture at a specific layer and mip level
         * @param layer The layer to view the image at
         * @param mipLevel The mip level to view the image at
         * @return The native image view of the texture
         * @note   The void pointer maps to the native image view handle of the selected renderer api backend
         */
        void* GetNativeImageView(uint32 layer, uint32 mipLevel) override;


        /**
         * @brief  Gets the image sampler of the texture
         * @return The image sampler of the texture
         * @note   The void pointer maps to the native vulkan sampler handle (VkSampler)
         */
        void* GetNativeSampler() override { return m_Sampler; }

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
         * @param desc The image usage flags for the texture
         */
        void CreateTexture(const VulkanTextureDesc& desc);

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
         * @param dataLength
         * @param generateMipMaps
         */
        void UploadDataToTexture(uint8* data, uint32 dataLength, bool generateMipMaps);

        /**
         * @brief Transitions the current layout to the new specified layout
         * @param currentLayout The current layout
         * @param newLayout The new layout
         */
        void TransitionImageLayout(VkImageLayout currentLayout, VkImageLayout newLayout);

        /**
         * @brief Copies the texture data from the staging buffer to the device local texture
         * @param stagingBuffer The staging buffer that contains the texture data
         * @param generateMipMaps
         */
        void CopyFromStagingBuffer(VulkanBuffer& stagingBuffer, bool generateMipMaps);

        /**
         * @brief Creates the VkImageSampler for the texture
         */
        void CreateImageSampler(SamplerFilter samplerFilter, SamplerAddressMode samplerAddressMode);

        /**
         * @brief Destroys the VkImageSampler for the texture
         */
        void DestroyImageSampler();

        /**
         * @brief Gets the index of the requested memory type in the physical device memory properties
         */
        uint32 GetMemoryTypeIndex(uint32 memoryTypeBitsMask);


        VulkanDevice* m_DeviceManager;
        VkDevice m_Device;
        VkPhysicalDeviceMemoryProperties m_PhysicalDeviceMemoryProperties;
        uint32 m_ImageWidth;
        uint32 m_ImageHeight;
        uint32 m_ImageDepth;
        VkFormat m_Format;
        VkImageLayout m_CurrentLayout;

        VkImage m_Image;
        VkDeviceMemory m_ImageMemory;
        uint32 m_AllocationSize;
        VkImageView m_ImageView;
        ImageUsageFlags m_ImageUsageFlags;
        std::vector<VkImageView> m_LayerImageViews; // All layer image views are at mip 0
        std::map<std::pair<uint32, uint32>, VkImageView> m_LayerMipImageViews;

        VkSampler m_Sampler;
        ImageAspectFlags m_ImageAspect;
        uint32 m_NumLayers;
        uint32 m_NumMipLevels;
        TextureType m_TextureType;

        bool m_IsSwapchainOwned;
    };

}
