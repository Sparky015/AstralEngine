//
// Created by Andrew Fagan on 11/30/24.
//

#pragma once

#include "Asset/Asset.h"
#include "Core/Math/Math.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"
#include "Renderer/RHI/Common/ImageFormats.h"
#include "Renderer/RHI/Common/ImageLayouts.h"
#include "Renderer/RHI/Common/ImageSubresourceRange.h"
#include "Renderer/RHI/Common/ImageUsageFlags.h"
#include "Renderer/RHI/Common/SampleCount.h"

#include "imgui/imgui.h"

#include <string>
#include <filesystem>

namespace Astral {

    /**
    * @brief Information to create a texture from
    */
    struct TextureCreateInfo
    {
        ImageFormat Format;
        ImageLayout Layout;
        ImageUsageFlags UsageFlags;
        UVec2 Dimensions;
        uint8* ImageData = nullptr;
        uint32 ImageDataLength = 0;
        uint32 LayerCount = 1;
        uint32 MipMapCount = 1;
        bool GenerateMipMaps = false;
        SampleCount MSAASampleCount = SampleCount::SAMPLE_1_BIT;
    };

    /**
     * @brief RHI Texture Object
     */
    class Texture : public Asset
    {
    public:
        ~Texture() override = default;

        /**
         * @brief  Gets the image width
         * @return The image width
         */
        virtual int GetWidth() = 0;

        /**
         * @brief  Gets the image height
         * @return The image height
         */
        virtual int GetHeight() = 0;

        /**
         * @brief  Gets the image dimensions
         * @return The image dimensions
         */
        virtual UVec2 GetDimensions() = 0;

        /**
         * @brief  Gets the image aspects of the texture
         * @return The image aspects of the texture
         */
        virtual ImageAspectFlags GetImageAspect() = 0;

        /**
         * @brief  Gets the image layout of the texture
         * @return The image layout of the texture
         */
        virtual ImageLayout GetLayout() = 0;

        /**
         * @brief Updates the layout of the texture to the specified layout
         * @param imageLayout The image layout to update the texture to
         */
        virtual void UpdateLayout(ImageLayout imageLayout) = 0;

        /**
         * @brief  Gets the image format of the texture
         * @return The image format of the texture
         */
        virtual ImageFormat GetFormat() = 0;

        /**
         * @brief  Gets the number of layers that the texture has
         * @return The number of layers that the texture has
         */
        virtual uint32 GetNumLayers() = 0;

        /**
         * @brief  Gets the number of mipmaps that the texture has
         * @return The number of mipmaps that the texture has
         */
        virtual uint32 GetNumMipLevels() = 0;

        /**
         * @brief  Gets the image sampler of the texture
         * @return The image sampler of the texture
         * @note   The void pointer maps to the native sampler handle of the selected renderer api backend
         */
        virtual void* GetNativeSampler() = 0;

        /**
         * @brief  Gets the native image handle of the texture
         * @return The native image handle of the texture
         * @note   The void pointer maps to the native image handle of the selected renderer api backend
         */
        virtual void* GetNativeImage() = 0;

        /**
         * @brief  Gets the native image view handle of the texture
         * @return The native image view of the texture
         * @note   The void pointer maps to the native image view handle of the selected renderer api backend
         */
        virtual void* GetNativeImageView() = 0;

        /**
         * @brief  Gets the native image view of a specific layer in the texture
         * @return The native image view of a specific layer in the texture
         * @note   The void pointer maps to the native vulkan image view handle (VkImageView)
         */
        virtual void* GetNativeLayerImageView(uint32 layer) = 0;

        /**
         * @brief  Gets the native image view of a specific mip level in the texture
         * @return The native image view of a specific mip level in the texture
         * @note   The void pointer maps to the native vulkan image view handle (VkImageView)
         */
        virtual void* GetNativeMipMapImageView(uint32 mipLevel) = 0;

        /**
         * @brief  Gets the native image view handle of the texture at a specific layer and mip level
         * @param layer The layer to view the image at
         * @param mipLevel The mip level to view the image at
         * @return The native image view of the texture
         * @note   The void pointer maps to the native image view handle of the selected renderer api backend
         */
        virtual void* GetNativeImageView(uint32 layer, uint32 mipLevel) = 0;

        /**
         * @brief  Calculates the number of mip map levels needed for a texture of the given dimensions
         * @param  width The width of the base texture
         * @param  height THe height of the base texture
         * @return The number of mip map levels needed for a texture of the given dimensions
         */
        static uint32 CalculateMipMapLevels(uint32 width, uint32 height);

        /**
         * @brief  Calculates the size of a mip map level based on the given dimensions
         * @param  imageFormat The format of the texture
         * @param  width The width of the base texture
         * @param  height The height of the base texture
         * @param  depth The height of the base texture
         * @param  numLayers The number of layers the texture has
         * @return The size of a mip map level based on the given dimensions
         */
        static uint32 CalculateMipMapLevelSize(ImageFormat imageFormat, uint32 width, uint32 height, uint32 depth, uint32 numLayers);

        /**
         * @brief  Creates a texture from a file path
         * @return The texture handle
         */
        static GraphicsRef<Texture> CreateTexture(const std::filesystem::path& filePath);

        /**
         * @brief  Creates a cubemap from the given texture create info
         * @return The cubemap texture handle
         */
        static GraphicsRef<Texture> CreateCubemap(const TextureCreateInfo& textureCreateInfo);

        /**
         * @brief  Creates a cubemap from the given data
         * @return The cubemap texture handle
         */
        static GraphicsRef<Texture> CreateCubemap(void* data, uint32 width, uint32 height, ImageFormat imageFormat, ImageUsageFlags imageUsageFlags);

        /**
         * @brief  Creates a lut from the given data
         * @return The lut texture handle
         */
        static GraphicsRef<Texture> Create3DTexture(void* data, uint32 width, uint32 height, ImageFormat imageFormat);

        /**
         * @brief  Creates a lut from the given data
         * @return The lut texture handle
         */
        static GraphicsRef<Texture> Create1DTexture(void* data, uint32 length, ImageFormat imageFormat);

        /**
         * @brief  Creates a texture from the image data
         * @return The texture handle
         */
        static GraphicsRef<Texture> CreateTexture(const TextureCreateInfo& textureCreateInfo);

        AssetType GetAssetType() override { return Texture::GetStaticAssetType(); }
        static AssetType GetStaticAssetType() { return AssetType::Texture; }
    };

    using TextureHandle = GraphicsRef<Texture>;

} // Renderer
