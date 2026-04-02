/**
* @file DescriptorSet.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Buffer.h"
#include "Sampler.h"
#include "Texture.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    enum class ShaderStage : uint8
    {
        VERTEX,
        FRAGMENT,
        COMPUTE,
        ALL
    };

    enum class Descriptor
    {
        STORAGE_BUFFER,
        UNIFORM_BUFFER,
        IMAGE_SAMPLER,
        SUBPASS_INPUT_ATTACHMENT,
        STORAGE_IMAGE
    };

    struct DescriptorSetLayout
    {
        std::vector<Descriptor> Descriptors;
        bool operator==(const DescriptorSetLayout&) const;
    };

    class DescriptorSet
    {
    public:
        virtual ~DescriptorSet() = default;

        virtual void BeginBuildingSet() = 0;
        virtual void AddDescriptorStorageBuffer(BufferHandle bufferHandle, ShaderStage shaderStage) = 0;
        virtual void AddDescriptorUniformBuffer(BufferHandle bufferHandle, ShaderStage shaderStage) = 0;
        virtual void AddDescriptorImageSampler(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) = 0;
        virtual void AddDescriptorStorageImage(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) = 0;
        virtual void AddDescriptorSubpassInputAttachment(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) = 0;
        virtual void EndBuildingSet() = 0;

        virtual void UpdateStorageBufferBinding(uint32 binding, BufferHandle bufferHandle) = 0;
        virtual void UpdateUniformBinding(uint32 binding, BufferHandle bufferHandle) = 0;
        virtual void UpdateImageSamplerBinding(uint32 binding, TextureHandle textureHandle, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) = 0;
        virtual void UpdateImageSamplerBinding(uint32 binding, TextureHandle newTextureHandle, uint32 mipLevel, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) = 0;
        virtual void UpdateStorageImageBinding(uint32 binding, TextureHandle newTextureHandle, uint32 mipLevel, ImageLayout imageLayout = ImageLayout::SHADER_READ_ONLY_OPTIMAL) = 0;
        virtual void UpdateSubpassInputAttachmentBinding(uint32 binding, TextureHandle textureHandle) = 0;

        virtual BufferHandle GetStorageBuffer(uint32 binding) = 0;
        virtual BufferHandle GetUniformBuffer(uint32 binding) = 0;
        virtual TextureHandle GetImageSampler(uint32 binding) = 0;
        virtual TextureHandle GetStorageImage(uint32 binding) = 0;
        virtual TextureHandle GetSubpassInputAttachment(uint32 binding) = 0;


        virtual const DescriptorSetLayout& GetDescriptorSetLayout() = 0;

        virtual void* GetNativeLayout() = 0;
        virtual void* GetNativeHandle() = 0;

        static GraphicsRef<DescriptorSet> CreateDescriptorSet();

    };

    using DescriptorSetHandle = GraphicsRef<DescriptorSet>;

}
