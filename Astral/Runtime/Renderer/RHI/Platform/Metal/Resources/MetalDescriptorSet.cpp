/**
* @file MetalDescriptorSet.cpp
* @author Andrew Fagan
* @date 5/24/26
*/

#include "MetalDescriptorSet.h"

namespace Astral {

    MetalDescriptorSet::MetalDescriptorSet(const MetalDescriptorSetDesc& descriptorSetDesc)
    {

    }


    MetalDescriptorSet::~MetalDescriptorSet()
    {

    }


    void MetalDescriptorSet::BeginBuildingSet()
    {

    }


    void MetalDescriptorSet::AddDescriptorStorageBuffer(BufferHandle bufferHandle, ShaderStage bindStage)
    {

    }


    void MetalDescriptorSet::AddDescriptorUniformBuffer(BufferHandle bufferHandle, ShaderStage bindStage)
    {

    }


    void MetalDescriptorSet::AddDescriptorImageSampler(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout)
    {

    }


    void MetalDescriptorSet::AddDescriptorStorageImage(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout)
    {

    }


    void MetalDescriptorSet::AddDescriptorSubpassInputAttachment(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout)
    {

    }


    void MetalDescriptorSet::EndBuildingSet()
    {

    }


    void MetalDescriptorSet::UpdateStorageBufferBinding(uint32 binding, BufferHandle newBufferHandle)
    {

    }


    void MetalDescriptorSet::UpdateUniformBinding(uint32 binding, BufferHandle newBufferHandle)
    {

    }


    void MetalDescriptorSet::UpdateImageSamplerBinding(uint32 binding, TextureHandle newTextureHandle, ImageLayout imageLayout)
    {

    }


    void MetalDescriptorSet::UpdateImageSamplerBinding(uint32 binding, TextureHandle newTextureHandle, uint32 mipLevel, ImageLayout imageLayout)
    {

    }


    void MetalDescriptorSet::UpdateStorageImageBinding(uint32 binding, TextureHandle newTextureHandle, uint32 mipLevel, ImageLayout imageLayout)
    {

    }


    void MetalDescriptorSet::UpdateSubpassInputAttachmentBinding(uint32 binding, TextureHandle newTextureHandle)
    {

    }


    BufferHandle MetalDescriptorSet::GetStorageBuffer(uint32 binding)
    {

    }


    BufferHandle MetalDescriptorSet::GetUniformBuffer(uint32 binding)
    {

    }


    TextureHandle MetalDescriptorSet::GetImageSampler(uint32 binding)
    {

    }


    TextureHandle MetalDescriptorSet::GetStorageImage(uint32 binding)
    {

    }


    TextureHandle MetalDescriptorSet::GetSubpassInputAttachment(uint32 binding)
    {

    }


    const DescriptorSetLayout& MetalDescriptorSet::GetDescriptorSetLayout()
    {

    }


    void* MetalDescriptorSet::GetNativeLayout()
    {

    }


    void* MetalDescriptorSet::GetNativeHandle()
    {

    }


    void MetalDescriptorSet::Invalidate()
    {

    }


    void MetalDescriptorSet::CreateArgumentTable()
    {

    }


    void MetalDescriptorSet::DestroyArgumentTable()
    {

    }


    void MetalDescriptorSet::UpdateDescriptorSets()
    {

    }

}
