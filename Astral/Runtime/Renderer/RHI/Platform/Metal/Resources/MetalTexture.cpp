/**
* @file MetalTexture.cpp
* @author Andrew Fagan
* @date 4/11/26
*/

#include "MetalTexture.h"

#include "MetalBuffer.h"
#include "Core/Utilities/Asserts.h"
#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Platform/Metal/Common/MTLEnumConversions.h"

#include "Metal/MTLBuffer.hpp"
#include "Metal/MTLCommandBuffer.hpp"
#include "Metal/MTLBlitCommandEncoder.hpp"

namespace Astral {

    MetalTexture::MetalTexture(const MetalTextureDesc& desc) :
        m_Device(desc.Device)
    {
        ASSERT(desc.Device, "The device passed to MetalTexture::MetalTexture cannot be nullptr!");

        CreateTexture(desc);
        InitializeTextureData(desc);
    }


    MetalTexture::MetalTexture(const MetalDrawableOwnedTextureDesc& desc) :
        m_Device(desc.Device),
        m_Texture(desc.DrawableOwnedTexture),
        m_Sampler(nullptr),
        m_Width(m_Texture->width()),
        m_Height(m_Texture->height()),
        m_ImageFormat(ConvertMTLPixelFormatToImageFormat(m_Texture->pixelFormat())),
        m_ImageUsageFlags(IMAGE_USAGE_COLOR_ATTACHMENT_BIT),
        m_NumLayers(1),
        m_NumMipLevels(1),
        m_TextureType(TextureType::IMAGE_2D),
        m_MemoryType(GPUMemoryType::DEVICE_LOCAL),
        m_MSAASampleCount(SampleCount::SAMPLE_1_BIT),
        m_SamplerFilter(SamplerFilter::LINEAR),
        m_SamplerAddressMode(SamplerAddressMode::REPEAT),
        m_IsAnisotropyEnabled(true),
        m_IsSwapchainOwned(true)
    {

    }


    MetalTexture::~MetalTexture()
    {
        DestroyTexture();
    }


    int MetalTexture::GetWidth()
    {
        return m_Height;
    }


    int MetalTexture::GetHeight()
    {
        return m_Height;
    }


    UVec2 MetalTexture::GetDimensions()
    {
        return UVec2(m_Width, m_Height);
    }


    ImageAspectFlags MetalTexture::GetImageAspect()
    {
        return 0; // Metal does not expose image aspects
    }


    ImageLayout MetalTexture::GetLayout()
    {
        return ImageLayout::GENERAL; // Metal does not expose image layouts
    }


    void MetalTexture::UpdateLayout(ImageLayout imageLayout)
    {
        // Metal does not expose image layouts
    }


    ImageFormat MetalTexture::GetFormat()
    {
        return m_ImageFormat;
    }


    uint32 MetalTexture::GetNumLayers()
    {
        return m_NumLayers;
    }


    uint32 MetalTexture::GetNumMipLevels()
    {
        return m_NumMipLevels;
    }


    void* MetalTexture::GetNativeSampler()
    {
        return m_Sampler;
    }


    void* MetalTexture::GetNativeImage()
    {
        return m_Texture;
    }


    void* MetalTexture::GetNativeImageView()
    {
        return m_Texture;
    }


    void* MetalTexture::GetNativeLayerImageView(uint32 layer)
    {
        return m_Texture;
    }


    void* MetalTexture::GetNativeMipMapImageView(uint32 mipLevel)
    {
        return m_Texture;
    }


    void* MetalTexture::GetNativeImageView(uint32 layer, uint32 mipLevel)
    {
        return m_Texture;
    }


    MetalTexture::MetalTexture(MetalTexture&& other) noexcept :
        m_Device(other.m_Device),
        m_Texture(other.m_Texture),
        m_Sampler(other.m_Sampler),

        m_Width(other.m_Width),
        m_Height(other.m_Height),
        m_ImageFormat(other.m_ImageFormat),
        m_ImageUsageFlags(other.m_ImageUsageFlags),
        m_NumLayers(other.m_NumLayers),
        m_NumMipLevels(other.m_NumMipLevels),
        m_TextureType(other.m_TextureType),
        m_MemoryType(other.m_MemoryType),

