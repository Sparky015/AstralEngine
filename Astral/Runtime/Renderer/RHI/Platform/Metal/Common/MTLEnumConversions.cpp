/**
* @file MTLEnumConversions.cpp
* @author Andrew Fagan
* @date 4/11/26
*/

#include "MTLEnumConversions.h"

namespace Astral {

    MTL::PixelFormat ConvertImageFormatToMTLPixelFormat(ImageFormat imageFormat)
    {
        switch (imageFormat)
        {
            case ImageFormat::UNDEFINED:                                        return MTL::PixelFormatInvalid;
            case ImageFormat::R4G4_UNORM_PACK8:                                 return MTL::PixelFormatInvalid;
            case ImageFormat::R4G4B4A4_UNORM_PACK16:                            return MTL::PixelFormatABGR4Unorm;
            case ImageFormat::B4G4R4A4_UNORM_PACK16:                            return MTL::PixelFormatInvalid;
            case ImageFormat::R5G6B5_UNORM_PACK16:                              return MTL::PixelFormatB5G6R5Unorm;
            case ImageFormat::B5G6R5_UNORM_PACK16:                              return MTL::PixelFormatB5G6R5Unorm;
            case ImageFormat::R5G5B5A1_UNORM_PACK16:                            return MTL::PixelFormatBGR5A1Unorm;
            case ImageFormat::B5G5R5A1_UNORM_PACK16:                            return MTL::PixelFormatBGR5A1Unorm;
            case ImageFormat::A1R5G5B5_UNORM_PACK16:                            return MTL::PixelFormatABGR4Unorm;
            case ImageFormat::R8_UNORM:                                         return MTL::PixelFormatR8Unorm;
            case ImageFormat::R8_SNORM:                                         return MTL::PixelFormatR8Snorm;
            case ImageFormat::R8_USCALED:                                       return MTL::PixelFormatInvalid;
            case ImageFormat::R8_SSCALED:                                       return MTL::PixelFormatInvalid;
            case ImageFormat::R8_UINT:                                          return MTL::PixelFormatR8Uint;
            case ImageFormat::R8_SINT:                                          return MTL::PixelFormatR8Sint;
            case ImageFormat::R8_SRGB:                                          return MTL::PixelFormatR8Unorm_sRGB;
            case ImageFormat::R8G8_UNORM:                                       return MTL::PixelFormatRG8Unorm;
            case ImageFormat::R8G8_SNORM:                                       return MTL::PixelFormatRG8Snorm;
            case ImageFormat::R8G8_USCALED:                                     return MTL::PixelFormatInvalid;
            case ImageFormat::R8G8_SSCALED:                                     return MTL::PixelFormatInvalid;
            case ImageFormat::R8G8_UINT:                                        return MTL::PixelFormatRG8Uint;
            case ImageFormat::R8G8_SINT:                                        return MTL::PixelFormatRG8Sint;
            case ImageFormat::R8G8_SRGB:                                        return MTL::PixelFormatRG8Unorm_sRGB;
            case ImageFormat::R8G8B8_UNORM:                                     return MTL::PixelFormatInvalid;
            case ImageFormat::R8G8B8_SNORM:                                     return MTL::PixelFormatInvalid;
            case ImageFormat::R8G8B8_USCALED:                                   return MTL::PixelFormatInvalid;
            case ImageFormat::R8G8B8_SSCALED:                                   return MTL::PixelFormatInvalid;
            case ImageFormat::R8G8B8_UINT:                                      return MTL::PixelFormatInvalid;
            case ImageFormat::R8G8B8_SINT:                                      return MTL::PixelFormatInvalid;
            case ImageFormat::R8G8B8_SRGB:                                      return MTL::PixelFormatInvalid;
            case ImageFormat::B8G8R8_UNORM:                                     return MTL::PixelFormatInvalid;
            case ImageFormat::B8G8R8_SNORM:                                     return MTL::PixelFormatInvalid;
            case ImageFormat::B8G8R8_USCALED:                                   return MTL::PixelFormatInvalid;
            case ImageFormat::B8G8R8_SSCALED:                                   return MTL::PixelFormatInvalid;
            case ImageFormat::B8G8R8_UINT:                                      return MTL::PixelFormatInvalid;
            case ImageFormat::B8G8R8_SINT:                                      return MTL::PixelFormatInvalid;
            case ImageFormat::B8G8R8_SRGB:                                      return MTL::PixelFormatInvalid;
            case ImageFormat::R8G8B8A8_UNORM:                                   return MTL::PixelFormatRGBA8Unorm;
            case ImageFormat::R8G8B8A8_SNORM:                                   return MTL::PixelFormatRGBA8Snorm;
            case ImageFormat::R8G8B8A8_USCALED:                                 return MTL::PixelFormatInvalid;
            case ImageFormat::R8G8B8A8_SSCALED:                                 return MTL::PixelFormatInvalid;
            case ImageFormat::R8G8B8A8_UINT:                                    return MTL::PixelFormatRGBA8Uint;
            case ImageFormat::R8G8B8A8_SINT:                                    return MTL::PixelFormatRGBA8Sint;
            case ImageFormat::R8G8B8A8_SRGB:                                    return MTL::PixelFormatRGBA8Unorm_sRGB;
            case ImageFormat::B8G8R8A8_UNORM:                                   return MTL::PixelFormatBGRA8Unorm;
            case ImageFormat::B8G8R8A8_SNORM:                                   return MTL::PixelFormatInvalid;
            case ImageFormat::B8G8R8A8_USCALED:                                 return MTL::PixelFormatInvalid;
            case ImageFormat::B8G8R8A8_SSCALED:                                 return MTL::PixelFormatInvalid;
            case ImageFormat::B8G8R8A8_UINT:                                    return MTL::PixelFormatInvalid;
            case ImageFormat::B8G8R8A8_SINT:                                    return MTL::PixelFormatInvalid;
            case ImageFormat::B8G8R8A8_SRGB:                                    return MTL::PixelFormatBGRA8Unorm_sRGB;
            case ImageFormat::A8B8G8R8_UNORM_PACK32:                            return MTL::PixelFormatInvalid;
            case ImageFormat::A8B8G8R8_SNORM_PACK32:                            return MTL::PixelFormatInvalid;
            case ImageFormat::A8B8G8R8_USCALED_PACK32:                          return MTL::PixelFormatInvalid;
            case ImageFormat::A8B8G8R8_SSCALED_PACK32:                          return MTL::PixelFormatInvalid;
            case ImageFormat::A8B8G8R8_UINT_PACK32:                             return MTL::PixelFormatInvalid;
            case ImageFormat::A8B8G8R8_SINT_PACK32:                             return MTL::PixelFormatInvalid;
            case ImageFormat::A8B8G8R8_SRGB_PACK32:                             return MTL::PixelFormatInvalid;
            case ImageFormat::A2R10G10B10_UNORM_PACK32:                         return MTL::PixelFormatRGB10A2Unorm;
            case ImageFormat::A2R10G10B10_SNORM_PACK32:                         return MTL::PixelFormatInvalid;
            case ImageFormat::A2R10G10B10_USCALED_PACK32:                       return MTL::PixelFormatInvalid;
            case ImageFormat::A2R10G10B10_SSCALED_PACK32:                       return MTL::PixelFormatInvalid;
            case ImageFormat::A2R10G10B10_UINT_PACK32:                          return MTL::PixelFormatRGB10A2Uint;
            case ImageFormat::A2R10G10B10_SINT_PACK32:                          return MTL::PixelFormatInvalid;
            case ImageFormat::A2B10G10R10_UNORM_PACK32:                         return MTL::PixelFormatBGR10A2Unorm;
            case ImageFormat::A2B10G10R10_SNORM_PACK32:                         return MTL::PixelFormatInvalid;
            case ImageFormat::A2B10G10R10_USCALED_PACK32:                       return MTL::PixelFormatInvalid;
            case ImageFormat::A2B10G10R10_SSCALED_PACK32:                       return MTL::PixelFormatInvalid;
            case ImageFormat::A2B10G10R10_UINT_PACK32:                          return MTL::PixelFormatInvalid;
            case ImageFormat::A2B10G10R10_SINT_PACK32:                          return MTL::PixelFormatInvalid;
            case ImageFormat::R16_UNORM:                                        return MTL::PixelFormatR16Unorm;
            case ImageFormat::R16_SNORM:                                        return MTL::PixelFormatR16Snorm;
            case ImageFormat::R16_USCALED:                                      return MTL::PixelFormatInvalid;
            case ImageFormat::R16_SSCALED:                                      return MTL::PixelFormatInvalid;
            case ImageFormat::R16_UINT:                                         return MTL::PixelFormatR16Uint;
            case ImageFormat::R16_SINT:                                         return MTL::PixelFormatR16Sint;
            case ImageFormat::R16_SFLOAT:                                       return MTL::PixelFormatR16Float;
            case ImageFormat::R16G16_UNORM:                                     return MTL::PixelFormatRG16Unorm;
            case ImageFormat::R16G16_SNORM:                                     return MTL::PixelFormatRG16Snorm;
            case ImageFormat::R16G16_USCALED:                                   return MTL::PixelFormatInvalid;
            case ImageFormat::R16G16_SSCALED:                                   return MTL::PixelFormatInvalid;
            case ImageFormat::R16G16_UINT:                                      return MTL::PixelFormatRG16Uint;
            case ImageFormat::R16G16_SINT:                                      return MTL::PixelFormatRG16Sint;
            case ImageFormat::R16G16_SFLOAT:                                    return MTL::PixelFormatRG16Float;
            case ImageFormat::R16G16B16_UNORM:                                  return MTL::PixelFormatInvalid;
            case ImageFormat::R16G16B16_SNORM:                                  return MTL::PixelFormatInvalid;
            case ImageFormat::R16G16B16_USCALED:                                return MTL::PixelFormatInvalid;
            case ImageFormat::R16G16B16_SSCALED:                                return MTL::PixelFormatInvalid;
            case ImageFormat::R16G16B16_UINT:                                   return MTL::PixelFormatInvalid;
            case ImageFormat::R16G16B16_SINT:                                   return MTL::PixelFormatInvalid;
            case ImageFormat::R16G16B16_SFLOAT:                                 return MTL::PixelFormatInvalid;
            case ImageFormat::R16G16B16A16_UNORM:                               return MTL::PixelFormatRGBA16Unorm;
            case ImageFormat::R16G16B16A16_SNORM:                               return MTL::PixelFormatRGBA16Snorm;
            case ImageFormat::R16G16B16A16_USCALED:                             return MTL::PixelFormatInvalid;
            case ImageFormat::R16G16B16A16_SSCALED:                             return MTL::PixelFormatInvalid;
            case ImageFormat::R16G16B16A16_UINT:                                return MTL::PixelFormatRGBA16Uint;
            case ImageFormat::R16G16B16A16_SINT:                                return MTL::PixelFormatRGBA16Sint;
            case ImageFormat::R16G16B16A16_SFLOAT:                              return MTL::PixelFormatRGBA16Float;
            case ImageFormat::R32_UINT:                                         return MTL::PixelFormatR32Uint;
            case ImageFormat::R32_SINT:                                         return MTL::PixelFormatR32Sint;
            case ImageFormat::R32_SFLOAT:                                       return MTL::PixelFormatR32Float;
            case ImageFormat::R32G32_UINT:                                      return MTL::PixelFormatRG32Uint;
            case ImageFormat::R32G32_SINT:                                      return MTL::PixelFormatRG32Sint;
            case ImageFormat::R32G32_SFLOAT:                                    return MTL::PixelFormatRG32Float;
            case ImageFormat::R32G32B32_UINT:                                   return MTL::PixelFormatInvalid;
            case ImageFormat::R32G32B32_SINT:                                   return MTL::PixelFormatInvalid;
            case ImageFormat::R32G32B32_SFLOAT:                                 return MTL::PixelFormatInvalid;
            case ImageFormat::R32G32B32A32_UINT:                                return MTL::PixelFormatRGBA32Uint;
            case ImageFormat::R32G32B32A32_SINT:                                return MTL::PixelFormatRGBA32Sint;
            case ImageFormat::R32G32B32A32_SFLOAT:                              return MTL::PixelFormatRGBA32Float;
            case ImageFormat::R64_UINT:                                         return MTL::PixelFormatInvalid;
            case ImageFormat::R64_SINT:                                         return MTL::PixelFormatInvalid;
            case ImageFormat::R64_SFLOAT:                                       return MTL::PixelFormatInvalid;
            case ImageFormat::R64G64_UINT:                                      return MTL::PixelFormatInvalid;
            case ImageFormat::R64G64_SINT:                                      return MTL::PixelFormatInvalid;
            case ImageFormat::R64G64_SFLOAT:                                    return MTL::PixelFormatInvalid;
            case ImageFormat::R64G64B64_UINT:                                   return MTL::PixelFormatInvalid;
            case ImageFormat::R64G64B64_SINT:                                   return MTL::PixelFormatInvalid;
            case ImageFormat::R64G64B64_SFLOAT:                                 return MTL::PixelFormatInvalid;
            case ImageFormat::R64G64B64A64_UINT:                                return MTL::PixelFormatInvalid;
            case ImageFormat::R64G64B64A64_SINT:                                return MTL::PixelFormatInvalid;
            case ImageFormat::R64G64B64A64_SFLOAT:                              return MTL::PixelFormatInvalid;
            case ImageFormat::B10G11R11_UFLOAT_PACK32:                          return MTL::PixelFormatRG11B10Float;
            case ImageFormat::E5B9G9R9_UFLOAT_PACK32:                           return MTL::PixelFormatRGB9E5Float;
            case ImageFormat::D16_UNORM:                                        return MTL::PixelFormatDepth16Unorm;
            case ImageFormat::X8_D24_UNORM_PACK32:                              return MTL::PixelFormatInvalid;
            case ImageFormat::D32_SFLOAT:                                       return MTL::PixelFormatDepth32Float;
            case ImageFormat::S8_UINT:                                          return MTL::PixelFormatStencil8;
            case ImageFormat::D16_UNORM_S8_UINT:                                return MTL::PixelFormatDepth16Unorm;
            case ImageFormat::D24_UNORM_S8_UINT:                                return MTL::PixelFormatDepth24Unorm_Stencil8;
            case ImageFormat::D32_SFLOAT_S8_UINT:                               return MTL::PixelFormatDepth32Float_Stencil8;
            case ImageFormat::BC1_RGB_UNORM_BLOCK:                              return MTL::PixelFormatBC1_RGBA;
            case ImageFormat::BC1_RGB_SRGB_BLOCK:                               return MTL::PixelFormatBC1_RGBA_sRGB;
            case ImageFormat::BC1_RGBA_UNORM_BLOCK:                             return MTL::PixelFormatBC1_RGBA;
            case ImageFormat::BC1_RGBA_SRGB_BLOCK:                              return MTL::PixelFormatBC1_RGBA_sRGB;
            case ImageFormat::BC2_UNORM_BLOCK:                                  return MTL::PixelFormatBC2_RGBA;
            case ImageFormat::BC2_SRGB_BLOCK:                                   return MTL::PixelFormatBC2_RGBA_sRGB;
            case ImageFormat::BC3_UNORM_BLOCK:                                  return MTL::PixelFormatBC3_RGBA;
            case ImageFormat::BC3_SRGB_BLOCK:                                   return MTL::PixelFormatBC3_RGBA_sRGB;
            case ImageFormat::BC4_UNORM_BLOCK:                                  return MTL::PixelFormatBC4_RUnorm;
            case ImageFormat::BC4_SNORM_BLOCK:                                  return MTL::PixelFormatBC4_RSnorm;
            case ImageFormat::BC5_UNORM_BLOCK:                                  return MTL::PixelFormatBC5_RGUnorm;
            case ImageFormat::BC5_SNORM_BLOCK:                                  return MTL::PixelFormatBC5_RGSnorm;
            case ImageFormat::BC6H_UFLOAT_BLOCK:                                return MTL::PixelFormatBC6H_RGBUfloat;
            case ImageFormat::BC6H_SFLOAT_BLOCK:                                return MTL::PixelFormatBC6H_RGBFloat;
            case ImageFormat::BC7_UNORM_BLOCK:                                  return MTL::PixelFormatBC7_RGBAUnorm;
            case ImageFormat::BC7_SRGB_BLOCK:                                   return MTL::PixelFormatBC7_RGBAUnorm_sRGB;
            case ImageFormat::ETC2_R8G8B8_UNORM_BLOCK:                          return MTL::PixelFormatETC2_RGB8;
            case ImageFormat::ETC2_R8G8B8_SRGB_BLOCK:                           return MTL::PixelFormatETC2_RGB8_sRGB;
            case ImageFormat::ETC2_R8G8B8A1_UNORM_BLOCK:                        return MTL::PixelFormatEAC_RGBA8;
            case ImageFormat::ETC2_R8G8B8A1_SRGB_BLOCK:                         return MTL::PixelFormatEAC_RGBA8_sRGB;
            case ImageFormat::ETC2_R8G8B8A8_UNORM_BLOCK:                        return MTL::PixelFormatEAC_RGBA8;
            case ImageFormat::ETC2_R8G8B8A8_SRGB_BLOCK:                         return MTL::PixelFormatEAC_RGBA8_sRGB;
            case ImageFormat::EAC_R11_UNORM_BLOCK:                              return MTL::PixelFormatEAC_R11Unorm;
            case ImageFormat::EAC_R11_SNORM_BLOCK:                              return MTL::PixelFormatEAC_R11Snorm;
            case ImageFormat::EAC_R11G11_UNORM_BLOCK:                           return MTL::PixelFormatEAC_RG11Unorm;
            case ImageFormat::EAC_R11G11_SNORM_BLOCK:                           return MTL::PixelFormatEAC_RG11Snorm;
            case ImageFormat::ASTC_4x4_UNORM_BLOCK:                             return MTL::PixelFormatASTC_4x4_LDR;
            case ImageFormat::ASTC_4x4_SRGB_BLOCK:                              return MTL::PixelFormatASTC_4x4_sRGB;
            case ImageFormat::ASTC_5x4_UNORM_BLOCK:                             return MTL::PixelFormatASTC_5x4_LDR;
            case ImageFormat::ASTC_5x4_SRGB_BLOCK:                              return MTL::PixelFormatASTC_5x4_sRGB;
            case ImageFormat::ASTC_5x5_UNORM_BLOCK:                             return MTL::PixelFormatASTC_5x5_LDR;
            case ImageFormat::ASTC_5x5_SRGB_BLOCK:                              return MTL::PixelFormatASTC_5x5_sRGB;
            case ImageFormat::ASTC_6x5_UNORM_BLOCK:                             return MTL::PixelFormatASTC_6x5_LDR;
            case ImageFormat::ASTC_6x5_SRGB_BLOCK:                              return MTL::PixelFormatASTC_6x5_sRGB;
            case ImageFormat::ASTC_6x6_UNORM_BLOCK:                             return MTL::PixelFormatASTC_6x6_LDR;
            case ImageFormat::ASTC_6x6_SRGB_BLOCK:                              return MTL::PixelFormatASTC_6x6_sRGB;
            case ImageFormat::ASTC_8x5_UNORM_BLOCK:                             return MTL::PixelFormatASTC_8x5_LDR;
            case ImageFormat::ASTC_8x5_SRGB_BLOCK:                              return MTL::PixelFormatASTC_8x5_sRGB;
            case ImageFormat::ASTC_8x6_UNORM_BLOCK:                             return MTL::PixelFormatASTC_8x6_LDR;
            case ImageFormat::ASTC_8x6_SRGB_BLOCK:                              return MTL::PixelFormatASTC_8x6_sRGB;
            case ImageFormat::ASTC_8x8_UNORM_BLOCK:                             return MTL::PixelFormatASTC_8x8_LDR;
            case ImageFormat::ASTC_8x8_SRGB_BLOCK:                              return MTL::PixelFormatASTC_8x8_sRGB;
            case ImageFormat::ASTC_10x5_UNORM_BLOCK:                            return MTL::PixelFormatASTC_10x5_LDR;
            case ImageFormat::ASTC_10x5_SRGB_BLOCK:                             return MTL::PixelFormatASTC_10x5_sRGB;
            case ImageFormat::ASTC_10x6_UNORM_BLOCK:                            return MTL::PixelFormatASTC_10x6_LDR;
            case ImageFormat::ASTC_10x6_SRGB_BLOCK:                             return MTL::PixelFormatASTC_10x6_sRGB;
            case ImageFormat::ASTC_10x8_UNORM_BLOCK:                            return MTL::PixelFormatASTC_10x8_LDR;
            case ImageFormat::ASTC_10x8_SRGB_BLOCK:                             return MTL::PixelFormatASTC_10x8_sRGB;
            case ImageFormat::ASTC_10x10_UNORM_BLOCK:                           return MTL::PixelFormatASTC_10x10_LDR;
            case ImageFormat::ASTC_10x10_SRGB_BLOCK:                            return MTL::PixelFormatASTC_10x10_sRGB;
            case ImageFormat::ASTC_12x10_UNORM_BLOCK:                           return MTL::PixelFormatASTC_12x10_LDR;
            case ImageFormat::ASTC_12x10_SRGB_BLOCK:                            return MTL::PixelFormatASTC_12x10_sRGB;
            case ImageFormat::ASTC_12x12_UNORM_BLOCK:                           return MTL::PixelFormatASTC_12x12_LDR;
            case ImageFormat::ASTC_12x12_SRGB_BLOCK:                            return MTL::PixelFormatASTC_12x12_sRGB;
            case ImageFormat::G8B8G8R8_422_UNORM:                               return MTL::PixelFormatInvalid;
            case ImageFormat::B8G8R8G8_422_UNORM:                               return MTL::PixelFormatInvalid;
            case ImageFormat::G8_B8_R8_3PLANE_420_UNORM:                        return MTL::PixelFormatInvalid;
            case ImageFormat::G8_B8R8_2PLANE_420_UNORM:                         return MTL::PixelFormatInvalid;
            case ImageFormat::G8_B8_R8_3PLANE_422_UNORM:                        return MTL::PixelFormatInvalid;
            case ImageFormat::G8_B8R8_2PLANE_422_UNORM:                         return MTL::PixelFormatInvalid;
            case ImageFormat::G8_B8_R8_3PLANE_444_UNORM:                        return MTL::PixelFormatInvalid;
            case ImageFormat::R10X6_UNORM_PACK16:                               return MTL::PixelFormatInvalid;
            case ImageFormat::R10X6G10X6_UNORM_2PACK16:                         return MTL::PixelFormatInvalid;
            case ImageFormat::R10X6G10X6B10X6A10X6_UNORM_4PACK16:               return MTL::PixelFormatInvalid;
            case ImageFormat::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:           return MTL::PixelFormatInvalid;
            case ImageFormat::B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:           return MTL::PixelFormatInvalid;
            case ImageFormat::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:       return MTL::PixelFormatInvalid;
            case ImageFormat::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:        return MTL::PixelFormatInvalid;
            case ImageFormat::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:       return MTL::PixelFormatInvalid;
            case ImageFormat::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:        return MTL::PixelFormatInvalid;
            case ImageFormat::G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:       return MTL::PixelFormatInvalid;
            case ImageFormat::R12X4_UNORM_PACK16:                               return MTL::PixelFormatInvalid;
            case ImageFormat::R12X4G12X4_UNORM_2PACK16:                         return MTL::PixelFormatInvalid;
            case ImageFormat::R12X4G12X4B12X4A12X4_UNORM_4PACK16:               return MTL::PixelFormatInvalid;
            case ImageFormat::G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:           return MTL::PixelFormatInvalid;
            case ImageFormat::B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:           return MTL::PixelFormatInvalid;
            case ImageFormat::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:       return MTL::PixelFormatInvalid;
            case ImageFormat::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:        return MTL::PixelFormatInvalid;
            case ImageFormat::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:       return MTL::PixelFormatInvalid;
            case ImageFormat::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:        return MTL::PixelFormatInvalid;
            case ImageFormat::G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:       return MTL::PixelFormatInvalid;
            case ImageFormat::G16B16G16R16_422_UNORM:                           return MTL::PixelFormatInvalid;
            case ImageFormat::B16G16R16G16_422_UNORM:                           return MTL::PixelFormatInvalid;
            case ImageFormat::G16_B16_R16_3PLANE_420_UNORM:                     return MTL::PixelFormatInvalid;
            case ImageFormat::G16_B16R16_2PLANE_420_UNORM:                      return MTL::PixelFormatInvalid;
            case ImageFormat::G16_B16_R16_3PLANE_422_UNORM:                     return MTL::PixelFormatInvalid;
            case ImageFormat::G16_B16R16_2PLANE_422_UNORM:                      return MTL::PixelFormatInvalid;
            case ImageFormat::G16_B16_R16_3PLANE_444_UNORM:                     return MTL::PixelFormatInvalid;
            case ImageFormat::G8_B8R8_2PLANE_444_UNORM:                         return MTL::PixelFormatInvalid;
            case ImageFormat::G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16:        return MTL::PixelFormatInvalid;
            case ImageFormat::G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16:        return MTL::PixelFormatInvalid;
            case ImageFormat::G16_B16R16_2PLANE_444_UNORM:                      return MTL::PixelFormatInvalid;
            case ImageFormat::A4R4G4B4_UNORM_PACK16:                            return MTL::PixelFormatInvalid;
            case ImageFormat::A4B4G4R4_UNORM_PACK16:                            return MTL::PixelFormatInvalid;
            case ImageFormat::ASTC_4x4_SFLOAT_BLOCK:                            return MTL::PixelFormatASTC_4x4_HDR;
            case ImageFormat::ASTC_5x4_SFLOAT_BLOCK:                            return MTL::PixelFormatASTC_5x4_HDR;
            case ImageFormat::ASTC_5x5_SFLOAT_BLOCK:                            return MTL::PixelFormatASTC_5x5_HDR;
            case ImageFormat::ASTC_6x5_SFLOAT_BLOCK:                            return MTL::PixelFormatASTC_6x5_HDR;
            case ImageFormat::ASTC_6x6_SFLOAT_BLOCK:                            return MTL::PixelFormatASTC_6x6_HDR;
            case ImageFormat::ASTC_8x5_SFLOAT_BLOCK:                            return MTL::PixelFormatASTC_8x5_HDR;
            case ImageFormat::ASTC_8x6_SFLOAT_BLOCK:                            return MTL::PixelFormatASTC_8x6_HDR;
            case ImageFormat::ASTC_8x8_SFLOAT_BLOCK:                            return MTL::PixelFormatASTC_8x8_HDR;
            case ImageFormat::ASTC_10x5_SFLOAT_BLOCK:                           return MTL::PixelFormatASTC_10x5_HDR;
            case ImageFormat::ASTC_10x6_SFLOAT_BLOCK:                           return MTL::PixelFormatASTC_10x6_HDR;
            case ImageFormat::ASTC_10x8_SFLOAT_BLOCK:                           return MTL::PixelFormatASTC_10x8_HDR;
            case ImageFormat::ASTC_10x10_SFLOAT_BLOCK:                          return MTL::PixelFormatASTC_10x10_HDR;
            case ImageFormat::ASTC_12x10_SFLOAT_BLOCK:                          return MTL::PixelFormatASTC_12x10_HDR;
            case ImageFormat::ASTC_12x12_SFLOAT_BLOCK:                          return MTL::PixelFormatASTC_12x12_HDR;
            case ImageFormat::PVRTC1_2BPP_UNORM_BLOCK_IMG:                      return MTL::PixelFormatPVRTC_RGB_2BPP;
            case ImageFormat::PVRTC1_4BPP_UNORM_BLOCK_IMG:                      return MTL::PixelFormatPVRTC_RGB_4BPP;
            case ImageFormat::PVRTC2_2BPP_UNORM_BLOCK_IMG:                      return MTL::PixelFormatInvalid;
            case ImageFormat::PVRTC2_4BPP_UNORM_BLOCK_IMG:                      return MTL::PixelFormatInvalid;
            case ImageFormat::PVRTC1_2BPP_SRGB_BLOCK_IMG:                       return MTL::PixelFormatPVRTC_RGB_2BPP_sRGB;
            case ImageFormat::PVRTC1_4BPP_SRGB_BLOCK_IMG:                       return MTL::PixelFormatPVRTC_RGB_4BPP_sRGB;
            case ImageFormat::PVRTC2_2BPP_SRGB_BLOCK_IMG:                       return MTL::PixelFormatInvalid;
            case ImageFormat::PVRTC2_4BPP_SRGB_BLOCK_IMG:                       return MTL::PixelFormatInvalid;
            case ImageFormat::R16G16_SFIXED5_NV:                                return MTL::PixelFormatInvalid;
            case ImageFormat::A1B5G5R5_UNORM_PACK16_KHR:                        return MTL::PixelFormatABGR4Unorm;
            case ImageFormat::A8_UNORM_KHR:                                     return MTL::PixelFormatA8Unorm;
            case ImageFormat::ASTC_4x4_SFLOAT_BLOCK_EXT:                        return MTL::PixelFormatASTC_4x4_HDR;
            case ImageFormat::ASTC_5x4_SFLOAT_BLOCK_EXT:                        return MTL::PixelFormatASTC_5x4_HDR;
            case ImageFormat::ASTC_5x5_SFLOAT_BLOCK_EXT:                        return MTL::PixelFormatASTC_5x5_HDR;
            case ImageFormat::ASTC_6x5_SFLOAT_BLOCK_EXT:                        return MTL::PixelFormatASTC_6x5_HDR;
            case ImageFormat::ASTC_6x6_SFLOAT_BLOCK_EXT:                        return MTL::PixelFormatASTC_6x6_HDR;
            case ImageFormat::ASTC_8x5_SFLOAT_BLOCK_EXT:                        return MTL::PixelFormatASTC_8x5_HDR;
            case ImageFormat::ASTC_8x6_SFLOAT_BLOCK_EXT:                        return MTL::PixelFormatASTC_8x6_HDR;
            case ImageFormat::ASTC_8x8_SFLOAT_BLOCK_EXT:                        return MTL::PixelFormatASTC_8x8_HDR;
            case ImageFormat::ASTC_10x5_SFLOAT_BLOCK_EXT:                       return MTL::PixelFormatASTC_10x5_HDR;
            case ImageFormat::ASTC_10x6_SFLOAT_BLOCK_EXT:                       return MTL::PixelFormatASTC_10x6_HDR;
            case ImageFormat::ASTC_10x8_SFLOAT_BLOCK_EXT:                       return MTL::PixelFormatASTC_10x8_HDR;
            case ImageFormat::ASTC_10x10_SFLOAT_BLOCK_EXT:                      return MTL::PixelFormatASTC_10x10_HDR;
            case ImageFormat::ASTC_12x10_SFLOAT_BLOCK_EXT:                      return MTL::PixelFormatASTC_12x10_HDR;
            case ImageFormat::ASTC_12x12_SFLOAT_BLOCK_EXT:                      return MTL::PixelFormatASTC_12x12_HDR;
            case ImageFormat::G8B8G8R8_422_UNORM_KHR:                           return MTL::PixelFormatInvalid;
            case ImageFormat::B8G8R8G8_422_UNORM_KHR:                           return MTL::PixelFormatInvalid;
            case ImageFormat::G8_B8_R8_3PLANE_420_UNORM_KHR:                    return MTL::PixelFormatInvalid;
            case ImageFormat::G8_B8R8_2PLANE_420_UNORM_KHR:                     return MTL::PixelFormatInvalid;
            case ImageFormat::G8_B8_R8_3PLANE_422_UNORM_KHR:                    return MTL::PixelFormatInvalid;
            case ImageFormat::G8_B8R8_2PLANE_422_UNORM_KHR:                     return MTL::PixelFormatInvalid;
            case ImageFormat::G8_B8_R8_3PLANE_444_UNORM_KHR:                    return MTL::PixelFormatInvalid;
            case ImageFormat::R10X6_UNORM_PACK16_KHR:                           return MTL::PixelFormatInvalid;
            case ImageFormat::R10X6G10X6_UNORM_2PACK16_KHR:                     return MTL::PixelFormatInvalid;
            case ImageFormat::R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR:           return MTL::PixelFormatInvalid;
            case ImageFormat::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR:       return MTL::PixelFormatInvalid;
            case ImageFormat::B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR:       return MTL::PixelFormatInvalid;
            case ImageFormat::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR:   return MTL::PixelFormatInvalid;
            case ImageFormat::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR:    return MTL::PixelFormatInvalid;
            case ImageFormat::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR:   return MTL::PixelFormatInvalid;
            case ImageFormat::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR:    return MTL::PixelFormatInvalid;
            case ImageFormat::G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR:   return MTL::PixelFormatInvalid;
            case ImageFormat::R12X4_UNORM_PACK16_KHR:                           return MTL::PixelFormatInvalid;
            case ImageFormat::R12X4G12X4_UNORM_2PACK16_KHR:                     return MTL::PixelFormatInvalid;
            case ImageFormat::R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR:           return MTL::PixelFormatInvalid;
            case ImageFormat::G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR:       return MTL::PixelFormatInvalid;
            case ImageFormat::B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR:       return MTL::PixelFormatInvalid;
            case ImageFormat::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR:   return MTL::PixelFormatInvalid;
            case ImageFormat::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR:    return MTL::PixelFormatInvalid;
            case ImageFormat::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR:   return MTL::PixelFormatInvalid;
            case ImageFormat::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR:    return MTL::PixelFormatInvalid;
            case ImageFormat::G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR:   return MTL::PixelFormatInvalid;
            case ImageFormat::G16B16G16R16_422_UNORM_KHR:                       return MTL::PixelFormatInvalid;
            case ImageFormat::B16G16R16G16_422_UNORM_KHR:                       return MTL::PixelFormatInvalid;
            case ImageFormat::G16_B16_R16_3PLANE_420_UNORM_KHR:                 return MTL::PixelFormatInvalid;
            case ImageFormat::G16_B16R16_2PLANE_420_UNORM_KHR:                  return MTL::PixelFormatInvalid;
            case ImageFormat::G16_B16_R16_3PLANE_422_UNORM_KHR:                 return MTL::PixelFormatInvalid;
            case ImageFormat::G16_B16R16_2PLANE_422_UNORM_KHR:                  return MTL::PixelFormatInvalid;
            case ImageFormat::G16_B16_R16_3PLANE_444_UNORM_KHR:                 return MTL::PixelFormatInvalid;
            case ImageFormat::G8_B8R8_2PLANE_444_UNORM_EXT:                     return MTL::PixelFormatInvalid;
            case ImageFormat::G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT:    return MTL::PixelFormatInvalid;
            case ImageFormat::G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT:    return MTL::PixelFormatInvalid;
            case ImageFormat::G16_B16R16_2PLANE_444_UNORM_EXT:                  return MTL::PixelFormatInvalid;
            case ImageFormat::A4R4G4B4_UNORM_PACK16_EXT:                        return MTL::PixelFormatInvalid;
            case ImageFormat::A4B4G4R4_UNORM_PACK16_EXT:                        return MTL::PixelFormatInvalid;
            case ImageFormat::R16G16_S10_5_NV:                                  return MTL::PixelFormatInvalid;

            case ImageFormat::MAX_ENUM: // Fallthrough
            default: AE_ERROR("Invalid Image Format Given!"); return MTL::PixelFormatInvalid;
        }
    }


