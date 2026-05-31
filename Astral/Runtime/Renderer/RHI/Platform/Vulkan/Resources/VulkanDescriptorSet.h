/**
* @file VulkanDescriptorSet.h
* @author Andrew Fagan
* @date 5/18/2025
*/


#pragma once

#include "Renderer/RHI/Resources/DescriptorSet.h"

#include <vulkan/vulkan_core.h>
#include <queue>

namespace Astral {

    /**
    * @brief Information to create a Vulkan descriptor set from
    */
    struct VulkanDescriptorSetDesc
    {
        VkDevice Device;
    };

    /**
     * @brief Defines a wrapper of the Vulkan descriptor set object with extra convenience functions
     */
    class VulkanDescriptorSet : public DescriptorSet
    {
    public:
        VulkanDescriptorSet(const VulkanDescriptorSetDesc& desc);
        ~VulkanDescriptorSet() override;

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
         * @brief Adds a subpass input image descriptor to the descriptor set
         * @param textureHandle The subpass input image to bind in the descriptor set
         * @param bindStage The shader stage to make the binding active for
         * @param imageLayout The image layout that the subpass input image will be in when accessed through the descriptor set
         */
        void AddDescriptorSubpassInputAttachment(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) override;

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
        * @brief Updates the descriptor set with a new subpass input image descriptor at the given binding
        * @param binding The binding to update
        * @param newTextureHandle The new subpass input image to bind in the descriptor set
        */
        void UpdateSubpassInputAttachmentBinding(uint32 binding, TextureHandle newTextureHandle) override;

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
         * @brief Gets the subpass input image at the given binding
         * @param binding The binding where the selected subpass input image is
         * @return The subpass input image at the given binding
         */
        TextureHandle GetSubpassInputAttachment(uint32 binding) override;

        /**
         * @brief Gets the descriptor layout of the descriptor set
         * @return The descriptor layout of the descriptor set
         */
        const DescriptorSetLayout& GetDescriptorSetLayout() override { return m_DescriptorSetLayout; }

        /**
         * @brief Gets the native API descriptor layout object of the descriptor set
         * @return The native API descriptor layout object of the descriptor set
         */
        void* GetNativeLayout() override { return m_VkDescriptorSetLayout; }

        /**
         * @brief Gets the native API descriptor set object (VkDescriptorSet)
         * @return The native API descriptor set object (VkDescriptorSet)
         */
        void* GetNativeHandle() override { return m_DescriptorSet; }

        /**
        * @brief Cleans up an existing Metal Argument Table object if applicable
        */
        void Invalidate();

    private:

        /**
         * @brief Creates a descriptor pool of size one fitted to this descriptor set
         */
        void CreateDescriptorPool();

        /**
         * @brief Destroys the descriptor pool
         */
        void DestroyDescriptorPool();

        /**
         * @brief Creates the Vulkan descriptor set based on the submitted descriptor definitions
         * @pre @ref EndBuildingSet is called to indicate that the definition of the descriptor set is complete
         */
        void CreateDescriptorSetLayout();

        /**
         * @brief Destroys the Vulkan descriptor set
         */
        void DestroyDescriptorSetLayout();

        /**
         * @brief Allocates the descriptor set from the descriptor pool
         */
        void AllocateDescriptorSets();

        /**
         * @brief Frees the descriptor set from the descriptor pool
         */
        void FreeDescriptorSet();

        /**
         * @brief Updates the descriptor set based on the current descriptor definitions
         */
        void UpdateDescriptorSets();


        VkDevice m_Device;

        std::vector<ImageLayout> m_ImageDescriptorLayouts;
        std::vector<VkDescriptorSetLayoutBinding> m_DescriptorSetLayoutBindings;
        uint32 m_NumberOfBindings;

        std::vector<BufferHandle> m_Buffers;
        std::vector<TextureHandle> m_Textures;

        VkDescriptorPool m_DescriptorPool;
        VkDescriptorSet m_DescriptorSet;
        VkDescriptorSetLayout m_VkDescriptorSetLayout;

        DescriptorSetLayout m_DescriptorSetLayout;
    };

}
