/**
* @file MetalDescriptorSet.h
* @author Andrew Fagan
* @date 5/24/26
*/

#pragma once

#include "Renderer/RHI/Resources/DescriptorSet.h"

#include "Metal/MTLDevice.hpp"

namespace Astral {

    /**
    * @brief Information to create a Metal descriptor set from
    */
    struct MetalDescriptorSetDesc
    {
        MTL::Device* Device;
    };

    /**
     * @brief Defines a wrapper of the Metal Argument Table object with extra convenience functions
     */
    class MetalDescriptorSet : public DescriptorSet
    {
    public:

        MetalDescriptorSet(const MetalDescriptorSetDesc& descriptorSetDesc);
        ~MetalDescriptorSet() override;

        /**
         * @brief Indicates the start of building a descriptor set
         * @post  @ref EndBuildingSet is called after all descriptors are added to create the descriptor set
         * @note Call AddDescriptor* methods to add descriptors after calling BeginBuildingSet function. The call order
         *       of AddDescriptor* calls maps directly to the binding order of the descriptors in the set.
         */
        void BeginBuildingSet() override;

        /**
         * @brief Adds a storage buffer descriptor to the descriptor set
         * @param bufferHandle The buffer to bind in the descriptor set
         * @param bindStage The shader stage to make the binding active for
         */
        void AddDescriptorStorageBuffer(BufferHandle bufferHandle, ShaderStage bindStage) override;

        /**
         * @brief Adds a uniform buffer descriptor to the descriptor set
         * @param bufferHandle The buffer to bind in the descriptor set
         * @param bindStage The shader stage to make the binding active for
         */
        void AddDescriptorUniformBuffer(BufferHandle bufferHandle, ShaderStage bindStage) override;

        /**
         * @brief Adds a sampled image descriptor to the descriptor set
         * @param textureHandle The combined image-sampler to bind in the descriptor set
         * @param bindStage The shader stage to make the binding active for
         * @param imageLayout The image layout that the sampled image will be in when accessed through the descriptor set
         */
        void AddDescriptorImageSampler(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) override;

        /**
         * @brief Adds a storage image descriptor to the descriptor set
         * @param textureHandle The storage image to bind in the descriptor set
         * @param bindStage The shader stage to make the binding active for
         * @param imageLayout The image layout that the storage image will be in when accessed through the descriptor set
         */
        void AddDescriptorStorageImage(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) override;

        /**
         * @brief Indicates the end of building a descriptor set
         * @pre  @ref BeginBuildingSet is called before all descriptors are added to create the descriptor set
         */
        void EndBuildingSet() override;

        /**
         * @brief Updates the descriptor set with a new storage buffer at the given binding
         * @param binding The binding to update
         * @param newBufferHandle The storage buffer to bind in the descriptor set
         */
        void UpdateStorageBufferBinding(uint32 binding, BufferHandle newBufferHandle) override;

        /**
         * @brief Updates the descriptor set with a new uniform buffer at the given binding
         * @param binding The binding to update
         * @param newBufferHandle The uniform buffer to bind in the descriptor set
         */
        void UpdateUniformBinding(uint32 binding, BufferHandle newBufferHandle) override;

        /**
         * @brief Updates the descriptor set with a new combined image-sampler at the given binding
         * @param binding The binding to update
         * @param newTextureHandle The new combined image-sampler to bind in the descriptor set
         */
        void UpdateImageSamplerBinding(uint32 binding, TextureHandle newTextureHandle, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) override;

        /**
         * @brief Updates the descriptor set with a new combined image-sampler at the given binding
         * @param binding The binding to update
         * @param newTextureHandle The new combined image-sampler to bind in the descriptor set
         * @param mipLevel The mip level of the new texture to use
         * @param imageLayout The image layout that the combined image-sampler will be in when accessed through the descriptor set
         */
        void UpdateImageSamplerBinding(uint32 binding, TextureHandle newTextureHandle, uint32 mipLevel, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) override;

        /**
         * @brief Updates the descriptor set with a new storage image at the given binding
         * @param binding The binding to update
         * @param newTextureHandle The new storage image to bind in the descriptor set
         * @param mipLevel The mip level of the new texture to use
         * @param imageLayout The image layout that the storage image will be in when accessed through the descriptor set
         */
        void UpdateStorageImageBinding(uint32 binding, TextureHandle newTextureHandle, uint32 mipLevel, ImageLayout imageLayout) override;

        /**
         * @brief Gets the storage buffer at the given binding
         * @param binding The binding where the selected storage buffer is
         * @return The storage buffer at the given binding
         */
        BufferHandle GetStorageBuffer(uint32 binding) override;

        /**
         * @brief Gets the uniform buffer at the given binding
         * @param binding The binding where the selected uniform buffer is
         * @return The uniform buffer at the given binding
         */
        BufferHandle GetUniformBuffer(uint32 binding) override;

        /**
         * @brief Gets the combined image-sampler at the given binding
         * @param binding The binding where the selected combined image-sampler is
         * @return The combined image-sampler at the given binding
         */
        TextureHandle GetImageSampler(uint32 binding) override;

        /**
         * @brief Gets the storage image at the given binding
         * @param binding The binding where the selected storage image is
         * @return The storage image at the given binding
         */
        TextureHandle GetStorageImage(uint32 binding) override;

        /**
         * @brief Gets the descriptor layout of the descriptor set
         * @return The descriptor layout of the descriptor set
         */
        const DescriptorSetLayout& GetDescriptorSetLayout() override;

        /**
         * @brief Gets the native API descriptor layout object of the descriptor set
         * @return The native API descriptor layout object of the descriptor set
         */
        void* GetNativeLayout() override;

        /**
         * @brief Gets the native API descriptor set object (MTL4::ArgumentTable)
         * @return The native API descriptor set object (MTL4::ArgumentTable)
         */
        void* GetNativeHandle() override;

        /**
         * @brief Cleans up an existing Metal Argument Table object if applicable
         */
        void Invalidate();

    private:

        /**
         * @brief Creates the Metal Argument Table based on the submitted descriptor definitions
         * @pre @ref EndBuildingSet is called to indicate that the definition of the descriptor set is complete
         */
        void CreateArgumentTable();

        /**
         * @brief Releases the Metal Argument Table
         */
        void ReleaseArgumentTable();

        /**
         * @brief Updates the Metal Argument Table based on the current descriptor definitions
         */
        void UpdateDescriptorSets();

        /**
         * @brief Adds texture resource to global residency set
         */
        void AddResourceToResidencySet(const TextureHandle& texture);

        /**
         * @brief Adds buffer resource to global residency set
         */
        void AddResourceToResidencySet(const BufferHandle& buffer);


        MTL::Device* m_Device;
        BufferHandle m_ArgumentBuffer;

        DescriptorSetLayout m_DescriptorSetLayout;
        uint32 m_NumLogicalBindings;

        std::vector<BufferHandle> m_Buffers;
        std::vector<TextureHandle> m_Textures;

        std::vector<uint32> m_LogicalToPhysicalBindingMap;
        uint32 m_NumPhysicalBindings;
    };

}