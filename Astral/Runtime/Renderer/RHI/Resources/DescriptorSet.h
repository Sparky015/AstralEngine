/**
* @file DescriptorSet.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Buffer.h"
#include "Texture.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"
#include "Renderer/RHI/Common/ShaderStage.h"

namespace Astral {

    /**
     * @brief Defines descriptors to use in descriptor sets
     */
    enum class Descriptor
    {
        STORAGE_BUFFER,
        UNIFORM_BUFFER,
        IMAGE_SAMPLER,
        STORAGE_IMAGE
    };

    /**
     * @brief Defines the descriptor layout of a descriptor set
     */
    struct DescriptorSetLayout
    {
        std::vector<Descriptor> Descriptors;
        bool operator==(const DescriptorSetLayout&) const;
    };

    /**
     * @brief Defines the descriptor set RHI object
     */
    class DescriptorSet
    {
    public:
        virtual ~DescriptorSet() = default;

        /**
         * @brief Indicates the start of building a descriptor set
         * @post  @ref EndBuildingSet is called after all descriptors are added to create the descriptor set
         * @note Call AddDescriptor* methods to add descriptors after calling BeginBuildingSet function. The call order
         *       of AddDescriptor* calls maps directly to the binding order of the descriptors in the set.
         */
        virtual void BeginBuildingSet() = 0;

        /**
         * @brief Adds a storage buffer descriptor to the descriptor set
         * @param bufferHandle The buffer to bind in the descriptor set
         * @param shaderStage The shader stage to make the binding active for
         */
        virtual void AddDescriptorStorageBuffer(BufferHandle bufferHandle, ShaderStage shaderStage) = 0;

        /**
         * @brief Adds a uniform buffer descriptor to the descriptor set
         * @param bufferHandle The buffer to bind in the descriptor set
         * @param shaderStage The shader stage to make the binding active for
         */
        virtual void AddDescriptorUniformBuffer(BufferHandle bufferHandle, ShaderStage shaderStage) = 0;

        /**
         * @brief Adds a sampled image descriptor to the descriptor set
         * @param textureHandle The combined image-sampler to bind in the descriptor set
         * @param bindStage The shader stage to make the binding active for
         * @param imageLayout The image layout that the sampled image will be in when accessed through the descriptor set
         */
        virtual void AddDescriptorImageSampler(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) = 0;

        /**
         * @brief Adds a storage image descriptor to the descriptor set
         * @param textureHandle The storage image to bind in the descriptor set
         * @param bindStage The shader stage to make the binding active for
         * @param imageLayout The image layout that the storage image will be in when accessed through the descriptor set
         */
        virtual void AddDescriptorStorageImage(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) = 0;

        /**
         * @brief Indicates the end of building a descriptor set
         * @pre  @ref BeginBuildingSet is called before all descriptors are added to create the descriptor set
         */
        virtual void EndBuildingSet() = 0;

        /**
         * @brief Updates the descriptor set with a new storage buffer at the given binding
         * @param binding The binding to update
         * @param bufferHandle The storage buffer to bind in the descriptor set
         */
        virtual void UpdateStorageBufferBinding(uint32 binding, BufferHandle bufferHandle) = 0;

        /**
         * @brief Updates the descriptor set with a new uniform buffer at the given binding
         * @param binding The binding to update
         * @param bufferHandle The uniform buffer to bind in the descriptor set
         */
        virtual void UpdateUniformBinding(uint32 binding, BufferHandle bufferHandle) = 0;

        /**
         * @brief Updates the descriptor set with a new combined image-sampler at the given binding
         * @param binding The binding to update
         * @param textureHandle The new combined image-sampler to bind in the descriptor set
         */
        virtual void UpdateImageSamplerBinding(uint32 binding, TextureHandle textureHandle, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) = 0;

        /**
         * @brief Updates the descriptor set with a new combined image-sampler at the given binding
         * @param binding The binding to update
         * @param newTextureHandle The new combined image-sampler to bind in the descriptor set
         * @param mipLevel The mip level of the new texture to use
         * @param imageLayout The image layout that the combined image-sampler will be in when accessed through the descriptor set
         */
        virtual void UpdateImageSamplerBinding(uint32 binding, TextureHandle newTextureHandle, uint32 mipLevel, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) = 0;

        /**
         * @brief Updates the descriptor set with a new storage image at the given binding
         * @param binding The binding to update
         * @param newTextureHandle The new storage image to bind in the descriptor set
         * @param mipLevel The mip level of the new texture to use
         * @param imageLayout The image layout that the storage image will be in when accessed through the descriptor set
         */
        virtual void UpdateStorageImageBinding(uint32 binding, TextureHandle newTextureHandle, uint32 mipLevel, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) = 0;

        /**
         * @brief Gets the storage buffer at the given binding
         * @param binding The binding where the selected storage buffer is
         * @return The storage buffer at the given binding
         */
        virtual BufferHandle GetStorageBuffer(uint32 binding) = 0;

        /**
         * @brief Gets the uniform buffer at the given binding
         * @param binding The binding where the selected uniform buffer is
         * @return The uniform buffer at the given binding
         */
        virtual BufferHandle GetUniformBuffer(uint32 binding) = 0;

        /**
         * @brief Gets the combined image-sampler at the given binding
         * @param binding The binding where the selected combined image-sampler is
         * @return The combined image-sampler at the given binding
         */
        virtual TextureHandle GetImageSampler(uint32 binding) = 0;

        /**
         * @brief Gets the storage image at the given binding
         * @param binding The binding where the selected storage image is
         * @return The storage image at the given binding
         */
        virtual TextureHandle GetStorageImage(uint32 binding) = 0;

        /**
         * @brief Gets the descriptor layout of the descriptor set
         * @return The descriptor layout of the descriptor set
         */
        virtual const DescriptorSetLayout& GetDescriptorSetLayout() = 0;

        /**
         * @brief Gets the native API descriptor layout object of the descriptor set
         * @return The native API descriptor layout object of the descriptor set
         */
        virtual void* GetNativeLayout() = 0;

        /**
         * @brief Gets the native API descriptor set object
         * @return The native API descriptor set object
         */
        virtual void* GetNativeHandle() = 0;

        /**
         * @brief Creates a descriptor set instance
         * @return A descriptor set instance
         */
        static GraphicsRef<DescriptorSet> CreateDescriptorSet();

    };

    using DescriptorSetHandle = GraphicsRef<DescriptorSet>;

}
