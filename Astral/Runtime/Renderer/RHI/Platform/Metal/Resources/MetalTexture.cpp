/**
* @file MetalTexture.cpp
* @author Andrew Fagan
* @date 4/11/26
*/

#include "MetalTexture.h"

#include "MetalBuffer.h"
#include "Core/Utilities/Asserts.h"
#include "Metal/MTL4CommandBuffer.hpp"
#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Platform/Metal/Common/MTLEnumConversions.h"

#include "Metal/MTLBuffer.hpp"
#include "Metal/MTLCommandBuffer.hpp"
#include "Renderer/RHI/Platform/Metal/MetalRendererContext.h"

namespace Astral {

    MetalTexture::MetalTexture(const MetalTextureDesc& desc) :
        m_Device(desc.Device),
        m_Width(desc.ImageWidth),
        m_Height(desc.ImageHeight),
        m_Depth(0),
        m_ImageFormat(desc.ImageFormat),
        m_ImageUsageFlags(desc.ImageUsageFlags),
        m_NumLayers(desc.NumLayers),
        m_NumMipLevels(desc.NumMipLevels),
        m_TextureType(desc.TextureType),
        m_MemoryType(desc.MemoryType),
        m_MSAASampleCount(desc.MSAASampleCount),
        m_SamplerFilter(desc.SamplerFilter),
        m_SamplerAddressMode(desc.SamplerAddressMode),
        m_IsAnisotropyEnabled(desc.EnableAnisotropy),
        m_IsSwapchainOwned(false)
    {
        ASSERT(desc.Device, "The device passed to MetalTexture::MetalTexture cannot be nullptr!");


        if (m_TextureType == TextureType::IMAGE_3D)
        {
            m_Depth = m_Width;
        }
        else if (m_TextureType == TextureType::IMAGE_1D)
        {
            m_Height = 1;
            m_Depth = 1;
        }
        else
        {
            m_Depth = 1;
        }

        CreateTexture(desc);
        CreateSampler(desc.SamplerFilter, desc.SamplerAddressMode, desc.EnableAnisotropy);

        MetalRenderingContext& renderingContext = (MetalRenderingContext&)RendererAPI::GetContext();
        MTL::ResidencySet* residencySet = renderingContext.GetGlobalResidencySet();
        residencySet->addAllocation(m_Texture);

        if (desc.ImageData != nullptr && desc.ImageDataLength != 0)
        {
            InitializeTextureData(desc);
        }
    }


