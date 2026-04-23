/**
* @file MetalTexture.cpp
* @author Andrew Fagan
* @date 4/11/26
*/

#include "MetalTexture.h"

#include "Debug/Utilities/Asserts.h"
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

    }


    int MetalTexture::GetHeight()
    {

    }


    UVec2 MetalTexture::GetDimensions()
    {

    }


    ImageAspectFlags MetalTexture::GetImageAspect()
    {

    }


    ImageLayout MetalTexture::GetLayout()
    {

    }


    void MetalTexture::UpdateLayout(ImageLayout imageLayout)
    {

    }


    ImageFormat MetalTexture::GetFormat()
    {

    }


    uint32 MetalTexture::GetNumLayers()
    {

    }


    uint32 MetalTexture::GetNumMipLevels()
    {

    }


    void* MetalTexture::GetNativeSampler()
    {

    }


    void* MetalTexture::GetNativeImage()
    {

    }


    void* MetalTexture::GetNativeImageView()
    {

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

        textureDescriptor->setPixelFormat(ConvertImageFormatToMTLPixelFormat(desc.ImageFormat));
        textureDescriptor->setUsage(ConvertImageUsageToMTLTextureUsage(desc.ImageUsageFlags));
        textureDescriptor->setWidth(desc.ImageWidth);
        textureDescriptor->setHeight(desc.ImageHeight);
        textureDescriptor->setArrayLength(desc.NumLayers);
        textureDescriptor->setMipmapLevelCount(desc.NumMipLevels);
        textureDescriptor->setTextureType(ConvertTextureTypeToMTLTextureType(desc.TextureType));
        // textureDescriptor->setSampleCount(ConvertSampleCountToIntSampleCount(desc.MSAASampleCount));

        // TODO: continue setting up texture description sets

        desc.Device->newTexture(textureDescriptor);

        textureDescriptor->release();
    }


    void MetalTexture::DestroyTexture()
    {

    }

}
