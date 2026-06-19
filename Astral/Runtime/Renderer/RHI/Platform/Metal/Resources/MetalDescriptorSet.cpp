/**
* @file MetalDescriptorSet.cpp
* @author Andrew Fagan
* @date 5/24/26
*/

#include "MetalDescriptorSet.h"

#include "Core/Utilities/Asserts.h"
#include "Metal/Metal.hpp"
#include "Metal/MTL4ArgumentTable.hpp"

namespace Astral {

    MetalDescriptorSet::MetalDescriptorSet(const MetalDescriptorSetDesc& descriptorSetDesc) :
        m_Device(descriptorSetDesc.Device),
        m_ArgumentTable(nullptr),
        m_DescriptorSetLayout(),
        m_NumberOfBindings(0),
        m_Buffers(),
        m_Textures()
    {

    }


    MetalDescriptorSet::~MetalDescriptorSet()
    {
        Invalidate();
    }


    void MetalDescriptorSet::BeginBuildingSet()
    {
        Invalidate();
    }


    void MetalDescriptorSet::AddDescriptorStorageBuffer(BufferHandle bufferHandle, ShaderStage bindStage)
    {
        m_NumberOfBindings++;
        m_Buffers.push_back(bufferHandle);
        m_DescriptorSetLayout.Descriptors.push_back(Descriptor::STORAGE_BUFFER);
    }


    void MetalDescriptorSet::AddDescriptorUniformBuffer(BufferHandle bufferHandle, ShaderStage bindStage)
    {
        m_NumberOfBindings++;
        m_Buffers.push_back(bufferHandle);
        m_DescriptorSetLayout.Descriptors.push_back(Descriptor::UNIFORM_BUFFER);
    }


    void MetalDescriptorSet::AddDescriptorImageSampler(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout)
    {
        m_NumberOfBindings++;
        m_Textures.push_back(textureHandle);
        m_DescriptorSetLayout.Descriptors.push_back(Descriptor::IMAGE_SAMPLER);
    }


    void MetalDescriptorSet::AddDescriptorStorageImage(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout)
    {
        m_NumberOfBindings++;
        m_Textures.push_back(textureHandle);
        m_DescriptorSetLayout.Descriptors.push_back(Descriptor::STORAGE_IMAGE);
    }

    void MetalDescriptorSet::EndBuildingSet()
    {
        CreateArgumentTable();
        UpdateDescriptorSets();
    }


    void MetalDescriptorSet::UpdateStorageBufferBinding(uint32 binding, BufferHandle newBufferHandle)
    {
        ASSERT(binding < m_DescriptorSetLayout.Descriptors.size(), "Specified binding is out of range of the existing descriptor set")
        Descriptor descriptorType = m_DescriptorSetLayout.Descriptors[binding];
        ASSERT(descriptorType == Descriptor::STORAGE_BUFFER, "Expected existing binding to be a storage buffer when updating a binding using UpdateStorageBufferBinding")

        // Finds the target binding buffer by counting the number of storage buffers or uniform buffers before the target binding
        uint32 bufferIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[binding];
            if (descriptorAtBinding == Descriptor::STORAGE_BUFFER || descriptorAtBinding == Descriptor::UNIFORM_BUFFER)
            {
                bufferIndex++;
            }
        }

        m_Buffers[bufferIndex] = newBufferHandle;

