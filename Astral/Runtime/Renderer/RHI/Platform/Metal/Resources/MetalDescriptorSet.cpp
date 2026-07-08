/**
* @file MetalDescriptorSet.cpp
* @author Andrew Fagan
* @date 5/24/26
*/

#include "MetalDescriptorSet.h"

#include "Core/Utilities/Asserts.h"
#include "Metal/Metal.hpp"
#include "Metal/MTL4ArgumentTable.hpp"
#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Platform/Metal/MetalRendererContext.h"

namespace Astral {

    MetalDescriptorSet::MetalDescriptorSet(const MetalDescriptorSetDesc& descriptorSetDesc) :
        m_Device(descriptorSetDesc.Device),
        m_ArgumentBuffer(nullptr),
        m_DescriptorSetLayout(),
        m_NumLogicalBindings(0),
        m_Buffers(),
        m_Textures(),
        m_NumPhysicalBindings(0)
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
        m_NumLogicalBindings++;
        m_Buffers.push_back(bufferHandle);
        m_DescriptorSetLayout.Descriptors.push_back(Descriptor::STORAGE_BUFFER);

        // Update logical to physical binding map
        m_LogicalToPhysicalBindingMap.push_back(m_NumPhysicalBindings);
        m_NumPhysicalBindings += 1;

