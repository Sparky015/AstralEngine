/**
* @file MetalTexture.h
* @author Andrew Fagan
* @date 4/11/26
*/

#pragma once

#include "Metal/MTL4ComputeCommandEncoder.hpp"
#include "Renderer/RHI/Resources/Texture.h"
#include "Renderer/RHI/Resources/Buffer.h"

#include "Metal/MTLDevice.hpp"
#include "Metal/MTLSampler.hpp"
#include "Metal/MTLBlitCommandEncoder.hpp"

namespace Astral {

    /**
     * @brief A description of how to build a Metal texture
     */
    struct MetalTextureDesc
    {
        MTL::Device* Device;
        unsigned char* ImageData;
        uint32 ImageDataLength;
        GPUMemoryType MemoryType;
        ImageFormat ImageFormat;
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
        bool EnableAnisotropy;
    };

    /**
     * @brief A description of how to build a Metal texture from a Metal Drawable owned texture
     */
    struct MetalDrawableOwnedTextureDesc
    {
        MTL::Device* Device;
        MTL::Texture* DrawableOwnedTexture;
    };

    /**
     * @brief A wrapper around a Metal texture supporting extra convenience functions
     */
    class MetalTexture : public Texture
    {
    public:
        explicit MetalTexture(const MetalTextureDesc& desc);
        explicit MetalTexture(const MetalDrawableOwnedTextureDesc& desc);
        ~MetalTexture() override;

        /**
         * @brief  Gets the image width
         * @return The image width
         */
        int GetWidth() override;

        /**
         * @brief  Gets the image height
         * @return The image height
         */
        int GetHeight() override;

        /**
         * @brief  Gets the image dimensions
         * @return The image dimensions
         */
        UVec2 GetDimensions() override;

        /**
         * @brief  Gets the image aspects of the texture
         * @return The image aspects of the texture
         */
        ImageAspectFlags GetImageAspect() override;

        /**
         * @brief  Gets the image layout of the texture
         * @return The image layout of the texture
         */
        ImageLayout GetLayout() override;

        /**
         * @brief Updates the layout of the texture to the specified layout
         * @param imageLayout The image layout to update the texture to
         */
        void UpdateLayout(ImageLayout imageLayout) override;

        /**
         * @brief  Gets the image format of the texture
         * @return The image format of the texture
         */
        ImageFormat GetFormat() override;

        /**
         * @brief  Gets the number of layers that the texture has
         * @return The number of layers that the texture has
         */
        uint32 GetNumLayers() override;

        /**
         * @brief  Gets the number of mipmaps that the texture has
         * @return The number of mipmaps that the texture has
         */
        uint32 GetNumMipLevels() override;

        /**
         * @brief  Gets the image sampler of the texture
         * @return The image sampler of the texture
         * @note   The void pointer maps to the native sampler handle of the selected renderer api backend
         */
        void* GetNativeSampler() override;

        /**
         * @brief  Gets the native image handle of the texture
         * @return The native image handle of the texture
         * @note   The void pointer maps to the native image handle of the selected renderer api backend
         */
        void* GetNativeImage() override;

        /**
         * @brief  Gets the native image view handle of the texture
         * @return The native image view of the texture
         * @note   The void pointer maps to the native image view handle of the selected renderer api backend
         */
        void* GetNativeImageView() override;

        /**
         * @brief  Gets the native image view of a specific layer in the texture
         * @return The native image view of a specific layer in the texture
         * @note   The void pointer maps to the native vulkan image view handle (VkImageView)
         */
        void* GetNativeLayerImageView(uint32 layer) override;

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

        MetalTexture(const MetalTexture&) = delete;
        MetalTexture& operator=(const MetalTexture&) = delete;
        MetalTexture(MetalTexture&& other) noexcept;
        MetalTexture& operator=(MetalTexture&& other) noexcept;

    private:

        /**
         * @brief Creates the MTL::Texture object
         * @param desc The image usage flags for the texture
         */
        void CreateTexture(const MetalTextureDesc& desc);

        /**
         * @brief Releases the MTL::Texture object
         */
        void DestroyTexture();

        /**
         * @brief Creates the MTL::SamplerState object
         * @param samplerFilter The sampler filter to use
         * @param samplerAddressMode The sampler address mode to use
         * @param shouldEnableAnisotropy Enables anisotropy for the sampler when true
         */
        void CreateSampler(SamplerFilter samplerFilter, SamplerAddressMode samplerAddressMode, bool shouldEnableAnisotropy);

        /**
         * @brief Releases the MTL::SamplerState object
         */
        void DestroySampler();

        /**
         * @brief Initializes texture data and generates mipmaps (if enabled)
         */
        void InitializeTextureData(const MetalTextureDesc& desc);

        /**
         * @brief Uploads texture data on initial construction
         */
        void UploadToPrivateTextureMemory(MTL4::ComputeCommandEncoder* blitEncoder, void* data, uint32 length);

        /**
         * @brief Copies data to shared texture memory on initial construction
         */
        void CopyToSharedTextureMemory(void* data, uint32 length);

        /**
         * @brief Copies texture data from a staging buffer to private memory
         * @param blitEncoder
         * @param stagingBuffer The staging buffer to copy from
         * @param length The length of data to copy
         * @note This is only for private memory
         */
        void CopyFromStagingBuffer(MTL4::ComputeCommandEncoder* blitEncoder, Buffer& stagingBuffer, uint32 length);

        /**
         * @brief Generates mip maps for texture
         * @param blitEncoder The blit encoder to use when generating mip maps
         */
        void GenerateMipMaps(MTL4::ComputeCommandEncoder* blitEncoder);


        MTL::Device* m_Device;
        MTL::Texture* m_Texture;
        MTL::SamplerState* m_Sampler;

        uint32 m_Width;
        uint32 m_Height;
        ImageFormat m_ImageFormat;
        ImageUsageFlags m_ImageUsageFlags;
        uint32 m_NumLayers;
        uint32 m_NumMipLevels;
        TextureType m_TextureType;
        GPUMemoryType m_MemoryType;

        SampleCount m_MSAASampleCount;
        SamplerFilter m_SamplerFilter;
        SamplerAddressMode m_SamplerAddressMode;
        bool m_IsAnisotropyEnabled;

        bool m_IsSwapchainOwned;
    };

}
