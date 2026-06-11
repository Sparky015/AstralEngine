/**
* @file MTLEnumConversions.h
* @author Andrew Fagan
* @date 4/11/26
*/

#pragma once

#include "Renderer/RHI/Common/ImageFormats.h"
#include "Renderer/RHI/Common/ImageLayouts.h"
#include "Renderer/RHI/Common/ImageUsageFlags.h"
#include "Renderer/RHI/Common/SamplerSettings.h"

#include "Metal/MTLPixelFormat.hpp"
#include "Metal/MTLSampler.hpp"
#include "Metal/MTLTexture.hpp"
#include "Renderer/RHI/Resources/Texture.h"


namespace Astral {

    MTL::PixelFormat ConvertImageFormatToMTLPixelFormat(ImageFormat imageFormat);
    MTL::TextureUsage ConvertImageUsageToMTLTextureUsage(ImageUsageFlags imageUsageFlags);
    MTL::TextureType ConvertTextureTypeToMTLTextureType(TextureType textureType);
    ImageFormat ConvertMTLPixelFormatToImageFormat(MTL::PixelFormat pixelFormat);

    MTL::SamplerAddressMode ConvertSamplerAddressModeToMTLSamplerAddressMode(SamplerAddressMode samplerAddressMode);
    MTL::SamplerMinMagFilter ConvertSamplerFilterToMTLMinMagFilter(SamplerFilter samplerFilter);

}
