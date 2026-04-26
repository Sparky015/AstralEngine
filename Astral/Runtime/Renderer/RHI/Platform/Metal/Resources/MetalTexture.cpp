/**
* @file MetalTexture.cpp
* @author Andrew Fagan
* @date 4/11/26
*/

#include "MetalTexture.h"

#include "Core/Utilities/Asserts.h"
#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Platform/Metal/Common/MTLEnumConversions.h"

namespace Astral {

    MetalTexture::MetalTexture(const MetalTextureDesc& desc) :
        m_Device(desc.Device)
    {
        ASSERT(desc.Device, "The device passed to MetalTexture::MetalTexture cannot be nullptr!");

        CreateTexture(desc);
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

    }


    void* MetalTexture::GetNativeMipMapImageView(uint32 mipLevel)
    {

    }


    void* MetalTexture::GetNativeImageView(uint32 layer, uint32 mipLevel)
    {

    }


    MetalTexture::MetalTexture(MetalTexture&& other) noexcept :
        m_Device(other.m_Device),
        m_Texture(other.m_Texture),
        m_Sampler(other.m_Sampler),

        m_Width(other.m_Width),
        m_Height(other.m_Height),
        m_ImageFormat(other.m_ImageFormat),
        m_ImageLayout(other.m_ImageLayout),
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
        other.m_ImageLayout = ImageLayout::UNDEFINED;
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
            m_ImageLayout = other.m_ImageLayout;
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
            other.m_ImageLayout = ImageLayout::UNDEFINED;
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
        if (m_Texture) { m_Texture->release(); }
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

}
