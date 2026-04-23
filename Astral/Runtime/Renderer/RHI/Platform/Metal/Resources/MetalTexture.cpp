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

    MetalTexture::MetalTexture(const MetalTextureDesc& desc)
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


    void MetalTexture::CreateTexture(const MetalTextureDesc& desc)
    {
        MTL::TextureDescriptor* textureDescriptor = MTL::TextureDescriptor::alloc();
        ASSERT(textureDescriptor, "MTL::TextureDescriptor failed to be allocated by Metal!")

        // textureDescriptor->setResourceOptions(); TODO: Handle different memory types

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