        AddResourceToResidencySet(bufferHandle);
    }


    void MetalDescriptorSet::AddDescriptorUniformBuffer(BufferHandle bufferHandle, ShaderStage bindStage)
    {
        m_NumLogicalBindings++;
        m_Buffers.push_back(bufferHandle);
        m_DescriptorSetLayout.Descriptors.push_back(Descriptor::UNIFORM_BUFFER);

        // Update logical to physical binding map
        m_LogicalToPhysicalBindingMap.push_back(m_NumPhysicalBindings);
        m_NumPhysicalBindings += 1;

        AddResourceToResidencySet(bufferHandle);
    }


    void MetalDescriptorSet::AddDescriptorImageSampler(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout)
    {
        m_NumLogicalBindings++;

        m_Textures.push_back(textureHandle);
        m_DescriptorSetLayout.Descriptors.push_back(Descriptor::IMAGE_SAMPLER);

        // Update logical to physical binding map
        m_LogicalToPhysicalBindingMap.push_back(m_NumPhysicalBindings);
        m_NumPhysicalBindings += 2; // For image binding and sampler binding

        AddResourceToResidencySet(textureHandle);
    }


    void MetalDescriptorSet::AddDescriptorStorageImage(TextureHandle textureHandle, ShaderStage bindStage, ImageLayout imageLayout)
    {
        m_NumLogicalBindings++;
        m_Textures.push_back(textureHandle);
        m_DescriptorSetLayout.Descriptors.push_back(Descriptor::STORAGE_IMAGE);

        // Update logical to physical binding map
        m_LogicalToPhysicalBindingMap.push_back(m_NumPhysicalBindings);
        m_NumPhysicalBindings += 1;

        AddResourceToResidencySet(textureHandle);
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
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[i];
            if (descriptorAtBinding == Descriptor::STORAGE_BUFFER || descriptorAtBinding == Descriptor::UNIFORM_BUFFER)
            {
                bufferIndex++;
            }
        }

        m_Buffers[bufferIndex] = newBufferHandle;

        MTL::Buffer* buffer = (MTL::Buffer*)newBufferHandle->GetNativeHandle();
        MTL::GPUAddress bufferGPUAddress = buffer->gpuAddress();

        // Set binding in argument buffer
        uint32 physicalBinding = m_LogicalToPhysicalBindingMap[binding];
        MTL::GPUAddress* bufferPointer;
        m_ArgumentBuffer->MapPointer(reinterpret_cast<void**>(&bufferPointer));
        bufferPointer[physicalBinding] = bufferGPUAddress;
        m_ArgumentBuffer->UnmapPointer();

        AddResourceToResidencySet(newBufferHandle);
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
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[i];
            if (descriptorAtBinding == Descriptor::STORAGE_BUFFER || descriptorAtBinding == Descriptor::UNIFORM_BUFFER)
            {
                bufferIndex++;
            }
        }

        m_Buffers[bufferIndex] = newBufferHandle;

        MTL::Buffer* buffer = (MTL::Buffer*)newBufferHandle->GetNativeHandle();
        MTL::GPUAddress bufferGPUAddress = buffer->gpuAddress();

        // Set binding in argument buffer
        uint32 physicalBinding = m_LogicalToPhysicalBindingMap[binding];
        MTL::GPUAddress* bufferPointer;
        m_ArgumentBuffer->MapPointer(reinterpret_cast<void**>(&bufferPointer));
        bufferPointer[physicalBinding] = bufferGPUAddress;
        m_ArgumentBuffer->UnmapPointer();

        AddResourceToResidencySet(newBufferHandle);
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
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[i];
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

        // Set binding in argument buffer
        uint32 physicalBinding = m_LogicalToPhysicalBindingMap[binding];
        MTL::ResourceID* bufferPointer;
        m_ArgumentBuffer->MapPointer(reinterpret_cast<void**>(&bufferPointer));
        bufferPointer[physicalBinding] = textureResourceID;
        bufferPointer[physicalBinding + 1] = samplerResourceID;
        m_ArgumentBuffer->UnmapPointer();

        AddResourceToResidencySet(newTextureHandle);
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
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[i];
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

        // Set binding in argument buffer
        uint32 physicalBinding = m_LogicalToPhysicalBindingMap[binding];
        MTL::ResourceID* bufferPointer;
        m_ArgumentBuffer->MapPointer(reinterpret_cast<void**>(&bufferPointer));
        bufferPointer[physicalBinding] = textureResourceID;
        bufferPointer[physicalBinding + 1] = samplerResourceID;
        m_ArgumentBuffer->UnmapPointer();

        AddResourceToResidencySet(newTextureHandle);
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
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[i];
            if (descriptorAtBinding == Descriptor::IMAGE_SAMPLER || descriptorAtBinding == Descriptor::STORAGE_IMAGE)
            {
                textureIndex++;
            }
        }

        m_Textures[textureIndex] = newTextureHandle;
        MTL::Texture* texture = (MTL::Texture*)newTextureHandle->GetNativeImageView();
        MTL::ResourceID textureResourceID = texture->gpuResourceID();

        // Set binding in argument buffer
        uint32 physicalBinding = m_LogicalToPhysicalBindingMap[binding];
        MTL::ResourceID* bufferPointer;
        m_ArgumentBuffer->MapPointer(reinterpret_cast<void**>(&bufferPointer));
        bufferPointer[physicalBinding] = textureResourceID;
        m_ArgumentBuffer->UnmapPointer();

        AddResourceToResidencySet(newTextureHandle);
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
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[i];
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
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[i];
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
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[i];
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
            Descriptor descriptorAtBinding = m_DescriptorSetLayout.Descriptors[i];
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
        return m_ArgumentBuffer->GetNativeHandle();
    }


    void* MetalDescriptorSet::GetNativeHandle()
    {
        return m_ArgumentBuffer->GetNativeHandle();
    }


    void MetalDescriptorSet::Invalidate()
    {
        ReleaseArgumentTable();
        m_DescriptorSetLayout.Descriptors.clear();
        m_Buffers.clear();
        m_Textures.clear();
        m_NumLogicalBindings = 0;
        m_NumPhysicalBindings = 0;
        m_LogicalToPhysicalBindingMap.clear();
    }


    void MetalDescriptorSet::CreateArgumentTable()
    {
        uint32 maxBindingMemoryRequirement = sizeof(size_t) * (m_Buffers.size() + m_Textures.size() + m_Textures.size());
        maxBindingMemoryRequirement = std::max(1u, maxBindingMemoryRequirement);

        m_ArgumentBuffer = RendererAPI::GetDevice().CreateUniformBuffer(nullptr, maxBindingMemoryRequirement, GPUMemoryType::HOST_VISIBLE);

        if (m_ArgumentBuffer == nullptr)
        {
            AE_ERROR("Argument buffer failed to be created!")
        }
    }


    void MetalDescriptorSet::ReleaseArgumentTable()
    {
        if (m_ArgumentBuffer)
        {
            m_ArgumentBuffer.reset();
            m_ArgumentBuffer = nullptr;
        }
    }


    void MetalDescriptorSet::UpdateDescriptorSets()
    {
        uint32 bufferIndex = 0;
        uint32 textureIndex = 0;
        for (uint32 i = 0; i < m_DescriptorSetLayout.Descriptors.size(); i++)
        {
            Descriptor descriptorType = m_DescriptorSetLayout.Descriptors[i];

            if (descriptorType == Descriptor::UNIFORM_BUFFER)
            {
                BufferHandle bufferHandle = m_Buffers[bufferIndex];
                bufferIndex++;
                UpdateUniformBinding(i, bufferHandle);
            }
            else if (descriptorType == Descriptor::STORAGE_BUFFER)
            {
                BufferHandle bufferHandle = m_Buffers[bufferIndex];
                bufferIndex++;
                UpdateStorageBufferBinding(i, bufferHandle);
            }
            else if (descriptorType == Descriptor::IMAGE_SAMPLER)
            {
                TextureHandle textureHandle = m_Textures[textureIndex];
                textureIndex++;
                UpdateImageSamplerBinding(i, textureHandle);
            }
            else if (descriptorType == Descriptor::STORAGE_IMAGE)
            {
                TextureHandle textureHandle = m_Textures[textureIndex];
                textureIndex++;
                UpdateStorageImageBinding(i, textureHandle, 0, ImageLayout::GENERAL); // Last two arguments do not get used for Metal
            }
        }
    }


    void MetalDescriptorSet::AddResourceToResidencySet(const TextureHandle& texture)
    {
        MTL::Texture* mtlTexture = (MTL::Texture*)texture->GetNativeImage();
        MetalRenderingContext& renderingContext = (MetalRenderingContext&)RendererAPI::GetContext();
        MTL::ResidencySet* residencySet = renderingContext.GetGlobalResidencySet();
        residencySet->addAllocation(mtlTexture);
    }


    void MetalDescriptorSet::AddResourceToResidencySet(const BufferHandle& buffer)
    {
        MTL::Buffer* mtlBuffer = (MTL::Buffer*)buffer->GetNativeHandle();
        MetalRenderingContext& renderingContext = (MetalRenderingContext&)RendererAPI::GetContext();
        MTL::ResidencySet* residencySet = renderingContext.GetGlobalResidencySet();
        residencySet->addAllocation(mtlBuffer);
    }

}