        MTL::Buffer* buffer = (MTL::Buffer*)newBufferHandle->GetNativeHandle();
        MTL::GPUAddress bufferGPUAddress = buffer->gpuAddress();
        m_ArgumentTable->setAddress(bufferGPUAddress, binding);
    }


    void MetalDescriptorSet::UpdateUniformBinding(uint32 binding, BufferHandle newBufferHandle)
    {
        ASSERT(binding < m_DescriptorSetLayout.Descriptors.size(), "Specified binding is out of range of the existing descriptor set")
        Descriptor descriptorType = m_DescriptorSetLayout.Descriptors[binding];
        ASSERT(descriptorType == Descriptor::UNIFORM_BUFFER, "Expected existing binding to be a uniform buffer when updating a binding using UpdateUniformBinding")

        // Finds the target binding buffer by counting the number of storage buffers or uniform buffers before the target binding
        uint32 bufferIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[binding];
            if (descriptorAtBinding == Descriptor::STORAGE_BUFFER || descriptorAtBinding == Descriptor::UNIFORM_BUFFER)
            {
                bufferIndex++;
            }
        }

        m_Buffers[bufferIndex] = newBufferHandle;

        MTL::Buffer* buffer = (MTL::Buffer*)newBufferHandle->GetNativeHandle();
        MTL::GPUAddress bufferGPUAddress = buffer->gpuAddress();
        m_ArgumentTable->setAddress(bufferGPUAddress, binding);
    }


    void MetalDescriptorSet::UpdateImageSamplerBinding(uint32 binding, TextureHandle newTextureHandle, ImageLayout imageLayout)
    {
        ASSERT(binding < m_DescriptorSetLayout.Descriptors.size(), "Specified binding is out of range of the existing descriptor set")
        Descriptor descriptorType = m_DescriptorSetLayout.Descriptors[binding];
        ASSERT(descriptorType == Descriptor::IMAGE_SAMPLER, "Expected existing binding to be a image sampler when updating a binding using UpdateImageSamplerBinding")

        // Finds the target binding buffer by counting the number of image samplers and storage images before the target binding
        uint32 textureIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[binding];
            if (descriptorAtBinding == Descriptor::IMAGE_SAMPLER || descriptorAtBinding == Descriptor::STORAGE_IMAGE)
            {
                textureIndex++;
            }
        }

        m_Textures[textureIndex] = newTextureHandle;

        MTL::Texture* texture = (MTL::Texture*)newTextureHandle->GetNativeImageView();
        MTL::SamplerState* sampler = (MTL::SamplerState*)newTextureHandle->GetNativeSampler();

        MTL::ResourceID textureResourceID = texture->gpuResourceID();
        MTL::ResourceID samplerResourceID = sampler->gpuResourceID();
        m_ArgumentTable->setResource(textureResourceID, binding);
        m_ArgumentTable->setResource(samplerResourceID, binding);
    }


    void MetalDescriptorSet::UpdateImageSamplerBinding(uint32 binding, TextureHandle newTextureHandle, uint32 mipLevel, ImageLayout imageLayout)
    {
        ASSERT(binding < m_DescriptorSetLayout.Descriptors.size(), "Specified binding is out of range of the existing descriptor set")
        Descriptor descriptorType = m_DescriptorSetLayout.Descriptors[binding];
        ASSERT(descriptorType == Descriptor::IMAGE_SAMPLER, "Expected existing binding to be a image sampler when updating a binding using UpdateImageSamplerBinding")

        // Finds the target binding buffer by counting the number of image samplers and storage images before the target binding
        uint32 textureIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[binding];
            if (descriptorAtBinding == Descriptor::IMAGE_SAMPLER || descriptorAtBinding == Descriptor::STORAGE_IMAGE)
            {
                textureIndex++;
            }
        }

        m_Textures[textureIndex] = newTextureHandle;

        MTL::Texture* texture = (MTL::Texture*)newTextureHandle->GetNativeImageView();
        MTL::SamplerState* sampler = (MTL::SamplerState*)newTextureHandle->GetNativeSampler();

        MTL::ResourceID textureResourceID = texture->gpuResourceID();
        MTL::ResourceID samplerResourceID = sampler->gpuResourceID();
        m_ArgumentTable->setResource(textureResourceID, binding);
        m_ArgumentTable->setResource(samplerResourceID, binding);
    }


    void MetalDescriptorSet::UpdateStorageImageBinding(uint32 binding, TextureHandle newTextureHandle, uint32 mipLevel, ImageLayout imageLayout)
    {
        ASSERT(binding < m_DescriptorSetLayout.Descriptors.size(), "Specified binding is out of range of the existing descriptor set")
         Descriptor descriptorType = m_DescriptorSetLayout.Descriptors[binding];
        ASSERT(descriptorType == Descriptor::STORAGE_IMAGE, "Expected existing binding to be a storage image when updating a binding using UpdateStorageImageBinding")

        // Finds the target binding buffer by counting the number of image samplers and storage images before the target binding
        uint32 textureIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[binding];
            if (descriptorAtBinding == Descriptor::IMAGE_SAMPLER || descriptorAtBinding == Descriptor::STORAGE_IMAGE)
            {
                textureIndex++;
            }
        }

        m_Textures[textureIndex] = newTextureHandle;

        MTL::Texture* texture = (MTL::Texture*)newTextureHandle->GetNativeImageView();
        MTL::SamplerState* sampler = (MTL::SamplerState*)newTextureHandle->GetNativeSampler();

        MTL::ResourceID textureResourceID = texture->gpuResourceID();
        MTL::ResourceID samplerResourceID = sampler->gpuResourceID();
        m_ArgumentTable->setResource(textureResourceID, binding);
        m_ArgumentTable->setResource(samplerResourceID, binding);
    }


    BufferHandle MetalDescriptorSet::GetStorageBuffer(uint32 binding)
    {
        ASSERT(binding < m_DescriptorSetLayout.Descriptors.size(), "Specified binding is out of range of the existing descriptor set")
        Descriptor descriptorType = m_DescriptorSetLayout.Descriptors[binding];
        ASSERT(descriptorType == Descriptor::STORAGE_BUFFER, "Expected existing binding to be a storage buffer when updating a binding using GetStorageBuffer")

        // Finds the target binding buffer by counting the number of storage buffers or uniform buffers before the target binding
        uint32 bufferIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[binding];
            if (descriptorAtBinding == Descriptor::STORAGE_BUFFER || descriptorAtBinding == Descriptor::UNIFORM_BUFFER)
            {
                bufferIndex++;
            }
        }

        return m_Buffers[bufferIndex];
    }


    BufferHandle MetalDescriptorSet::GetUniformBuffer(uint32 binding)
    {
        ASSERT(binding < m_DescriptorSetLayout.Descriptors.size(), "Specified binding is out of range of the existing descriptor set")
        Descriptor descriptorType = m_DescriptorSetLayout.Descriptors[binding];
        ASSERT(descriptorType == Descriptor::UNIFORM_BUFFER, "Expected existing binding to be a uniform buffer when updating a binding using GetStorageBuffer")

        // Finds the target binding buffer by counting the number of storage buffers or uniform buffers before the target binding
        uint32 bufferIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[binding];
            if (descriptorAtBinding == Descriptor::STORAGE_BUFFER || descriptorAtBinding == Descriptor::UNIFORM_BUFFER)
            {
                bufferIndex++;
            }
        }

        return m_Buffers[bufferIndex];
    }


    TextureHandle MetalDescriptorSet::GetImageSampler(uint32 binding)
    {
        ASSERT(binding < m_DescriptorSetLayout.Descriptors.size(), "Specified binding is out of range of the existing descriptor set")
        Descriptor descriptorType = m_DescriptorSetLayout.Descriptors[binding];
        ASSERT(descriptorType == Descriptor::IMAGE_SAMPLER, "Expected existing binding to be a image sampler when updating a binding using UpdateImageSamplerBinding")

        // Finds the target binding buffer by counting the number of image samplers and storage images before the target binding
        uint32 textureIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[binding];
            if (descriptorAtBinding == Descriptor::IMAGE_SAMPLER || descriptorAtBinding == Descriptor::STORAGE_IMAGE)
            {
                textureIndex++;
            }
        }

        return m_Textures[textureIndex];
    }


    TextureHandle MetalDescriptorSet::GetStorageImage(uint32 binding)
    {
        ASSERT(binding < m_DescriptorSetLayout.Descriptors.size(), "Specified binding is out of range of the existing descriptor set")
         Descriptor descriptorType = m_DescriptorSetLayout.Descriptors[binding];
        ASSERT(descriptorType == Descriptor::STORAGE_IMAGE, "Expected existing binding to be a storage image when updating a binding using UpdateStorageImageBinding")

        // Finds the target binding buffer by counting the number of image samplers and storage images before the target binding
        uint32 textureIndex = 0;
        for (size_t i = 0; i < binding; i++)
        {
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[binding];
            if (descriptorAtBinding == Descriptor::IMAGE_SAMPLER || descriptorAtBinding == Descriptor::STORAGE_IMAGE)
            {
                textureIndex++;
            }
        }

        return m_Textures[textureIndex];
    }


    const DescriptorSetLayout& MetalDescriptorSet::GetDescriptorSetLayout()
    {
        return m_DescriptorSetLayout;
    }


    void* MetalDescriptorSet::GetNativeLayout()
    {
        return m_ArgumentTable;
    }


    void* MetalDescriptorSet::GetNativeHandle()
    {
        return m_ArgumentTable;
    }


    void MetalDescriptorSet::Invalidate()
    {
        ReleaseArgumentTable();
        m_DescriptorSetLayout.Descriptors.clear();
        m_Buffers.clear();
        m_Textures.clear();
        m_NumberOfBindings = 0;
    }


    void MetalDescriptorSet::CreateArgumentTable()
    {
        MTL4::ArgumentTableDescriptor* argumentTableDescriptor = MTL4::ArgumentTableDescriptor::alloc();
        NS::Error* error = nullptr;

        argumentTableDescriptor->setMaxBufferBindCount(m_Buffers.size());
        argumentTableDescriptor->setMaxSamplerStateBindCount(m_Textures.size());
        argumentTableDescriptor->setMaxTextureBindCount(m_Textures.size());

        m_ArgumentTable = m_Device->newArgumentTable(argumentTableDescriptor, &error);

        if (m_ArgumentTable == nullptr)
        {
            AE_ERROR("Argument table failed to be created! Error: " << error->localizedDescription()->utf8String());
            error->release();
        }
    }


    void MetalDescriptorSet::ReleaseArgumentTable()
    {
        if (m_ArgumentTable)
        {
            m_ArgumentTable->release();
            m_ArgumentTable = nullptr;
        }
    }


    void MetalDescriptorSet::UpdateDescriptorSets()
    {
        uint32 bufferIndex = 0;
        uint32 samplerIndex = 0;
        for (uint32 i = 0; i < m_DescriptorSetLayout.Descriptors.size(); i++)
        {
            Descriptor descriptorType = m_DescriptorSetLayout.Descriptors[i];

            if (descriptorType == Descriptor::UNIFORM_BUFFER || descriptorType == Descriptor::STORAGE_BUFFER)
            {
                BufferHandle bufferHandle = m_Buffers[bufferIndex];
                bufferIndex++;

                MTL::Buffer* buffer = (MTL::Buffer*)bufferHandle->GetNativeHandle();
                MTL::GPUAddress bufferGPUAddress = buffer->gpuAddress();
                m_ArgumentTable->setAddress(bufferGPUAddress, i);
            }
            else if (descriptorType == Descriptor::IMAGE_SAMPLER ||
                    descriptorType == Descriptor::STORAGE_IMAGE)
            {

                TextureHandle textureHandle = m_Textures[samplerIndex];
                samplerIndex++;

                MTL::Texture* texture = (MTL::Texture*)textureHandle->GetNativeImageView();
                MTL::SamplerState* sampler = (MTL::SamplerState*)textureHandle->GetNativeSampler();

                MTL::ResourceID textureResourceID = texture->gpuResourceID();
                MTL::ResourceID samplerResourceID = sampler->gpuResourceID();
                m_ArgumentTable->setResource(textureResourceID, i);
                m_ArgumentTable->setResource(samplerResourceID, i);
            }
        }
    }

}