    MetalTexture::MetalTexture(const MetalDrawableOwnedTextureDesc& desc) :
        m_Device(desc.Device),
        m_Texture(desc.DrawableOwnedTexture),
        m_Sampler(nullptr),
        m_Width(m_Texture->width()),
        m_Height(m_Texture->height()),
        m_Depth(1),
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
        CreateSampler(m_SamplerFilter, m_SamplerAddressMode, m_IsAnisotropyEnabled);
    }


    MetalTexture::~MetalTexture()
    {
        MetalRenderingContext& renderingContext = (MetalRenderingContext&)RendererAPI::GetContext();
        MTL::ResidencySet* residencySet = renderingContext.GetGlobalResidencySet();
        residencySet->removeAllocation(m_Texture);

        DestroySampler();
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
        MTL::TextureDescriptor* textureDescriptor = MTL::TextureDescriptor::alloc()->init();
        ASSERT(textureDescriptor, "MTL::TextureDescriptor failed to be allocated by Metal!")

        MTL::ResourceOptions resourceOptions = MTL::ResourceOptions();
        switch (m_MemoryType)
        {
            case GPUMemoryType::DEVICE_LOCAL: resourceOptions |= MTL::ResourceStorageModePrivate; break;
            case GPUMemoryType::HOST_VISIBLE: resourceOptions |= MTL::ResourceStorageModeShared; break;
            default: AE_ERROR("[MetalBuffer::CreateBuffer] Given memory type is not implemented!")
        }
        
        textureDescriptor->setResourceOptions(resourceOptions);

        textureDescriptor->setPixelFormat(ConvertImageFormatToMTLPixelFormat(m_ImageFormat));
        textureDescriptor->setUsage(ConvertImageUsageToMTLTextureUsage(m_ImageUsageFlags));
        textureDescriptor->setWidth(m_Width);
        textureDescriptor->setHeight(m_Height);
        textureDescriptor->setDepth(m_Depth);
        textureDescriptor->setArrayLength(m_NumLayers);
        textureDescriptor->setMipmapLevelCount(m_NumMipLevels);
        textureDescriptor->setTextureType(ConvertTextureTypeToMTLTextureType(m_TextureType));
        textureDescriptor->setSampleCount(ConvertSampleCountToIntSampleCount(m_MSAASampleCount));


        // Validate against edge cases and clamp inputs

        if (desc.TextureType == TextureType::IMAGE_1D)
        {
            textureDescriptor->setHeight(1);
        }
        if (desc.NumMipLevels > Texture::CalculateMipMapLevels(desc.ImageWidth, desc.ImageHeight))
        {
            uint32 clampedMipMapCount = Texture::CalculateMipMapLevels(desc.ImageWidth, desc.ImageHeight);
            AE_WARN("Texture with dimensions (" << desc.ImageWidth << ", " << desc.ImageHeight << ") cannot support mip map count of "
                    << desc.NumMipLevels << ". Clamping mip map level count to " << clampedMipMapCount << ". ");
            textureDescriptor->setMipmapLevelCount(clampedMipMapCount);
            m_NumMipLevels = clampedMipMapCount;
        }
        if (desc.NumLayers > 1 && desc.TextureType != TextureType::IMAGE_2D_ARRAY)
        {
            AE_WARN("Specified texture type does not support an array length of more than 1. Clamping array length to 1!");
            textureDescriptor->setArrayLength(1);
            m_NumLayers = 1;
        }
        if (desc.NumMipLevels > 1 && desc.TextureType == TextureType::IMAGE_1D)
        {
            AE_WARN("Mip map level count of more than 1 is not supported with IMAGE_1D texture type.. Clamping mip map level count to 1!");
            textureDescriptor->setMipmapLevelCount(1);
            m_NumMipLevels = 1;
        }

        m_Texture = desc.Device->newTexture(textureDescriptor);

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


    void MetalTexture::CreateSampler(SamplerFilter samplerFilter, SamplerAddressMode samplerAddressMode, bool shouldEnableAnisotropy)
    {
        MTL::SamplerDescriptor* samplerDescriptor = MTL::SamplerDescriptor::alloc()->init();
        ASSERT(samplerDescriptor, "MTL::SamplerDescriptor failed to be allocated by Metal!")

        samplerDescriptor->setSupportArgumentBuffers(true);
        samplerDescriptor->setMinFilter(ConvertSamplerFilterToMTLMinMagFilter(samplerFilter));
        samplerDescriptor->setMagFilter(ConvertSamplerFilterToMTLMinMagFilter(samplerFilter));
        samplerDescriptor->setMipFilter(MTL::SamplerMipFilterLinear);

        samplerDescriptor->setSAddressMode(ConvertSamplerAddressModeToMTLSamplerAddressMode(samplerAddressMode));
        samplerDescriptor->setTAddressMode(ConvertSamplerAddressModeToMTLSamplerAddressMode(samplerAddressMode));
        samplerDescriptor->setRAddressMode(ConvertSamplerAddressModeToMTLSamplerAddressMode(samplerAddressMode));

        if (RendererAPI::GetDevice().IsAnisotropySupported() && shouldEnableAnisotropy)
        {
            samplerDescriptor->setMaxAnisotropy(RendererAPI::GetDevice().GetMaxAnisotropySupported());
        }
        else
        {
            // Disable Anisotropy
            samplerDescriptor->setMaxAnisotropy(1);
        }

        m_Sampler = m_Device->newSamplerState(samplerDescriptor);

        samplerDescriptor->release();
    }


    void MetalTexture::DestroySampler()
    {
        if (m_Sampler) { m_Sampler->release(); }
    }


    void MetalTexture::InitializeTextureData(const MetalTextureDesc& desc)
    {
        uint32 imageSize = Texture::CalculateRequiredTextureMemory(m_ImageFormat, m_Width, m_Height, m_Depth, m_NumLayers, m_NumMipLevels);

        if (m_TextureType == TextureType::CUBEMAP)
        {
            imageSize *= 6;
        }

        imageSize = std::min(imageSize, desc.ImageDataLength);

        if (m_MemoryType == GPUMemoryType::HOST_VISIBLE)
        {
            CopyToSharedTextureMemory(desc.ImageData, imageSize);
        }

        // Allocates a shared command buffer if a staging buffer copy or/and mip map generation is needed
        if (m_MemoryType == GPUMemoryType::DEVICE_LOCAL || desc.GenerateMipMaps)
        {
            CommandBufferHandle commandBufferHandle = RendererAPI::GetDevice().AllocateCommandBuffer();
            MTL4::CommandBuffer* commandBuffer = (MTL4::CommandBuffer*)commandBufferHandle->GetNativeHandle();

            commandBufferHandle->BeginRecording();
            MTL4::ComputeCommandEncoder* blitEncoder = commandBuffer->computeCommandEncoder();


            MetalBuffer stagingBuffer = {}; // Initialize buffer here, so it won't be deallocated before command buffer submission
            if (m_MemoryType == GPUMemoryType::DEVICE_LOCAL)
            {
                // Upload to private texture

                MetalBufferDesc bufferDesc = {
                    .Device = m_Device,
                    .Size = imageSize,
                    .MemoryType = GPUMemoryType::HOST_VISIBLE,
                };

                stagingBuffer = MetalBuffer{bufferDesc};
                stagingBuffer.CopyDataToBuffer(desc.ImageData, imageSize);
                CopyFromStagingBuffer(blitEncoder, stagingBuffer, imageSize);
            }


            if (desc.GenerateMipMaps)
            {
                GenerateMipMaps(blitEncoder);
            }

            blitEncoder->endEncoding();
            commandBufferHandle->EndRecording();

            CommandQueueHandle commandQueueHandle = RendererAPI::GetDevice().GetPrimaryCommandQueue();
            commandQueueHandle->SubmitSync(commandBufferHandle);
            commandQueueHandle->WaitIdle();
        }
    }


    void MetalTexture::UploadToPrivateTextureMemory(MTL4::ComputeCommandEncoder* blitEncoder, void* data, uint32 length)
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
        uint32 usedData = 0;

        uint32 mipWidth = m_Width;
        uint32 mipHeight = m_Height;

        for (uint32 mipLevel = 0; mipLevel < m_NumMipLevels; mipLevel++)
        {
            for (uint32 layer = 0; layer < m_NumLayers; layer++)
            {
                MTL::Region region{};
                if (m_TextureType == TextureType::IMAGE_1D)
                {
                   region = MTL::Region::Make1D(0, mipWidth);
                }
                else if (m_TextureType == TextureType::IMAGE_2D)
                {
                   region = MTL::Region::Make2D(0, 0, mipWidth, mipHeight);
                }
                else if (m_TextureType == TextureType::IMAGE_3D)
                {
                    region = MTL::Region::Make3D(0, 0, 0, mipWidth, mipHeight, mipWidth);
                }
                else
                {
                    region = MTL::Region::Make2D(0, 0, mipWidth, mipHeight);
                }

                uint32 bytesPerRow = GetBytesPerTexel(m_ImageFormat) * mipWidth;
                uint32 bytesPerImage = bytesPerRow * mipHeight;

                if (m_TextureType == TextureType::CUBEMAP)
                {
                    for (uint32 face = 0; face < 6; face++)
                    {
                        if (length < usedData + bytesPerImage) { return; }
                        m_Texture->replaceRegion(region, mipLevel, face, (uint8*)data + usedData, bytesPerRow, bytesPerImage);
                        usedData += bytesPerImage;
                    }
                }
                else
                {
                    if (length < usedData + bytesPerImage) { return; }
                    m_Texture->replaceRegion(region, mipLevel, layer, (uint8*)data + usedData, bytesPerRow, bytesPerImage);
                    usedData += bytesPerImage;
                }
            }

            mipWidth /= 2;
            mipHeight /= 2;
        }
    }


    void MetalTexture::CopyFromStagingBuffer(MTL4::ComputeCommandEncoder* blitEncoder, Buffer& stagingBuffer, uint32 length)
    {
        ASSERT(length <= stagingBuffer.GetAllocatedSize(), "Data does not fit in buffer!")
        MTL::Buffer* metalStagingBuffer = (MTL::Buffer*)stagingBuffer.GetNativeHandle();

        uint32 usedData = 0;
        uint32 mipWidth = m_Width;
        uint32 mipHeight = m_Height;

        for (uint32 mipLevel = 0; mipLevel < m_NumMipLevels; mipLevel++)
        {
            MTL::Size imageDimensions = MTL::Size(mipWidth, mipHeight, 1);
            if (m_TextureType == TextureType::IMAGE_3D)
            {
                imageDimensions.depth = mipWidth;
            }

            for (uint32 layer = 0; layer < m_NumLayers; layer++)
            {
                uint32 bytesPerRow = GetBytesPerTexel(m_ImageFormat) * mipWidth;
                uint32 bytesPerImage = bytesPerRow * mipHeight;
                uint32 bytePerMipLevel = bytesPerRow * mipHeight * imageDimensions.depth;

                if (m_TextureType == TextureType::CUBEMAP)
                {
                    for (uint32 face = 0; face < 6; face++)
                    {
                        if (length < usedData + bytePerMipLevel) { return; }
                        blitEncoder->copyFromBuffer(metalStagingBuffer, usedData, bytesPerRow, bytesPerImage, imageDimensions, m_Texture, face, mipLevel, MTL::Origin(0,0,0));
                        usedData += bytesPerImage;
                    }
                }
                else
                {
                    if (length < usedData + bytePerMipLevel) { return; }
                    blitEncoder->copyFromBuffer(metalStagingBuffer, usedData, bytesPerRow, bytesPerImage, imageDimensions, m_Texture, layer, mipLevel, MTL::Origin(0,0,0));
                    usedData += bytesPerImage;
                }
            }

            mipWidth /= 2;
            mipHeight /= 2;
        }
    }


    void MetalTexture::GenerateMipMaps(MTL4::ComputeCommandEncoder* blitEncoder)
    {
        if (!RendererAPI::GetDevice().IsBlitSupportedByFormat(m_ImageFormat))
        {
            AE_WARN("Texture format does not support blit command needed to generate mip maps!")
            return;
        }

        blitEncoder->generateMipmaps(m_Texture);
    }

}