    MTL::TextureUsage ConvertImageUsageToMTLTextureUsage(ImageUsageFlags imageUsageFlags)
    {
        MTL::TextureUsage textureUsage = 0;

        if (imageUsageFlags & IMAGE_USAGE_TRANSFER_SRC_BIT)                           { /* n/a */ }
        if (imageUsageFlags & IMAGE_USAGE_TRANSFER_DST_BIT)                           { /* n/a */ }
        if (imageUsageFlags & IMAGE_USAGE_SAMPLED_BIT)                                { textureUsage |= MTL::TextureUsageShaderRead; }
        if (imageUsageFlags & IMAGE_USAGE_STORAGE_BIT)                                { textureUsage |= MTL::TextureUsageShaderRead | MTL::TextureUsageShaderWrite; }
        if (imageUsageFlags & IMAGE_USAGE_COLOR_ATTACHMENT_BIT)                       { textureUsage |= MTL::TextureUsageRenderTarget; }
        if (imageUsageFlags & IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)               { textureUsage |= MTL::TextureUsageRenderTarget; }
        if (imageUsageFlags & IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)                   { /* n/a */ }
        if (imageUsageFlags & IMAGE_USAGE_INPUT_ATTACHMENT_BIT)                       { /* n/a */ }

        return textureUsage;
    }