        m_MSAASampleCount(other.m_MSAASampleCount),
        m_SamplerFilter(other.m_SamplerFilter),
        m_SamplerAddressMode(other.m_SamplerAddressMode),
        m_IsAnisotropyEnabled(other.m_IsAnisotropyEnabled)
    {
        other.m_Device = nullptr;
        other.m_Texture = nullptr;
        other.m_Sampler = nullptr;

        other.m_Width = 0;
        other.m_Height = 0;
        other.m_ImageFormat = ImageFormat::UNDEFINED;
        other.m_ImageUsageFlags = 0;
        other.m_NumLayers = 0;
        other.m_NumMipLevels = 0;
        other.m_TextureType = TextureType::IMAGE_1D;
        other.m_MemoryType = GPUMemoryType::HOST_VISIBLE;

        other.m_MSAASampleCount = SampleCount::SAMPLE_1_BIT;
        other.m_SamplerFilter = SamplerFilter::LINEAR;
        other.m_SamplerAddressMode = SamplerAddressMode::REPEAT;
        other.m_IsAnisotropyEnabled = false;
    }


    MetalTexture& MetalTexture::operator=(MetalTexture&& other) noexcept
    {
        if (this != &other)
        {
            DestroyTexture(); // Clean up old texture and sampler if one exists
            DestroySampler();

            m_Device = other.m_Device;
            m_Texture = other.m_Texture;
            m_Sampler = other.m_Sampler;

            m_Width = other.m_Width;
            m_Height = other.m_Height;
            m_ImageFormat = other.m_ImageFormat;
            m_ImageUsageFlags = other.m_ImageUsageFlags;
            m_NumLayers = other.m_NumLayers;
            m_NumMipLevels = other.m_NumMipLevels;
            m_TextureType = other.m_TextureType;
            m_MemoryType = other.m_MemoryType;

            m_MSAASampleCount = other.m_MSAASampleCount;
            m_SamplerFilter = other.m_SamplerFilter;
            m_SamplerAddressMode = other.m_SamplerAddressMode;
            m_IsAnisotropyEnabled = other.m_IsAnisotropyEnabled;


            other.m_Device = nullptr;
            other.m_Texture = nullptr;
            other.m_Sampler = nullptr;

            other.m_Width = 0;
            other.m_Height = 0;
            other.m_ImageFormat = ImageFormat::UNDEFINED;
            other.m_ImageUsageFlags = 0;
            other.m_NumLayers = 0;
            other.m_NumMipLevels = 0;
            other.m_TextureType = TextureType::IMAGE_1D;
            other.m_MemoryType = GPUMemoryType::HOST_VISIBLE;

            other.m_MSAASampleCount = SampleCount::SAMPLE_1_BIT;
            other.m_SamplerFilter = SamplerFilter::LINEAR;
            other.m_SamplerAddressMode = SamplerAddressMode::REPEAT;
            other.m_IsAnisotropyEnabled = false;
        }

        return *this;
    }


    void MetalTexture::CreateTexture(const MetalTextureDesc& desc)
    {
        MTL::TextureDescriptor* textureDescriptor = MTL::TextureDescriptor::alloc();
        ASSERT(textureDescriptor, "MTL::TextureDescriptor failed to be allocated by Metal!")

        MTL::ResourceOptions resourceOptions = MTL::ResourceOptions();
        switch (m_MemoryType)
        {
            case GPUMemoryType::DEVICE_LOCAL: resourceOptions |= MTL::ResourceStorageModePrivate; break;
            case GPUMemoryType::HOST_VISIBLE: resourceOptions |= MTL::ResourceStorageModeShared; break;
            default: AE_ERROR("[MetalBuffer::CreateBuffer] Given memory type is not implemented!")
        }
        
        textureDescriptor->setResourceOptions(resourceOptions);

        textureDescriptor->setPixelFormat(ConvertImageFormatToMTLPixelFormat(desc.ImageFormat));
        textureDescriptor->setUsage(ConvertImageUsageToMTLTextureUsage(desc.ImageUsageFlags));
        textureDescriptor->setWidth(desc.ImageWidth);
        textureDescriptor->setHeight(desc.ImageHeight);
        textureDescriptor->setArrayLength(desc.NumLayers);
        textureDescriptor->setMipmapLevelCount(desc.NumMipLevels);
        textureDescriptor->setTextureType(ConvertTextureTypeToMTLTextureType(desc.TextureType));
        textureDescriptor->setSampleCount(ConvertSampleCountToIntSampleCount(desc.MSAASampleCount));

        desc.Device->newTexture(textureDescriptor);

        textureDescriptor->release();
    }


    void MetalTexture::DestroyTexture()
    {
        if (m_IsSwapchainOwned)
        {
            m_Texture = nullptr;
            return;
        }

        if (m_Texture)
        {
            m_Texture->release();
            m_Texture = nullptr;
        }
    }


