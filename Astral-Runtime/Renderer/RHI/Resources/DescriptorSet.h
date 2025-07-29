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
        ALL
    };

    enum class Descriptor
    {
        STORAGE_BUFFER,
        UNIFORM_BUFFER,
        IMAGE_SAMPLER,
        SUBPASS_INPUT_ATTACHMENT
    };

    struct DescriptorSetLayout
    {
        std::vector<Descriptor> Descriptors;
    };

    class DescriptorSet
    {
    public:
        virtual ~DescriptorSet() = default;

        virtual void BeginBuildingSet() = 0;
        virtual void AddDescriptorStorageBuffer(BufferHandle bufferHandle, ShaderStage shaderStage) = 0;
        virtual void AddDescriptorUniformBuffer(BufferHandle bufferHandle, ShaderStage shaderStage) = 0;
        virtual void AddDescriptorImageSampler(TextureHandle textureHandle, ShaderStage bindStage) = 0;
        virtual void AddDescriptorSubpassInputAttachment(TextureHandle textureHandle, ShaderStage bindStage) = 0;
        virtual void EndBuildingSet() = 0;

        virtual void UpdateStorageBufferBinding(uint32 binding, BufferHandle bufferHandle) = 0;
        virtual void UpdateUniformBinding(uint32 binding, BufferHandle bufferHandle) = 0;
        virtual void UpdateImageSamplerBinding(uint32 binding, TextureHandle textureHandle) = 0;
        virtual void UpdateSubpassInputAttachmentBinding(uint32 binding, TextureHandle textureHandle) = 0;

        virtual const DescriptorSetLayout& GetDescriptorSetLayout() = 0;

        virtual void* GetNativeLayout() = 0;
        virtual void* GetNativeHandle() = 0;

        static GraphicsRef<DescriptorSet> CreateDescriptorSet();

    };

    using DescriptorSetHandle = GraphicsRef<DescriptorSet>;

}