    MTL::TextureType ConvertTextureTypeToMTLTextureType(TextureType textureType)
    {
        switch (textureType)
        {
            case TextureType::IMAGE_1D:         return MTL::TextureType1D;
            case TextureType::IMAGE_2D:         return MTL::TextureType2D;
            case TextureType::IMAGE_3D:         return MTL::TextureType3D;
            case TextureType::CUBEMAP:          return MTL::TextureTypeCube;
            case TextureType::IMAGE_2D_ARRAY:   return MTL::TextureType2DArray;
            default: AE_ERROR("Unsupported texture type given!")
        }

    }


    MTL::SamplerAddressMode ConvertSamplerAddressModeToMTLSamplerAddressMode(SamplerAddressMode samplerAddressMode)
    {
        switch (samplerAddressMode)
        {
            case SamplerAddressMode::REPEAT:                return MTL::SamplerAddressModeRepeat;
            case SamplerAddressMode::MIRRORED_REPEAT:       return MTL::SamplerAddressModeMirrorRepeat;
            case SamplerAddressMode::CLAMP_TO_EDGE:         return MTL::SamplerAddressModeClampToEdge;
            case SamplerAddressMode::CLAMP_TO_BORDER:       return MTL::SamplerAddressModeClampToBorderColor;
            case SamplerAddressMode::MIRROR_CLAMP_TO_EDGE:  return MTL::SamplerAddressModeMirrorClampToEdge;
            default: AE_ERROR("Unsupported SamplerAddressMode given!")
        }
    }


    MTL::SamplerMinMagFilter ConvertSamplerFilterToMTLMinMagFilter(SamplerFilter samplerFilter)
    {
        switch (samplerFilter)
        {
            case SamplerFilter::NEAREST: return MTL::SamplerMinMagFilterNearest;
            case SamplerFilter::LINEAR: return MTL::SamplerMinMagFilterLinear;
            default: AE_ERROR("Unsupported SamplerFilter given!")
        }
    }

}