    void MetalTexture::CreateSampler(const MetalTextureDesc& desc)
    {
        MTL::SamplerDescriptor* samplerDescriptor = MTL::SamplerDescriptor::alloc();
        ASSERT(samplerDescriptor, "MTL::SamplerDescriptor failed to be allocated by Metal!")

        samplerDescriptor->setMinFilter(ConvertSamplerFilterToMTLMinMagFilter(desc.SamplerFilter));
        samplerDescriptor->setMagFilter(ConvertSamplerFilterToMTLMinMagFilter(desc.SamplerFilter));
        samplerDescriptor->setMipFilter(MTL::SamplerMipFilterLinear);

        samplerDescriptor->setSAddressMode(ConvertSamplerAddressModeToMTLSamplerAddressMode(desc.SamplerAddressMode));
        samplerDescriptor->setTAddressMode(ConvertSamplerAddressModeToMTLSamplerAddressMode(desc.SamplerAddressMode));
        samplerDescriptor->setRAddressMode(ConvertSamplerAddressModeToMTLSamplerAddressMode(desc.SamplerAddressMode));

        if (RendererAPI::GetDevice().IsAnisotropySupported() && desc.EnableAnisotropy)
        {
            samplerDescriptor->setMaxAnisotropy(RendererAPI::GetDevice().GetMaxAnisotropySupported());
        }
        else
        {
            // Disable Anisotropy
            samplerDescriptor->setMaxAnisotropy(1);
        }

        desc.Device->newSamplerState(samplerDescriptor);

        samplerDescriptor->release();
    }


    void MetalTexture::DestroySampler()
    {
        if (m_Sampler) { m_Sampler->release(); }
    }


    void MetalTexture::InitializeTextureData(const MetalTextureDesc& desc)
    {
        if (m_MemoryType == GPUMemoryType::HOST_VISIBLE)
        {
            CopyToSharedTextureMemory(desc.ImageData, desc.ImageDataLength);
        }

        // Allocates a shared command buffer if a staging buffer copy or/and mip map generation is needed
        if (m_MemoryType == GPUMemoryType::DEVICE_LOCAL || desc.GenerateMipMaps)
        {
            CommandBufferHandle commandBufferHandle = RendererAPI::GetDevice().AllocateCommandBuffer();
            MTL::CommandBuffer* commandBuffer = (MTL::CommandBuffer*)commandBufferHandle->GetNativeHandle();

            commandBufferHandle->BeginRecording();
            MTL::BlitCommandEncoder* blitEncoder = commandBuffer->blitCommandEncoder();

            if (m_MemoryType == GPUMemoryType::DEVICE_LOCAL)
            {
                UploadToPrivateTextureMemory(blitEncoder, desc.ImageData, desc.ImageDataLength);
            }

            if (desc.GenerateMipMaps)
            {
                GenerateMipMaps(blitEncoder);
            }

            blitEncoder->endEncoding();
            commandBufferHandle->EndRecording();

            CommandQueueHandle commandQueueHandle = RendererAPI::GetDevice().GetPrimaryCommandQueue();
            commandQueueHandle->SubmitSync(commandBufferHandle);
            RendererAPI::GetDevice().WaitIdle();
        }
    }


    void MetalTexture::UploadToPrivateTextureMemory(MTL::BlitCommandEncoder* blitEncoder, void* data, uint32 length)
    {
        MetalBufferDesc bufferDesc = {
            .Device = m_Device,
            .Size = length,
            .MemoryType = GPUMemoryType::HOST_VISIBLE,
        };

        MetalBuffer stagingBuffer = MetalBuffer{bufferDesc};
        stagingBuffer.CopyDataToBuffer(data, length);

        CopyFromStagingBuffer(blitEncoder, stagingBuffer, length);
    }


    void MetalTexture::CopyToSharedTextureMemory(void* data, uint32 length)
    {
        void* textureMemory = m_Texture->buffer()->contents();
        memcpy(textureMemory, data, length);
    }


    void MetalTexture::CopyFromStagingBuffer(MTL::BlitCommandEncoder* blitEncoder, Buffer& stagingBuffer, uint32 length)
    {
        ASSERT(length <= stagingBuffer.GetAllocatedSize(), "Data does not fit in buffer!")
        MTL::Buffer* metalStagingBuffer = (MTL::Buffer*)stagingBuffer.GetNativeHandle();
        blitEncoder->copyFromBuffer(metalStagingBuffer, 0, m_Texture->buffer(), 0, length);
    }


    void MetalTexture::GenerateMipMaps(MTL::BlitCommandEncoder* blitEncoder)
    {
        if (!RendererAPI::GetDevice().IsBlitSupportedByFormat(m_ImageFormat))
        {
            AE_WARN("Texture format does not support blit command needed to generate mip maps!")
            return;
        }

        blitEncoder->generateMipmaps(m_Texture);
    }

}
