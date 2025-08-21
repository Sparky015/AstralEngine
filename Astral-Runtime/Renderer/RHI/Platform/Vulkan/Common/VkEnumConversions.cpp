/**
* @file VkEnumConversions.cpp
* @author Andrew Fagan
* @date 6/11/2025
*/

#include "VkEnumConversions.h"

namespace Astral {
    VkFormat ConvertImageFormatToVkFormat(ImageFormat imageFormat)
    {
        switch (imageFormat)
        {
            case ImageFormat::UNDEFINED:                                        return VK_FORMAT_UNDEFINED;
            case ImageFormat::R4G4_UNORM_PACK8:                                 return VK_FORMAT_R4G4_UNORM_PACK8;
            case ImageFormat::R4G4B4A4_UNORM_PACK16:                            return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
            case ImageFormat::B4G4R4A4_UNORM_PACK16:                            return VK_FORMAT_B4G4R4A4_UNORM_PACK16;
            case ImageFormat::R5G6B5_UNORM_PACK16:                              return VK_FORMAT_R5G6B5_UNORM_PACK16;
            case ImageFormat::B5G6R5_UNORM_PACK16:                              return VK_FORMAT_B5G6R5_UNORM_PACK16;
            case ImageFormat::R5G5B5A1_UNORM_PACK16:                            return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
            case ImageFormat::B5G5R5A1_UNORM_PACK16:                            return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
            case ImageFormat::A1R5G5B5_UNORM_PACK16:                            return VK_FORMAT_A1R5G5B5_UNORM_PACK16;
            case ImageFormat::R8_UNORM:                                         return VK_FORMAT_R8_UNORM;
            case ImageFormat::R8_SNORM:                                         return VK_FORMAT_R8_SNORM;
            case ImageFormat::R8_USCALED:                                       return VK_FORMAT_R8_USCALED;
            case ImageFormat::R8_SSCALED:                                       return VK_FORMAT_R8_SSCALED;
            case ImageFormat::R8_UINT:                                          return VK_FORMAT_R8_UINT;
            case ImageFormat::R8_SINT:                                          return VK_FORMAT_R8_SINT;
            case ImageFormat::R8_SRGB:                                          return VK_FORMAT_R8_SRGB;
            case ImageFormat::R8G8_UNORM:                                       return VK_FORMAT_R8G8_UNORM;
            case ImageFormat::R8G8_SNORM:                                       return VK_FORMAT_R8G8_SNORM;
            case ImageFormat::R8G8_USCALED:                                     return VK_FORMAT_R8G8_USCALED;
            case ImageFormat::R8G8_SSCALED:                                     return VK_FORMAT_R8G8_SSCALED;
            case ImageFormat::R8G8_UINT:                                        return VK_FORMAT_R8G8_UINT;
            case ImageFormat::R8G8_SINT:                                        return VK_FORMAT_R8G8_SINT;
            case ImageFormat::R8G8_SRGB:                                        return VK_FORMAT_R8G8_SRGB;
            case ImageFormat::R8G8B8_UNORM:                                     return VK_FORMAT_R8G8B8_UNORM;
            case ImageFormat::R8G8B8_SNORM:                                     return VK_FORMAT_R8G8B8_SNORM;
            case ImageFormat::R8G8B8_USCALED:                                   return VK_FORMAT_R8G8B8_USCALED;
            case ImageFormat::R8G8B8_SSCALED:                                   return VK_FORMAT_R8G8B8_SSCALED;
            case ImageFormat::R8G8B8_UINT:                                      return VK_FORMAT_R8G8B8_UINT;
            case ImageFormat::R8G8B8_SINT:                                      return VK_FORMAT_R8G8B8_SINT;
            case ImageFormat::R8G8B8_SRGB:                                      return VK_FORMAT_R8G8B8_SRGB;
            case ImageFormat::B8G8R8_UNORM:                                     return VK_FORMAT_B8G8R8_UNORM;
            case ImageFormat::B8G8R8_SNORM:                                     return VK_FORMAT_B8G8R8_SNORM;
            case ImageFormat::B8G8R8_USCALED:                                   return VK_FORMAT_B8G8R8_USCALED;
            case ImageFormat::B8G8R8_SSCALED:                                   return VK_FORMAT_B8G8R8_SSCALED;
            case ImageFormat::B8G8R8_UINT:                                      return VK_FORMAT_B8G8R8_UINT;
            case ImageFormat::B8G8R8_SINT:                                      return VK_FORMAT_B8G8R8_SINT;
            case ImageFormat::B8G8R8_SRGB:                                      return VK_FORMAT_B8G8R8_SRGB;
            case ImageFormat::R8G8B8A8_UNORM:                                   return VK_FORMAT_R8G8B8A8_UNORM;
            case ImageFormat::R8G8B8A8_SNORM:                                   return VK_FORMAT_R8G8B8A8_SNORM;
            case ImageFormat::R8G8B8A8_USCALED:                                 return VK_FORMAT_R8G8B8A8_USCALED;
            case ImageFormat::R8G8B8A8_SSCALED:                                 return VK_FORMAT_R8G8B8A8_SSCALED;
            case ImageFormat::R8G8B8A8_UINT:                                    return VK_FORMAT_R8G8B8A8_UINT;
            case ImageFormat::R8G8B8A8_SINT:                                    return VK_FORMAT_R8G8B8A8_SINT;
            case ImageFormat::R8G8B8A8_SRGB:                                    return VK_FORMAT_R8G8B8A8_SRGB;
            case ImageFormat::B8G8R8A8_UNORM:                                   return VK_FORMAT_B8G8R8A8_UNORM;
            case ImageFormat::B8G8R8A8_SNORM:                                   return VK_FORMAT_B8G8R8A8_SNORM;
            case ImageFormat::B8G8R8A8_USCALED:                                 return VK_FORMAT_B8G8R8A8_USCALED;
            case ImageFormat::B8G8R8A8_SSCALED:                                 return VK_FORMAT_B8G8R8A8_SSCALED;
            case ImageFormat::B8G8R8A8_UINT:                                    return VK_FORMAT_B8G8R8A8_UINT;
            case ImageFormat::B8G8R8A8_SINT:                                    return VK_FORMAT_B8G8R8A8_SINT;
            case ImageFormat::B8G8R8A8_SRGB:                                    return VK_FORMAT_B8G8R8A8_SRGB;
            case ImageFormat::A8B8G8R8_UNORM_PACK32:                            return VK_FORMAT_A8B8G8R8_UNORM_PACK32;
            case ImageFormat::A8B8G8R8_SNORM_PACK32:                            return VK_FORMAT_A8B8G8R8_SNORM_PACK32;
            case ImageFormat::A8B8G8R8_USCALED_PACK32:                          return VK_FORMAT_A8B8G8R8_USCALED_PACK32;
            case ImageFormat::A8B8G8R8_SSCALED_PACK32:                          return VK_FORMAT_A8B8G8R8_SSCALED_PACK32;
            case ImageFormat::A8B8G8R8_UINT_PACK32:                             return VK_FORMAT_A8B8G8R8_UINT_PACK32;
            case ImageFormat::A8B8G8R8_SINT_PACK32:                             return VK_FORMAT_A8B8G8R8_SINT_PACK32;
            case ImageFormat::A8B8G8R8_SRGB_PACK32:                             return VK_FORMAT_A8B8G8R8_SRGB_PACK32;
            case ImageFormat::A2R10G10B10_UNORM_PACK32:                         return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
            case ImageFormat::A2R10G10B10_SNORM_PACK32:                         return VK_FORMAT_A2R10G10B10_SNORM_PACK32;
            case ImageFormat::A2R10G10B10_USCALED_PACK32:                       return VK_FORMAT_A2R10G10B10_USCALED_PACK32;
            case ImageFormat::A2R10G10B10_SSCALED_PACK32:                       return VK_FORMAT_A2R10G10B10_SSCALED_PACK32;
            case ImageFormat::A2R10G10B10_UINT_PACK32:                          return VK_FORMAT_A2R10G10B10_UINT_PACK32;
            case ImageFormat::A2R10G10B10_SINT_PACK32:                          return VK_FORMAT_A2R10G10B10_SINT_PACK32;
            case ImageFormat::A2B10G10R10_UNORM_PACK32:                         return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
            case ImageFormat::A2B10G10R10_SNORM_PACK32:                         return VK_FORMAT_A2B10G10R10_SNORM_PACK32;
            case ImageFormat::A2B10G10R10_USCALED_PACK32:                       return VK_FORMAT_A2B10G10R10_USCALED_PACK32;
            case ImageFormat::A2B10G10R10_SSCALED_PACK32:                       return VK_FORMAT_A2B10G10R10_SSCALED_PACK32;
            case ImageFormat::A2B10G10R10_UINT_PACK32:                          return VK_FORMAT_A2B10G10R10_UINT_PACK32;
            case ImageFormat::A2B10G10R10_SINT_PACK32:                          return VK_FORMAT_A2B10G10R10_SINT_PACK32;
            case ImageFormat::R16_UNORM:                                        return VK_FORMAT_R16_UNORM;
            case ImageFormat::R16_SNORM:                                        return VK_FORMAT_R16_SNORM;
            case ImageFormat::R16_USCALED:                                      return VK_FORMAT_R16_USCALED;
            case ImageFormat::R16_SSCALED:                                      return VK_FORMAT_R16_SSCALED;
            case ImageFormat::R16_UINT:                                         return VK_FORMAT_R16_UINT;
            case ImageFormat::R16_SINT:                                         return VK_FORMAT_R16_SINT;
            case ImageFormat::R16_SFLOAT:                                       return VK_FORMAT_R16_SFLOAT;
            case ImageFormat::R16G16_UNORM:                                     return VK_FORMAT_R16G16_UNORM;
            case ImageFormat::R16G16_SNORM:                                     return VK_FORMAT_R16G16_SNORM;
            case ImageFormat::R16G16_USCALED:                                   return VK_FORMAT_R16G16_USCALED;
            case ImageFormat::R16G16_SSCALED:                                   return VK_FORMAT_R16G16_SSCALED;
            case ImageFormat::R16G16_UINT:                                      return VK_FORMAT_R16G16_UINT;
            case ImageFormat::R16G16_SINT:                                      return VK_FORMAT_R16G16_SINT;
            case ImageFormat::R16G16_SFLOAT:                                    return VK_FORMAT_R16G16_SFLOAT;
            case ImageFormat::R16G16B16_UNORM:                                  return VK_FORMAT_R16G16B16_UNORM;
            case ImageFormat::R16G16B16_SNORM:                                  return VK_FORMAT_R16G16B16_SNORM;
            case ImageFormat::R16G16B16_USCALED:                                return VK_FORMAT_R16G16B16_USCALED;
            case ImageFormat::R16G16B16_SSCALED:                                return VK_FORMAT_R16G16B16_SSCALED;
            case ImageFormat::R16G16B16_UINT:                                   return VK_FORMAT_R16G16B16_UINT;
            case ImageFormat::R16G16B16_SINT:                                   return VK_FORMAT_R16G16B16_SINT;
            case ImageFormat::R16G16B16_SFLOAT:                                 return VK_FORMAT_R16G16B16_SFLOAT;
            case ImageFormat::R16G16B16A16_UNORM:                               return VK_FORMAT_R16G16B16A16_UNORM;
            case ImageFormat::R16G16B16A16_SNORM:                               return VK_FORMAT_R16G16B16A16_SNORM;
            case ImageFormat::R16G16B16A16_USCALED:                             return VK_FORMAT_R16G16B16A16_USCALED;
            case ImageFormat::R16G16B16A16_SSCALED:                             return VK_FORMAT_R16G16B16A16_SSCALED;
            case ImageFormat::R16G16B16A16_UINT:                                return VK_FORMAT_R16G16B16A16_UINT;
            case ImageFormat::R16G16B16A16_SINT:                                return VK_FORMAT_R16G16B16A16_SINT;
            case ImageFormat::R16G16B16A16_SFLOAT:                              return VK_FORMAT_R16G16B16A16_SFLOAT;
            case ImageFormat::R32_UINT:                                         return VK_FORMAT_R32_UINT;
            case ImageFormat::R32_SINT:                                         return VK_FORMAT_R32_SINT;
            case ImageFormat::R32_SFLOAT:                                       return VK_FORMAT_R32_SFLOAT;
            case ImageFormat::R32G32_UINT:                                      return VK_FORMAT_R32G32_UINT;
            case ImageFormat::R32G32_SINT:                                      return VK_FORMAT_R32G32_SINT;
            case ImageFormat::R32G32_SFLOAT:                                    return VK_FORMAT_R32G32_SFLOAT;
            case ImageFormat::R32G32B32_UINT:                                   return VK_FORMAT_R32G32B32_UINT;
            case ImageFormat::R32G32B32_SINT:                                   return VK_FORMAT_R32G32B32_SINT;
            case ImageFormat::R32G32B32_SFLOAT:                                 return VK_FORMAT_R32G32B32_SFLOAT;
            case ImageFormat::R32G32B32A32_UINT:                                return VK_FORMAT_R32G32B32A32_UINT;
            case ImageFormat::R32G32B32A32_SINT:                                return VK_FORMAT_R32G32B32A32_SINT;
            case ImageFormat::R32G32B32A32_SFLOAT:                              return VK_FORMAT_R32G32B32A32_SFLOAT;
            case ImageFormat::R64_UINT:                                         return VK_FORMAT_R64_UINT;
            case ImageFormat::R64_SINT:                                         return VK_FORMAT_R64_SINT;
            case ImageFormat::R64_SFLOAT:                                       return VK_FORMAT_R64_SFLOAT;
            case ImageFormat::R64G64_UINT:                                      return VK_FORMAT_R64G64_UINT;
            case ImageFormat::R64G64_SINT:                                      return VK_FORMAT_R64G64_SINT;
            case ImageFormat::R64G64_SFLOAT:                                    return VK_FORMAT_R64G64_SFLOAT;
            case ImageFormat::R64G64B64_UINT:                                   return VK_FORMAT_R64G64B64_UINT;
            case ImageFormat::R64G64B64_SINT:                                   return VK_FORMAT_R64G64B64_SINT;
            case ImageFormat::R64G64B64_SFLOAT:                                 return VK_FORMAT_R64G64B64_SFLOAT;
            case ImageFormat::R64G64B64A64_UINT:                                return VK_FORMAT_R64G64B64A64_UINT;
            case ImageFormat::R64G64B64A64_SINT:                                return VK_FORMAT_R64G64B64A64_SINT;
            case ImageFormat::R64G64B64A64_SFLOAT:                              return VK_FORMAT_R64G64B64A64_SFLOAT;
            case ImageFormat::B10G11R11_UFLOAT_PACK32:                          return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
            case ImageFormat::E5B9G9R9_UFLOAT_PACK32:                           return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
            case ImageFormat::D16_UNORM:                                        return VK_FORMAT_D16_UNORM;
            case ImageFormat::X8_D24_UNORM_PACK32:                              return VK_FORMAT_X8_D24_UNORM_PACK32;
            case ImageFormat::D32_SFLOAT:                                       return VK_FORMAT_D32_SFLOAT;
            case ImageFormat::S8_UINT:                                          return VK_FORMAT_S8_UINT;
            case ImageFormat::D16_UNORM_S8_UINT:                                return VK_FORMAT_D16_UNORM_S8_UINT;
            case ImageFormat::D24_UNORM_S8_UINT:                                return VK_FORMAT_D24_UNORM_S8_UINT;
            case ImageFormat::D32_SFLOAT_S8_UINT:                               return VK_FORMAT_D32_SFLOAT_S8_UINT;
            case ImageFormat::BC1_RGB_UNORM_BLOCK:                              return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
            case ImageFormat::BC1_RGB_SRGB_BLOCK:                               return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
            case ImageFormat::BC1_RGBA_UNORM_BLOCK:                             return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
            case ImageFormat::BC1_RGBA_SRGB_BLOCK:                              return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
            case ImageFormat::BC2_UNORM_BLOCK:                                  return VK_FORMAT_BC2_UNORM_BLOCK;
            case ImageFormat::BC2_SRGB_BLOCK:                                   return VK_FORMAT_BC2_SRGB_BLOCK;
            case ImageFormat::BC3_UNORM_BLOCK:                                  return VK_FORMAT_BC3_UNORM_BLOCK;
            case ImageFormat::BC3_SRGB_BLOCK:                                   return VK_FORMAT_BC3_SRGB_BLOCK;
            case ImageFormat::BC4_UNORM_BLOCK:                                  return VK_FORMAT_BC4_UNORM_BLOCK;
            case ImageFormat::BC4_SNORM_BLOCK:                                  return VK_FORMAT_BC4_SNORM_BLOCK;
            case ImageFormat::BC5_UNORM_BLOCK:                                  return VK_FORMAT_BC5_UNORM_BLOCK;
            case ImageFormat::BC5_SNORM_BLOCK:                                  return VK_FORMAT_BC5_SNORM_BLOCK;
            case ImageFormat::BC6H_UFLOAT_BLOCK:                                return VK_FORMAT_BC6H_UFLOAT_BLOCK;
            case ImageFormat::BC6H_SFLOAT_BLOCK:                                return VK_FORMAT_BC6H_SFLOAT_BLOCK;
            case ImageFormat::BC7_UNORM_BLOCK:                                  return VK_FORMAT_BC7_UNORM_BLOCK;
            case ImageFormat::BC7_SRGB_BLOCK:                                   return VK_FORMAT_BC7_SRGB_BLOCK;
            case ImageFormat::ETC2_R8G8B8_UNORM_BLOCK:                          return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
            case ImageFormat::ETC2_R8G8B8_SRGB_BLOCK:                           return VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
            case ImageFormat::ETC2_R8G8B8A1_UNORM_BLOCK:                        return VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
            case ImageFormat::ETC2_R8G8B8A1_SRGB_BLOCK:                         return VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
            case ImageFormat::ETC2_R8G8B8A8_UNORM_BLOCK:                        return VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
            case ImageFormat::ETC2_R8G8B8A8_SRGB_BLOCK:                         return VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
            case ImageFormat::EAC_R11_UNORM_BLOCK:                              return VK_FORMAT_EAC_R11_UNORM_BLOCK;
            case ImageFormat::EAC_R11_SNORM_BLOCK:                              return VK_FORMAT_EAC_R11_SNORM_BLOCK;
            case ImageFormat::EAC_R11G11_UNORM_BLOCK:                           return VK_FORMAT_EAC_R11G11_UNORM_BLOCK;
            case ImageFormat::EAC_R11G11_SNORM_BLOCK:                           return VK_FORMAT_EAC_R11G11_SNORM_BLOCK;
            case ImageFormat::ASTC_4x4_UNORM_BLOCK:                             return VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
            case ImageFormat::ASTC_4x4_SRGB_BLOCK:                              return VK_FORMAT_ASTC_4x4_SRGB_BLOCK;
            case ImageFormat::ASTC_5x4_UNORM_BLOCK:                             return VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
            case ImageFormat::ASTC_5x4_SRGB_BLOCK:                              return VK_FORMAT_ASTC_5x4_SRGB_BLOCK;
            case ImageFormat::ASTC_5x5_UNORM_BLOCK:                             return VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
            case ImageFormat::ASTC_5x5_SRGB_BLOCK:                              return VK_FORMAT_ASTC_5x5_SRGB_BLOCK;
            case ImageFormat::ASTC_6x5_UNORM_BLOCK:                             return VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
            case ImageFormat::ASTC_6x5_SRGB_BLOCK:                              return VK_FORMAT_ASTC_6x5_SRGB_BLOCK;
            case ImageFormat::ASTC_6x6_UNORM_BLOCK:                             return VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
            case ImageFormat::ASTC_6x6_SRGB_BLOCK:                              return VK_FORMAT_ASTC_6x6_SRGB_BLOCK;
            case ImageFormat::ASTC_8x5_UNORM_BLOCK:                             return VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
            case ImageFormat::ASTC_8x5_SRGB_BLOCK:                              return VK_FORMAT_ASTC_8x5_SRGB_BLOCK;
            case ImageFormat::ASTC_8x6_UNORM_BLOCK:                             return VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
            case ImageFormat::ASTC_8x6_SRGB_BLOCK:                              return VK_FORMAT_ASTC_8x6_SRGB_BLOCK;
            case ImageFormat::ASTC_8x8_UNORM_BLOCK:                             return VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
            case ImageFormat::ASTC_8x8_SRGB_BLOCK:                              return VK_FORMAT_ASTC_8x8_SRGB_BLOCK;
            case ImageFormat::ASTC_10x5_UNORM_BLOCK:                            return VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
            case ImageFormat::ASTC_10x5_SRGB_BLOCK:                             return VK_FORMAT_ASTC_10x5_SRGB_BLOCK;
            case ImageFormat::ASTC_10x6_UNORM_BLOCK:                            return VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
            case ImageFormat::ASTC_10x6_SRGB_BLOCK:                             return VK_FORMAT_ASTC_10x6_SRGB_BLOCK;
            case ImageFormat::ASTC_10x8_UNORM_BLOCK:                            return VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
            case ImageFormat::ASTC_10x8_SRGB_BLOCK:                             return VK_FORMAT_ASTC_10x8_SRGB_BLOCK;
            case ImageFormat::ASTC_10x10_UNORM_BLOCK:                           return VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
            case ImageFormat::ASTC_10x10_SRGB_BLOCK:                            return VK_FORMAT_ASTC_10x10_SRGB_BLOCK;
            case ImageFormat::ASTC_12x10_UNORM_BLOCK:                           return VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
            case ImageFormat::ASTC_12x10_SRGB_BLOCK:                            return VK_FORMAT_ASTC_12x10_SRGB_BLOCK;
            case ImageFormat::ASTC_12x12_UNORM_BLOCK:                           return VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
            case ImageFormat::ASTC_12x12_SRGB_BLOCK:                            return VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
            case ImageFormat::G8B8G8R8_422_UNORM:                               return VK_FORMAT_G8B8G8R8_422_UNORM;
            case ImageFormat::B8G8R8G8_422_UNORM:                               return VK_FORMAT_B8G8R8G8_422_UNORM;
            case ImageFormat::G8_B8_R8_3PLANE_420_UNORM:                        return VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM;
            case ImageFormat::G8_B8R8_2PLANE_420_UNORM:                         return VK_FORMAT_G8_B8R8_2PLANE_420_UNORM;
            case ImageFormat::G8_B8_R8_3PLANE_422_UNORM:                        return VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM;
            case ImageFormat::G8_B8R8_2PLANE_422_UNORM:                         return VK_FORMAT_G8_B8R8_2PLANE_422_UNORM;
            case ImageFormat::G8_B8_R8_3PLANE_444_UNORM:                        return VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM;
            case ImageFormat::R10X6_UNORM_PACK16:                               return VK_FORMAT_R10X6_UNORM_PACK16;
            case ImageFormat::R10X6G10X6_UNORM_2PACK16:                         return VK_FORMAT_R10X6G10X6_UNORM_2PACK16;
            case ImageFormat::R10X6G10X6B10X6A10X6_UNORM_4PACK16:               return VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16;
            case ImageFormat::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:           return VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16;
            case ImageFormat::B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:           return VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16;
            case ImageFormat::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:       return VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16;
            case ImageFormat::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:        return VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16;
            case ImageFormat::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:       return VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16;
            case ImageFormat::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:        return VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16;
            case ImageFormat::G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:       return VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16;
            case ImageFormat::R12X4_UNORM_PACK16:                               return VK_FORMAT_R12X4_UNORM_PACK16;
            case ImageFormat::R12X4G12X4_UNORM_2PACK16:                         return VK_FORMAT_R12X4G12X4_UNORM_2PACK16;
            case ImageFormat::R12X4G12X4B12X4A12X4_UNORM_4PACK16:               return VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16;
            case ImageFormat::G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:           return VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16;
            case ImageFormat::B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:           return VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16;
            case ImageFormat::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:       return VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16;
            case ImageFormat::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:        return VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16;
            case ImageFormat::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:       return VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16;
            case ImageFormat::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:        return VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16;
            case ImageFormat::G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:       return VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16;
            case ImageFormat::G16B16G16R16_422_UNORM:                           return VK_FORMAT_G16B16G16R16_422_UNORM;
            case ImageFormat::B16G16R16G16_422_UNORM:                           return VK_FORMAT_B16G16R16G16_422_UNORM;
            case ImageFormat::G16_B16_R16_3PLANE_420_UNORM:                     return VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM;
            case ImageFormat::G16_B16R16_2PLANE_420_UNORM:                      return VK_FORMAT_G16_B16R16_2PLANE_420_UNORM;
            case ImageFormat::G16_B16_R16_3PLANE_422_UNORM:                     return VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM;
            case ImageFormat::G16_B16R16_2PLANE_422_UNORM:                      return VK_FORMAT_G16_B16R16_2PLANE_422_UNORM;
            case ImageFormat::G16_B16_R16_3PLANE_444_UNORM:                     return VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM;
            case ImageFormat::G8_B8R8_2PLANE_444_UNORM:                         return VK_FORMAT_G8_B8R8_2PLANE_444_UNORM;
            case ImageFormat::G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16:        return VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16;
            case ImageFormat::G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16:        return VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16;
            case ImageFormat::G16_B16R16_2PLANE_444_UNORM:                      return VK_FORMAT_G16_B16R16_2PLANE_444_UNORM;
            case ImageFormat::A4R4G4B4_UNORM_PACK16:                            return VK_FORMAT_A4R4G4B4_UNORM_PACK16;
            case ImageFormat::A4B4G4R4_UNORM_PACK16:                            return VK_FORMAT_A4B4G4R4_UNORM_PACK16;
            case ImageFormat::ASTC_4x4_SFLOAT_BLOCK:                            return VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK;
            case ImageFormat::ASTC_5x4_SFLOAT_BLOCK:                            return VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK;
            case ImageFormat::ASTC_5x5_SFLOAT_BLOCK:                            return VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK;
            case ImageFormat::ASTC_6x5_SFLOAT_BLOCK:                            return VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK;
            case ImageFormat::ASTC_6x6_SFLOAT_BLOCK:                            return VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK;
            case ImageFormat::ASTC_8x5_SFLOAT_BLOCK:                            return VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK;
            case ImageFormat::ASTC_8x6_SFLOAT_BLOCK:                            return VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK;
            case ImageFormat::ASTC_8x8_SFLOAT_BLOCK:                            return VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK;
            case ImageFormat::ASTC_10x5_SFLOAT_BLOCK:                           return VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK;
            case ImageFormat::ASTC_10x6_SFLOAT_BLOCK:                           return VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK;
            case ImageFormat::ASTC_10x8_SFLOAT_BLOCK:                           return VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK;
            case ImageFormat::ASTC_10x10_SFLOAT_BLOCK:                          return VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK;
            case ImageFormat::ASTC_12x10_SFLOAT_BLOCK:                          return VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK;
            case ImageFormat::ASTC_12x12_SFLOAT_BLOCK:                          return VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK;
            case ImageFormat::PVRTC1_2BPP_UNORM_BLOCK_IMG:                      return VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG;
            case ImageFormat::PVRTC1_4BPP_UNORM_BLOCK_IMG:                      return VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG;
            case ImageFormat::PVRTC2_2BPP_UNORM_BLOCK_IMG:                      return VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG;
            case ImageFormat::PVRTC2_4BPP_UNORM_BLOCK_IMG:                      return VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG;
            case ImageFormat::PVRTC1_2BPP_SRGB_BLOCK_IMG:                       return VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG;
            case ImageFormat::PVRTC1_4BPP_SRGB_BLOCK_IMG:                       return VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG;
            case ImageFormat::PVRTC2_2BPP_SRGB_BLOCK_IMG:                       return VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG;
            case ImageFormat::PVRTC2_4BPP_SRGB_BLOCK_IMG:                       return VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG;
            case ImageFormat::R16G16_SFIXED5_NV:                                return VK_FORMAT_R16G16_SFIXED5_NV;
            case ImageFormat::A1B5G5R5_UNORM_PACK16_KHR:                        return VK_FORMAT_A1B5G5R5_UNORM_PACK16_KHR;
            case ImageFormat::A8_UNORM_KHR:                                     return VK_FORMAT_A8_UNORM_KHR;
            case ImageFormat::ASTC_4x4_SFLOAT_BLOCK_EXT:                        return VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK_EXT;
            case ImageFormat::ASTC_5x4_SFLOAT_BLOCK_EXT:                        return VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK_EXT;
            case ImageFormat::ASTC_5x5_SFLOAT_BLOCK_EXT:                        return VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK_EXT;
            case ImageFormat::ASTC_6x5_SFLOAT_BLOCK_EXT:                        return VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK_EXT;
            case ImageFormat::ASTC_6x6_SFLOAT_BLOCK_EXT:                        return VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK_EXT;
            case ImageFormat::ASTC_8x5_SFLOAT_BLOCK_EXT:                        return VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK_EXT;
            case ImageFormat::ASTC_8x6_SFLOAT_BLOCK_EXT:                        return VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK_EXT;
            case ImageFormat::ASTC_8x8_SFLOAT_BLOCK_EXT:                        return VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK_EXT;
            case ImageFormat::ASTC_10x5_SFLOAT_BLOCK_EXT:                       return VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK_EXT;
            case ImageFormat::ASTC_10x6_SFLOAT_BLOCK_EXT:                       return VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK_EXT;
            case ImageFormat::ASTC_10x8_SFLOAT_BLOCK_EXT:                       return VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK_EXT;
            case ImageFormat::ASTC_10x10_SFLOAT_BLOCK_EXT:                      return VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK_EXT;
            case ImageFormat::ASTC_12x10_SFLOAT_BLOCK_EXT:                      return VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK_EXT;
            case ImageFormat::ASTC_12x12_SFLOAT_BLOCK_EXT:                      return VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK_EXT;
            case ImageFormat::G8B8G8R8_422_UNORM_KHR:                           return VK_FORMAT_G8B8G8R8_422_UNORM_KHR;
            case ImageFormat::B8G8R8G8_422_UNORM_KHR:                           return VK_FORMAT_B8G8R8G8_422_UNORM_KHR;
            case ImageFormat::G8_B8_R8_3PLANE_420_UNORM_KHR:                    return VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM_KHR;
            case ImageFormat::G8_B8R8_2PLANE_420_UNORM_KHR:                     return VK_FORMAT_G8_B8R8_2PLANE_420_UNORM_KHR;
            case ImageFormat::G8_B8_R8_3PLANE_422_UNORM_KHR:                    return VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM_KHR;
            case ImageFormat::G8_B8R8_2PLANE_422_UNORM_KHR:                     return VK_FORMAT_G8_B8R8_2PLANE_422_UNORM_KHR;
            case ImageFormat::G8_B8_R8_3PLANE_444_UNORM_KHR:                    return VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM_KHR;
            case ImageFormat::R10X6_UNORM_PACK16_KHR:                           return VK_FORMAT_R10X6_UNORM_PACK16_KHR;
            case ImageFormat::R10X6G10X6_UNORM_2PACK16_KHR:                     return VK_FORMAT_R10X6G10X6_UNORM_2PACK16_KHR;
            case ImageFormat::R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR:           return VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16_KHR;
            case ImageFormat::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR:       return VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16_KHR;
            case ImageFormat::B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR:       return VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16_KHR;
            case ImageFormat::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR:   return VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16_KHR;
            case ImageFormat::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR:    return VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16_KHR;
            case ImageFormat::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR:   return VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16_KHR;
            case ImageFormat::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR:    return VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16_KHR;
            case ImageFormat::G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR:   return VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16_KHR;
            case ImageFormat::R12X4_UNORM_PACK16_KHR:                           return VK_FORMAT_R12X4_UNORM_PACK16_KHR;
            case ImageFormat::R12X4G12X4_UNORM_2PACK16_KHR:                     return VK_FORMAT_R12X4G12X4_UNORM_2PACK16_KHR;
            case ImageFormat::R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR:           return VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16_KHR;
            case ImageFormat::G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR:       return VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16_KHR;
            case ImageFormat::B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR:       return VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16_KHR;
            case ImageFormat::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR:   return VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16_KHR;
            case ImageFormat::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR:    return VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16_KHR;
            case ImageFormat::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR:   return VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16_KHR;
            case ImageFormat::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR:    return VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16_KHR;
            case ImageFormat::G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR:   return VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16_KHR;
            case ImageFormat::G16B16G16R16_422_UNORM_KHR:                       return VK_FORMAT_G16B16G16R16_422_UNORM_KHR;
            case ImageFormat::B16G16R16G16_422_UNORM_KHR:                       return VK_FORMAT_B16G16R16G16_422_UNORM_KHR;
            case ImageFormat::G16_B16_R16_3PLANE_420_UNORM_KHR:                 return VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM_KHR;
            case ImageFormat::G16_B16R16_2PLANE_420_UNORM_KHR:                  return VK_FORMAT_G16_B16R16_2PLANE_420_UNORM_KHR;
            case ImageFormat::G16_B16_R16_3PLANE_422_UNORM_KHR:                 return VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM_KHR;
            case ImageFormat::G16_B16R16_2PLANE_422_UNORM_KHR:                  return VK_FORMAT_G16_B16R16_2PLANE_422_UNORM_KHR;
            case ImageFormat::G16_B16_R16_3PLANE_444_UNORM_KHR:                 return VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM_KHR;
            case ImageFormat::G8_B8R8_2PLANE_444_UNORM_EXT:                     return VK_FORMAT_G8_B8R8_2PLANE_444_UNORM_EXT;
            case ImageFormat::G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT:    return VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16_EXT;
            case ImageFormat::G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT:    return VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16_EXT;
            case ImageFormat::G16_B16R16_2PLANE_444_UNORM_EXT:                  return VK_FORMAT_G16_B16R16_2PLANE_444_UNORM_EXT;
            case ImageFormat::A4R4G4B4_UNORM_PACK16_EXT:                        return VK_FORMAT_A4R4G4B4_UNORM_PACK16_EXT;
            case ImageFormat::A4B4G4R4_UNORM_PACK16_EXT:                        return VK_FORMAT_A4B4G4R4_UNORM_PACK16_EXT;
            case ImageFormat::R16G16_S10_5_NV:                                  return VK_FORMAT_R16G16_S10_5_NV;

            case ImageFormat::MAX_ENUM: // Fallthrough
            default: ASTRAL_ERROR("Invalid Image Format Given!");
        }
    }


    VkImageLayout ConvertImageLayoutToVkImageLayout(ImageLayout imageLayout)
    {
        switch (imageLayout)
        {
            case ImageLayout::UNDEFINED:                                       return VK_IMAGE_LAYOUT_UNDEFINED;
            case ImageLayout::GENERAL:                                         return VK_IMAGE_LAYOUT_GENERAL;
            case ImageLayout::COLOR_ATTACHMENT_OPTIMAL:                        return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL:                return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            case ImageLayout::DEPTH_STENCIL_READ_ONLY_OPTIMAL:                 return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            case ImageLayout::SHADER_READ_ONLY_OPTIMAL:                        return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            case ImageLayout::TRANSFER_SRC_OPTIMAL:                            return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            case ImageLayout::TRANSFER_DST_OPTIMAL:                            return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            case ImageLayout::PREINITIALIZED:                                  return VK_IMAGE_LAYOUT_PREINITIALIZED;
            case ImageLayout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:      return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
            case ImageLayout::DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:      return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
            case ImageLayout::DEPTH_ATTACHMENT_OPTIMAL:                        return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
            case ImageLayout::DEPTH_READ_ONLY_OPTIMAL:                         return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
            case ImageLayout::STENCIL_ATTACHMENT_OPTIMAL:                      return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
            case ImageLayout::STENCIL_READ_ONLY_OPTIMAL:                       return VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
            case ImageLayout::READ_ONLY_OPTIMAL:                               return VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
            case ImageLayout::ATTACHMENT_OPTIMAL:                              return VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
            case ImageLayout::PRESENT_SRC_KHR:                                 return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            case ImageLayout::VIDEO_DECODE_DST_KHR:                            return VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR;
            case ImageLayout::VIDEO_DECODE_SRC_KHR:                            return VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR;
            case ImageLayout::VIDEO_DECODE_DPB_KHR:                            return VK_IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR;
            case ImageLayout::SHARED_PRESENT_KHR:                              return VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR;
            case ImageLayout::FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:                return VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT;
            case ImageLayout::FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR:    return VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR;
            case ImageLayout::RENDERING_LOCAL_READ_KHR:                        return VK_IMAGE_LAYOUT_RENDERING_LOCAL_READ_KHR;
            case ImageLayout::VIDEO_ENCODE_DST_KHR:                            return VK_IMAGE_LAYOUT_VIDEO_ENCODE_DST_KHR;
            case ImageLayout::VIDEO_ENCODE_SRC_KHR:                            return VK_IMAGE_LAYOUT_VIDEO_ENCODE_SRC_KHR;
            case ImageLayout::VIDEO_ENCODE_DPB_KHR:                            return VK_IMAGE_LAYOUT_VIDEO_ENCODE_DPB_KHR;
            case ImageLayout::ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT:            return VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT;
            case ImageLayout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR:  return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL_KHR;
            case ImageLayout::DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR:  return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL_KHR;
            case ImageLayout::SHADING_RATE_OPTIMAL_NV:                         return VK_IMAGE_LAYOUT_SHADING_RATE_OPTIMAL_NV;
            case ImageLayout::DEPTH_ATTACHMENT_OPTIMAL_KHR:                    return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL_KHR;
            case ImageLayout::DEPTH_READ_ONLY_OPTIMAL_KHR:                     return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL_KHR;
            case ImageLayout::STENCIL_ATTACHMENT_OPTIMAL_KHR:                  return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL_KHR;
            case ImageLayout::STENCIL_READ_ONLY_OPTIMAL_KHR:                   return VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL_KHR;
            case ImageLayout::READ_ONLY_OPTIMAL_KHR:                           return VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL_KHR;
            case ImageLayout::ATTACHMENT_OPTIMAL_KHR:                          return VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR;

            case Astral::ImageLayout::MAX_ENUM: // Fallthrough
                default: ASTRAL_ERROR("Invalid Image Layout Given!");
        }
    }


    VkAccessFlags ConvertAccessFlagsToVkAccessFlags(AccessFlags accessFlags)
    {
        VkAccessFlags vkAccessFlags = 0;

        if (accessFlags & ACCESS_FLAGS_INDIRECT_COMMAND_READ_BIT)                  { vkAccessFlags |= VK_ACCESS_INDIRECT_COMMAND_READ_BIT; }
        if (accessFlags & ACCESS_FLAGS_INDEX_READ_BIT)                             { vkAccessFlags |= VK_ACCESS_INDEX_READ_BIT; }
        if (accessFlags & ACCESS_FLAGS_VERTEX_ATTRIBUTE_READ_BIT)                  { vkAccessFlags |= VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT; }
        if (accessFlags & ACCESS_FLAGS_UNIFORM_READ_BIT)                           { vkAccessFlags |= VK_ACCESS_UNIFORM_READ_BIT; }
        if (accessFlags & ACCESS_FLAGS_INPUT_ATTACHMENT_READ_BIT)                  { vkAccessFlags |= VK_ACCESS_INPUT_ATTACHMENT_READ_BIT; }
        if (accessFlags & ACCESS_FLAGS_SHADER_READ_BIT)                            { vkAccessFlags |= VK_ACCESS_SHADER_READ_BIT; }
        if (accessFlags & ACCESS_FLAGS_SHADER_WRITE_BIT)                           { vkAccessFlags |= VK_ACCESS_SHADER_WRITE_BIT; }
        if (accessFlags & ACCESS_FLAGS_COLOR_ATTACHMENT_READ_BIT)                  { vkAccessFlags |= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT; }
        if (accessFlags & ACCESS_FLAGS_COLOR_ATTACHMENT_WRITE_BIT)                 { vkAccessFlags |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; }
        if (accessFlags & ACCESS_FLAGS_DEPTH_STENCIL_ATTACHMENT_READ_BIT)          { vkAccessFlags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT; }
        if (accessFlags & ACCESS_FLAGS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT)         { vkAccessFlags |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT; }
        if (accessFlags & ACCESS_FLAGS_TRANSFER_READ_BIT)                          { vkAccessFlags |= VK_ACCESS_TRANSFER_READ_BIT; }
        if (accessFlags & ACCESS_FLAGS_TRANSFER_WRITE_BIT)                         { vkAccessFlags |= VK_ACCESS_TRANSFER_WRITE_BIT; }
        if (accessFlags & ACCESS_FLAGS_HOST_READ_BIT)                              { vkAccessFlags |= VK_ACCESS_HOST_READ_BIT; }
        if (accessFlags & ACCESS_FLAGS_HOST_WRITE_BIT)                             { vkAccessFlags |= VK_ACCESS_HOST_WRITE_BIT; }
        if (accessFlags & ACCESS_FLAGS_MEMORY_READ_BIT)                            { vkAccessFlags |= VK_ACCESS_MEMORY_READ_BIT; }
        if (accessFlags & ACCESS_FLAGS_MEMORY_WRITE_BIT)                           { vkAccessFlags |= VK_ACCESS_MEMORY_WRITE_BIT; }
        if (accessFlags & ACCESS_FLAGS_NONE)                                       { vkAccessFlags |= VK_ACCESS_NONE; }
        if (accessFlags & ACCESS_FLAGS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT)           { vkAccessFlags |= VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT; }
        if (accessFlags & ACCESS_FLAGS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT)    { vkAccessFlags |= VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT; }
        if (accessFlags & ACCESS_FLAGS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT)   { vkAccessFlags |= VK_ACCESS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT; }
        if (accessFlags & ACCESS_FLAGS_CONDITIONAL_RENDERING_READ_BIT_EXT)         { vkAccessFlags |= VK_ACCESS_CONDITIONAL_RENDERING_READ_BIT_EXT; }
        if (accessFlags & ACCESS_FLAGS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT)  { vkAccessFlags |= VK_ACCESS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT; }
        if (accessFlags & ACCESS_FLAGS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT)          { vkAccessFlags |= VK_ACCESS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT; }
        if (accessFlags & ACCESS_FLAGS_COMMAND_PREPROCESS_READ_BIT_EXT)            { vkAccessFlags |= VK_ACCESS_COMMAND_PREPROCESS_READ_BIT_EXT; }
        if (accessFlags & ACCESS_FLAGS_COMMAND_PREPROCESS_WRITE_BIT_EXT)           { vkAccessFlags |= VK_ACCESS_COMMAND_PREPROCESS_WRITE_BIT_EXT; }

        return vkAccessFlags;
    }


    VkPipelineStageFlags ConvertPipelineStageToVkPipelineStageFlags(PipelineStageFlags pipelineStageFlags)
    {
        VkAccessFlags vkPipelineStageFlags = 0;

        if (pipelineStageFlags & PIPELINE_STAGE_TOP_OF_PIPE_BIT)                         { vkPipelineStageFlags |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_DRAW_INDIRECT_BIT)                       { vkPipelineStageFlags |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_VERTEX_INPUT_BIT)                        { vkPipelineStageFlags |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_VERTEX_SHADER_BIT)                       { vkPipelineStageFlags |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT)         { vkPipelineStageFlags |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT)      { vkPipelineStageFlags |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_GEOMETRY_SHADER_BIT)                     { vkPipelineStageFlags |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_FRAGMENT_SHADER_BIT)                     { vkPipelineStageFlags |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT)                { vkPipelineStageFlags |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT)                 { vkPipelineStageFlags |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)             { vkPipelineStageFlags |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_COMPUTE_SHADER_BIT)                      { vkPipelineStageFlags |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_TRANSFER_BIT)                            { vkPipelineStageFlags |= VK_PIPELINE_STAGE_TRANSFER_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT)                      { vkPipelineStageFlags |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_HOST_BIT)                                { vkPipelineStageFlags |= VK_PIPELINE_STAGE_HOST_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_ALL_GRAPHICS_BIT)                        { vkPipelineStageFlags |= VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_ALL_COMMANDS_BIT)                        { vkPipelineStageFlags |= VK_PIPELINE_STAGE_ALL_COMMANDS_BIT; }
        if (pipelineStageFlags & PIPELINE_STAGE_NONE)                                    { vkPipelineStageFlags |= VK_PIPELINE_STAGE_NONE; }
        if (pipelineStageFlags & PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT)        { vkPipelineStageFlags |= VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR; }
        if (pipelineStageFlags & PIPELINE_STAGE_RAY_TRACING_SHADER_BIT)                  { vkPipelineStageFlags |= VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR; }
        if (pipelineStageFlags & PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT)    { vkPipelineStageFlags |= VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR; }

        return vkPipelineStageFlags;
    }


    VkImageUsageFlags ConvertImageUsageFlagsToVkImageUsageFlags(ImageUsageFlags imageUsageFlags)
    {
        VkImageUsageFlags vkImageUsageFlags = 0;

        if (imageUsageFlags & IMAGE_USAGE_TRANSFER_SRC_BIT)                           { vkImageUsageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT; }
        if (imageUsageFlags & IMAGE_USAGE_TRANSFER_DST_BIT)                           { vkImageUsageFlags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT; }
        if (imageUsageFlags & IMAGE_USAGE_SAMPLED_BIT)                                { vkImageUsageFlags |= VK_IMAGE_USAGE_SAMPLED_BIT; }
        if (imageUsageFlags & IMAGE_USAGE_STORAGE_BIT)                                { vkImageUsageFlags |= VK_IMAGE_USAGE_STORAGE_BIT; }
        if (imageUsageFlags & IMAGE_USAGE_COLOR_ATTACHMENT_BIT)                       { vkImageUsageFlags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; }
        if (imageUsageFlags & IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)               { vkImageUsageFlags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT; }
        if (imageUsageFlags & IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)                   { vkImageUsageFlags |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT; }
        if (imageUsageFlags & IMAGE_USAGE_INPUT_ATTACHMENT_BIT)                       { vkImageUsageFlags |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT; }
        if (imageUsageFlags & IMAGE_USAGE_VIDEO_DECODE_DST_BIT)                       { vkImageUsageFlags |= VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR; }
        if (imageUsageFlags & IMAGE_USAGE_VIDEO_DECODE_SRC_BIT)                       { vkImageUsageFlags |= VK_IMAGE_USAGE_VIDEO_DECODE_SRC_BIT_KHR; }
        if (imageUsageFlags & IMAGE_USAGE_VIDEO_DECODE_DPB_BIT)                       { vkImageUsageFlags |= VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR; }
        if (imageUsageFlags & IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT)                   { vkImageUsageFlags |= VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT; }
        if (imageUsageFlags & IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT)       { vkImageUsageFlags |= VK_IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR; }
        if (imageUsageFlags & IMAGE_USAGE_VIDEO_ENCODE_DST_BIT)                       { vkImageUsageFlags |= VK_IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT_KHR; }
        if (imageUsageFlags & IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT)                       { vkImageUsageFlags |= VK_IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT_KHR; }
        if (imageUsageFlags & IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT)           { vkImageUsageFlags |= VK_IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT; }

        return vkImageUsageFlags;
    }


    VkImageAspectFlags ConvertImageAspectFlagsToVkImageAspectFlags(ImageAspectFlags imageAspectFlags)
    {
        VkImageAspectFlags vkImageAspectFlags = 0;

        if (imageAspectFlags & IMAGE_ASPECT_COLOR_BIT)             { vkImageAspectFlags |= VK_IMAGE_ASPECT_COLOR_BIT; }
        if (imageAspectFlags & IMAGE_ASPECT_DEPTH_BIT)             { vkImageAspectFlags |= VK_IMAGE_ASPECT_DEPTH_BIT; }
        if (imageAspectFlags & IMAGE_ASPECT_STENCIL_BIT)           { vkImageAspectFlags |= VK_IMAGE_ASPECT_STENCIL_BIT; }
        if (imageAspectFlags & IMAGE_ASPECT_METADATA_BIT)          { vkImageAspectFlags |= VK_IMAGE_ASPECT_METADATA_BIT; }
        if (imageAspectFlags & IMAGE_ASPECT_PLANE_0_BIT)           { vkImageAspectFlags |= VK_IMAGE_ASPECT_PLANE_0_BIT; }
        if (imageAspectFlags & IMAGE_ASPECT_PLANE_1_BIT)           { vkImageAspectFlags |= VK_IMAGE_ASPECT_PLANE_1_BIT; }
        if (imageAspectFlags & IMAGE_ASPECT_PLANE_2_BIT)           { vkImageAspectFlags |= VK_IMAGE_ASPECT_PLANE_2_BIT; }

        return vkImageAspectFlags;
    }


    VkDependencyFlags ConvertDependencyFlagsToVkDependencyFlags(DependencyFlags dependencyFlags)
    {
        VkDependencyFlags vkDependencyFlags = 0;

        if (dependencyFlags & DependencyFlags::BY_REGION_BIT)           { vkDependencyFlags |= VK_DEPENDENCY_BY_REGION_BIT; }
        if (dependencyFlags & DependencyFlags::DEVICE_GROUP_BIT)        { vkDependencyFlags |= VK_DEPENDENCY_DEVICE_GROUP_BIT; }
        if (dependencyFlags & DependencyFlags::VIEW_LOCAL_BIT)          { vkDependencyFlags |= VK_DEPENDENCY_VIEW_LOCAL_BIT; }
        if (dependencyFlags & DependencyFlags::FEEDBACK_LOOP_BIT)       { vkDependencyFlags |= VK_DEPENDENCY_FEEDBACK_LOOP_BIT_EXT; }


        return vkDependencyFlags;
    }


    ImageLayout ConvertVkImageLayoutToImageLayout(VkImageLayout imageLayout)
    {
        switch (imageLayout)
        {
            case VK_IMAGE_LAYOUT_UNDEFINED:                                      return ImageLayout::UNDEFINED;
            case VK_IMAGE_LAYOUT_GENERAL:                                        return ImageLayout::GENERAL;
            case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:                       return ImageLayout::COLOR_ATTACHMENT_OPTIMAL;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:               return ImageLayout::DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:                return ImageLayout::DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:                       return ImageLayout::SHADER_READ_ONLY_OPTIMAL;
            case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:                           return ImageLayout::TRANSFER_SRC_OPTIMAL;
            case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:                           return ImageLayout::TRANSFER_DST_OPTIMAL;
            case VK_IMAGE_LAYOUT_PREINITIALIZED:                                 return ImageLayout::PREINITIALIZED;
            case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:     return ImageLayout::DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
            case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:     return ImageLayout::DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
            case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:                       return ImageLayout::DEPTH_ATTACHMENT_OPTIMAL;
            case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:                        return ImageLayout::DEPTH_READ_ONLY_OPTIMAL;
            case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:                     return ImageLayout::STENCIL_ATTACHMENT_OPTIMAL;
            case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:                      return ImageLayout::STENCIL_READ_ONLY_OPTIMAL;
            case VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL:                              return ImageLayout::READ_ONLY_OPTIMAL;
            case VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL:                             return ImageLayout::ATTACHMENT_OPTIMAL;
            case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:                                return ImageLayout::PRESENT_SRC_KHR;
            case VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR:                           return ImageLayout::VIDEO_DECODE_DST_KHR;
            case VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR:                           return ImageLayout::VIDEO_DECODE_SRC_KHR;
            case VK_IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR:                           return ImageLayout::VIDEO_DECODE_DPB_KHR;
            case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:                             return ImageLayout::SHARED_PRESENT_KHR;
            case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:               return ImageLayout::FRAGMENT_DENSITY_MAP_OPTIMAL_EXT;
            case VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR:   return ImageLayout::FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR;
            case VK_IMAGE_LAYOUT_RENDERING_LOCAL_READ_KHR:                       return ImageLayout::RENDERING_LOCAL_READ_KHR;
            case VK_IMAGE_LAYOUT_VIDEO_ENCODE_DST_KHR:                           return ImageLayout::VIDEO_ENCODE_DST_KHR;
            case VK_IMAGE_LAYOUT_VIDEO_ENCODE_SRC_KHR:                           return ImageLayout::VIDEO_ENCODE_SRC_KHR;
            case VK_IMAGE_LAYOUT_VIDEO_ENCODE_DPB_KHR:                           return ImageLayout::VIDEO_ENCODE_DPB_KHR;
            case VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT:           return ImageLayout::ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT;

            default: ASTRAL_ERROR("Invalid VkImage Layout Given!");
        }
    }


    ImageFormat ConvertVkFormatToImageFormat(VkFormat imageFormat)
    {
        switch (imageFormat)
        {
            case VK_FORMAT_UNDEFINED:                                   return ImageFormat::UNDEFINED;
            case VK_FORMAT_R4G4_UNORM_PACK8:                            return ImageFormat::R4G4_UNORM_PACK8;
            case VK_FORMAT_R4G4B4A4_UNORM_PACK16:                       return ImageFormat::R4G4B4A4_UNORM_PACK16;
            case VK_FORMAT_B4G4R4A4_UNORM_PACK16:                       return ImageFormat::B4G4R4A4_UNORM_PACK16;
            case VK_FORMAT_R5G6B5_UNORM_PACK16:                         return ImageFormat::R5G6B5_UNORM_PACK16;
            case VK_FORMAT_B5G6R5_UNORM_PACK16:                         return ImageFormat::B5G6R5_UNORM_PACK16;
            case VK_FORMAT_R5G5B5A1_UNORM_PACK16:                       return ImageFormat::R5G5B5A1_UNORM_PACK16;
            case VK_FORMAT_B5G5R5A1_UNORM_PACK16:                       return ImageFormat::B5G5R5A1_UNORM_PACK16;
            case VK_FORMAT_A1R5G5B5_UNORM_PACK16:                       return ImageFormat::A1R5G5B5_UNORM_PACK16;
            case VK_FORMAT_R8_UNORM:                                    return ImageFormat::R8_UNORM;
            case VK_FORMAT_R8_SNORM:                                    return ImageFormat::R8_SNORM;
            case VK_FORMAT_R8_USCALED:                                  return ImageFormat::R8_USCALED;
            case VK_FORMAT_R8_SSCALED:                                  return ImageFormat::R8_SSCALED;
            case VK_FORMAT_R8_UINT:                                     return ImageFormat::R8_UINT;
            case VK_FORMAT_R8_SINT:                                     return ImageFormat::R8_SINT;
            case VK_FORMAT_R8_SRGB:                                     return ImageFormat::R8_SRGB;
            case VK_FORMAT_R8G8_UNORM:                                  return ImageFormat::R8G8_UNORM;
            case VK_FORMAT_R8G8_SNORM:                                  return ImageFormat::R8G8_SNORM;
            case VK_FORMAT_R8G8_USCALED:                                return ImageFormat::R8G8_USCALED;
            case VK_FORMAT_R8G8_SSCALED:                                return ImageFormat::R8G8_SSCALED;
            case VK_FORMAT_R8G8_UINT:                                   return ImageFormat::R8G8_UINT;
            case VK_FORMAT_R8G8_SINT:                                   return ImageFormat::R8G8_SINT;
            case VK_FORMAT_R8G8_SRGB:                                   return ImageFormat::R8G8_SRGB;
            case VK_FORMAT_R8G8B8_UNORM:                                return ImageFormat::R8G8B8_UNORM;
            case VK_FORMAT_R8G8B8_SNORM:                                return ImageFormat::R8G8B8_SNORM;
            case VK_FORMAT_R8G8B8_USCALED:                              return ImageFormat::R8G8B8_USCALED;
            case VK_FORMAT_R8G8B8_SSCALED:                              return ImageFormat::R8G8B8_SSCALED;
            case VK_FORMAT_R8G8B8_UINT:                                 return ImageFormat::R8G8B8_UINT;
            case VK_FORMAT_R8G8B8_SINT:                                 return ImageFormat::R8G8B8_SINT;
            case VK_FORMAT_R8G8B8_SRGB:                                 return ImageFormat::R8G8B8_SRGB;
            case VK_FORMAT_B8G8R8_UNORM:                                return ImageFormat::B8G8R8_UNORM;
            case VK_FORMAT_B8G8R8_SNORM:                                return ImageFormat::B8G8R8_SNORM;
            case VK_FORMAT_B8G8R8_USCALED:                              return ImageFormat::B8G8R8_USCALED;
            case VK_FORMAT_B8G8R8_SSCALED:                              return ImageFormat::B8G8R8_SSCALED;
            case VK_FORMAT_B8G8R8_UINT:                                 return ImageFormat::B8G8R8_UINT;
            case VK_FORMAT_B8G8R8_SINT:                                 return ImageFormat::B8G8R8_SINT;
            case VK_FORMAT_B8G8R8_SRGB:                                 return ImageFormat::B8G8R8_SRGB;
            case VK_FORMAT_R8G8B8A8_UNORM:                              return ImageFormat::R8G8B8A8_UNORM;
            case VK_FORMAT_R8G8B8A8_SNORM:                              return ImageFormat::R8G8B8A8_SNORM;
            case VK_FORMAT_R8G8B8A8_USCALED:                            return ImageFormat::R8G8B8A8_USCALED;
            case VK_FORMAT_R8G8B8A8_SSCALED:                            return ImageFormat::R8G8B8A8_SSCALED;
            case VK_FORMAT_R8G8B8A8_UINT:                               return ImageFormat::R8G8B8A8_UINT;
            case VK_FORMAT_R8G8B8A8_SINT:                               return ImageFormat::R8G8B8A8_SINT;
            case VK_FORMAT_R8G8B8A8_SRGB:                               return ImageFormat::R8G8B8A8_SRGB;
            case VK_FORMAT_B8G8R8A8_UNORM:                              return ImageFormat::B8G8R8A8_UNORM;
            case VK_FORMAT_B8G8R8A8_SNORM:                              return ImageFormat::B8G8R8A8_SNORM;
            case VK_FORMAT_B8G8R8A8_USCALED:                            return ImageFormat::B8G8R8A8_USCALED;
            case VK_FORMAT_B8G8R8A8_SSCALED:                            return ImageFormat::B8G8R8A8_SSCALED;
            case VK_FORMAT_B8G8R8A8_UINT:                               return ImageFormat::B8G8R8A8_UINT;
            case VK_FORMAT_B8G8R8A8_SINT:                               return ImageFormat::B8G8R8A8_SINT;
            case VK_FORMAT_B8G8R8A8_SRGB:                               return ImageFormat::B8G8R8A8_SRGB;
            case VK_FORMAT_A8B8G8R8_UNORM_PACK32:                       return ImageFormat::A8B8G8R8_UNORM_PACK32;
            case VK_FORMAT_A8B8G8R8_SNORM_PACK32:                       return ImageFormat::A8B8G8R8_SNORM_PACK32;
            case VK_FORMAT_A8B8G8R8_USCALED_PACK32:                     return ImageFormat::A8B8G8R8_USCALED_PACK32;
            case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:                     return ImageFormat::A8B8G8R8_SSCALED_PACK32;
            case VK_FORMAT_A8B8G8R8_UINT_PACK32:                        return ImageFormat::A8B8G8R8_UINT_PACK32;
            case VK_FORMAT_A8B8G8R8_SINT_PACK32:                        return ImageFormat::A8B8G8R8_SINT_PACK32;
            case VK_FORMAT_A8B8G8R8_SRGB_PACK32:                        return ImageFormat::A8B8G8R8_SRGB_PACK32;
            case VK_FORMAT_A2R10G10B10_UNORM_PACK32:                    return ImageFormat::A2R10G10B10_UNORM_PACK32;
            case VK_FORMAT_A2R10G10B10_SNORM_PACK32:                    return ImageFormat::A2R10G10B10_SNORM_PACK32;
            case VK_FORMAT_A2R10G10B10_USCALED_PACK32:                  return ImageFormat::A2R10G10B10_USCALED_PACK32;
            case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:                  return ImageFormat::A2R10G10B10_SSCALED_PACK32;
            case VK_FORMAT_A2R10G10B10_UINT_PACK32:                     return ImageFormat::A2R10G10B10_UINT_PACK32;
            case VK_FORMAT_A2R10G10B10_SINT_PACK32:                     return ImageFormat::A2R10G10B10_SINT_PACK32;
            case VK_FORMAT_A2B10G10R10_UNORM_PACK32:                    return ImageFormat::A2B10G10R10_UNORM_PACK32;
            case VK_FORMAT_A2B10G10R10_SNORM_PACK32:                    return ImageFormat::A2B10G10R10_SNORM_PACK32;
            case VK_FORMAT_A2B10G10R10_USCALED_PACK32:                  return ImageFormat::A2B10G10R10_USCALED_PACK32;
            case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:                  return ImageFormat::A2B10G10R10_SSCALED_PACK32;
            case VK_FORMAT_A2B10G10R10_UINT_PACK32:                     return ImageFormat::A2B10G10R10_UINT_PACK32;
            case VK_FORMAT_A2B10G10R10_SINT_PACK32:                     return ImageFormat::A2B10G10R10_SINT_PACK32;
            case VK_FORMAT_R16_UNORM:                                   return ImageFormat::R16_UNORM;
            case VK_FORMAT_R16_SNORM:                                   return ImageFormat::R16_SNORM;
            case VK_FORMAT_R16_USCALED:                                 return ImageFormat::R16_USCALED;
            case VK_FORMAT_R16_SSCALED:                                 return ImageFormat::R16_SSCALED;
            case VK_FORMAT_R16_UINT:                                    return ImageFormat::R16_UINT;
            case VK_FORMAT_R16_SINT:                                    return ImageFormat::R16_SINT;
            case VK_FORMAT_R16_SFLOAT:                                  return ImageFormat::R16_SFLOAT;
            case VK_FORMAT_R16G16_UNORM:                                return ImageFormat::R16G16_UNORM;
            case VK_FORMAT_R16G16_SNORM:                                return ImageFormat::R16G16_SNORM;
            case VK_FORMAT_R16G16_USCALED:                              return ImageFormat::R16G16_USCALED;
            case VK_FORMAT_R16G16_SSCALED:                              return ImageFormat::R16G16_SSCALED;
            case VK_FORMAT_R16G16_UINT:                                 return ImageFormat::R16G16_UINT;
            case VK_FORMAT_R16G16_SINT:                                 return ImageFormat::R16G16_SINT;
            case VK_FORMAT_R16G16_SFLOAT:                               return ImageFormat::R16G16_SFLOAT;
            case VK_FORMAT_R16G16B16_UNORM:                             return ImageFormat::R16G16B16_UNORM;
            case VK_FORMAT_R16G16B16_SNORM:                             return ImageFormat::R16G16B16_SNORM;
            case VK_FORMAT_R16G16B16_USCALED:                           return ImageFormat::R16G16B16_USCALED;
            case VK_FORMAT_R16G16B16_SSCALED:                           return ImageFormat::R16G16B16_SSCALED;
            case VK_FORMAT_R16G16B16_UINT:                              return ImageFormat::R16G16B16_UINT;
            case VK_FORMAT_R16G16B16_SINT:                              return ImageFormat::R16G16B16_SINT;
            case VK_FORMAT_R16G16B16_SFLOAT:                            return ImageFormat::R16G16B16_SFLOAT;
            case VK_FORMAT_R16G16B16A16_UNORM:                          return ImageFormat::R16G16B16A16_UNORM;
            case VK_FORMAT_R16G16B16A16_SNORM:                          return ImageFormat::R16G16B16A16_SNORM;
            case VK_FORMAT_R16G16B16A16_USCALED:                        return ImageFormat::R16G16B16A16_USCALED;
            case VK_FORMAT_R16G16B16A16_SSCALED:                        return ImageFormat::R16G16B16A16_SSCALED;
            case VK_FORMAT_R16G16B16A16_UINT:                           return ImageFormat::R16G16B16A16_UINT;
            case VK_FORMAT_R16G16B16A16_SINT:                           return ImageFormat::R16G16B16A16_SINT;
            case VK_FORMAT_R16G16B16A16_SFLOAT:                         return ImageFormat::R16G16B16A16_SFLOAT;
            case VK_FORMAT_R32_UINT:                                    return ImageFormat::R32_UINT;
            case VK_FORMAT_R32_SINT:                                    return ImageFormat::R32_SINT;
            case VK_FORMAT_R32_SFLOAT:                                  return ImageFormat::R32_SFLOAT;
            case VK_FORMAT_R32G32_UINT:                                 return ImageFormat::R32G32_UINT;
            case VK_FORMAT_R32G32_SINT:                                 return ImageFormat::R32G32_SINT;
            case VK_FORMAT_R32G32_SFLOAT:                               return ImageFormat::R32G32_SFLOAT;
            case VK_FORMAT_R32G32B32_UINT:                              return ImageFormat::R32G32B32_UINT;
            case VK_FORMAT_R32G32B32_SINT:                              return ImageFormat::R32G32B32_SINT;
            case VK_FORMAT_R32G32B32_SFLOAT:                            return ImageFormat::R32G32B32_SFLOAT;
            case VK_FORMAT_R32G32B32A32_UINT:                           return ImageFormat::R32G32B32A32_UINT;
            case VK_FORMAT_R32G32B32A32_SINT:                           return ImageFormat::R32G32B32A32_SINT;
            case VK_FORMAT_R32G32B32A32_SFLOAT:                         return ImageFormat::R32G32B32A32_SFLOAT;
            case VK_FORMAT_R64_UINT:                                    return ImageFormat::R64_UINT;
            case VK_FORMAT_R64_SINT:                                    return ImageFormat::R64_SINT;
            case VK_FORMAT_R64_SFLOAT:                                  return ImageFormat::R64_SFLOAT;
            case VK_FORMAT_R64G64_UINT:                                 return ImageFormat::R64G64_UINT;
            case VK_FORMAT_R64G64_SINT:                                 return ImageFormat::R64G64_SINT;
            case VK_FORMAT_R64G64_SFLOAT:                               return ImageFormat::R64G64_SFLOAT;
            case VK_FORMAT_R64G64B64_UINT:                              return ImageFormat::R64G64B64_UINT;
            case VK_FORMAT_R64G64B64_SINT:                              return ImageFormat::R64G64B64_SINT;
            case VK_FORMAT_R64G64B64_SFLOAT:                            return ImageFormat::R64G64B64_SFLOAT;
            case VK_FORMAT_R64G64B64A64_UINT:                           return ImageFormat::R64G64B64A64_UINT;
            case VK_FORMAT_R64G64B64A64_SINT:                           return ImageFormat::R64G64B64A64_SINT;
            case VK_FORMAT_R64G64B64A64_SFLOAT:                         return ImageFormat::R64G64B64A64_SFLOAT;
            case VK_FORMAT_B10G11R11_UFLOAT_PACK32:                     return ImageFormat::B10G11R11_UFLOAT_PACK32;
            case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:                      return ImageFormat::E5B9G9R9_UFLOAT_PACK32;
            case VK_FORMAT_D16_UNORM:                                   return ImageFormat::D16_UNORM;
            case VK_FORMAT_X8_D24_UNORM_PACK32:                         return ImageFormat::X8_D24_UNORM_PACK32;
            case VK_FORMAT_D32_SFLOAT:                                  return ImageFormat::D32_SFLOAT;
            case VK_FORMAT_S8_UINT:                                     return ImageFormat::S8_UINT;
            case VK_FORMAT_D16_UNORM_S8_UINT:                           return ImageFormat::D16_UNORM_S8_UINT;
            case VK_FORMAT_D24_UNORM_S8_UINT:                           return ImageFormat::D24_UNORM_S8_UINT;
            case VK_FORMAT_D32_SFLOAT_S8_UINT:                          return ImageFormat::D32_SFLOAT_S8_UINT;
            case VK_FORMAT_BC1_RGB_UNORM_BLOCK:                         return ImageFormat::BC1_RGB_UNORM_BLOCK;
            case VK_FORMAT_BC1_RGB_SRGB_BLOCK:                          return ImageFormat::BC1_RGB_SRGB_BLOCK;
            case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:                        return ImageFormat::BC1_RGBA_UNORM_BLOCK;
            case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:                         return ImageFormat::BC1_RGBA_SRGB_BLOCK;
            case VK_FORMAT_BC2_UNORM_BLOCK:                             return ImageFormat::BC2_UNORM_BLOCK;
            case VK_FORMAT_BC2_SRGB_BLOCK:                              return ImageFormat::BC2_SRGB_BLOCK;
            case VK_FORMAT_BC3_UNORM_BLOCK:                             return ImageFormat::BC3_UNORM_BLOCK;
            case VK_FORMAT_BC3_SRGB_BLOCK:                              return ImageFormat::BC3_SRGB_BLOCK;
            case VK_FORMAT_BC4_UNORM_BLOCK:                             return ImageFormat::BC4_UNORM_BLOCK;
            case VK_FORMAT_BC4_SNORM_BLOCK:                             return ImageFormat::BC4_SNORM_BLOCK;
            case VK_FORMAT_BC5_UNORM_BLOCK:                             return ImageFormat::BC5_UNORM_BLOCK;
            case VK_FORMAT_BC5_SNORM_BLOCK:                             return ImageFormat::BC5_SNORM_BLOCK;
            case VK_FORMAT_BC6H_UFLOAT_BLOCK:                           return ImageFormat::BC6H_UFLOAT_BLOCK;
            case VK_FORMAT_BC6H_SFLOAT_BLOCK:                           return ImageFormat::BC6H_SFLOAT_BLOCK;
            case VK_FORMAT_BC7_UNORM_BLOCK:                             return ImageFormat::BC7_UNORM_BLOCK;
            case VK_FORMAT_BC7_SRGB_BLOCK:                              return ImageFormat::BC7_SRGB_BLOCK;
            case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:                     return ImageFormat::ETC2_R8G8B8_UNORM_BLOCK;
            case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:                      return ImageFormat::ETC2_R8G8B8_SRGB_BLOCK;
            case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:                   return ImageFormat::ETC2_R8G8B8A1_UNORM_BLOCK;
            case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:                    return ImageFormat::ETC2_R8G8B8A1_SRGB_BLOCK;
            case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:                   return ImageFormat::ETC2_R8G8B8A8_UNORM_BLOCK;
            case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:                    return ImageFormat::ETC2_R8G8B8A8_SRGB_BLOCK;
            case VK_FORMAT_EAC_R11_UNORM_BLOCK:                         return ImageFormat::EAC_R11_UNORM_BLOCK;
            case VK_FORMAT_EAC_R11_SNORM_BLOCK:                         return ImageFormat::EAC_R11_SNORM_BLOCK;
            case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:                      return ImageFormat::EAC_R11G11_UNORM_BLOCK;
            case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:                      return ImageFormat::EAC_R11G11_SNORM_BLOCK;
            case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:                        return ImageFormat::ASTC_4x4_UNORM_BLOCK;
            case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:                         return ImageFormat::ASTC_4x4_SRGB_BLOCK;
            case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:                        return ImageFormat::ASTC_5x4_UNORM_BLOCK;
            case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:                         return ImageFormat::ASTC_5x4_SRGB_BLOCK;
            case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:                        return ImageFormat::ASTC_5x5_UNORM_BLOCK;
            case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:                         return ImageFormat::ASTC_5x5_SRGB_BLOCK;
            case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:                        return ImageFormat::ASTC_6x5_UNORM_BLOCK;
            case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:                         return ImageFormat::ASTC_6x5_SRGB_BLOCK;
            case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:                        return ImageFormat::ASTC_6x6_UNORM_BLOCK;
            case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:                         return ImageFormat::ASTC_6x6_SRGB_BLOCK;
            case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:                        return ImageFormat::ASTC_8x5_UNORM_BLOCK;
            case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:                         return ImageFormat::ASTC_8x5_SRGB_BLOCK;
            case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:                        return ImageFormat::ASTC_8x6_UNORM_BLOCK;
            case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:                         return ImageFormat::ASTC_8x6_SRGB_BLOCK;
            case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:                        return ImageFormat::ASTC_8x8_UNORM_BLOCK;
            case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:                         return ImageFormat::ASTC_8x8_SRGB_BLOCK;
            case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:                       return ImageFormat::ASTC_10x5_UNORM_BLOCK;
            case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:                        return ImageFormat::ASTC_10x5_SRGB_BLOCK;
            case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:                       return ImageFormat::ASTC_10x6_UNORM_BLOCK;
            case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:                        return ImageFormat::ASTC_10x6_SRGB_BLOCK;
            case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:                       return ImageFormat::ASTC_10x8_UNORM_BLOCK;
            case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:                        return ImageFormat::ASTC_10x8_SRGB_BLOCK;
            case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:                      return ImageFormat::ASTC_10x10_UNORM_BLOCK;
            case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:                       return ImageFormat::ASTC_10x10_SRGB_BLOCK;
            case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:                      return ImageFormat::ASTC_12x10_UNORM_BLOCK;
            case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:                       return ImageFormat::ASTC_12x10_SRGB_BLOCK;
            case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:                      return ImageFormat::ASTC_12x12_UNORM_BLOCK;
            case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:                       return ImageFormat::ASTC_12x12_SRGB_BLOCK;
            case VK_FORMAT_G8B8G8R8_422_UNORM:                          return ImageFormat::G8B8G8R8_422_UNORM;
            case VK_FORMAT_B8G8R8G8_422_UNORM:                          return ImageFormat::B8G8R8G8_422_UNORM;
            case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM:                   return ImageFormat::G8_B8_R8_3PLANE_420_UNORM;
            case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM:                    return ImageFormat::G8_B8R8_2PLANE_420_UNORM;
            case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM:                   return ImageFormat::G8_B8_R8_3PLANE_422_UNORM;
            case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM:                    return ImageFormat::G8_B8R8_2PLANE_422_UNORM;
            case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM:                   return ImageFormat::G8_B8_R8_3PLANE_444_UNORM;
            case VK_FORMAT_R10X6_UNORM_PACK16:                          return ImageFormat::R10X6_UNORM_PACK16;
            case VK_FORMAT_R10X6G10X6_UNORM_2PACK16:                    return ImageFormat::R10X6G10X6_UNORM_2PACK16;
            case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16:          return ImageFormat::R10X6G10X6B10X6A10X6_UNORM_4PACK16;
            case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:      return ImageFormat::G10X6B10X6G10X6R10X6_422_UNORM_4PACK16;
            case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:      return ImageFormat::B10X6G10X6R10X6G10X6_422_UNORM_4PACK16;
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:  return ImageFormat::G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16;
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:   return ImageFormat::G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16;
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:  return ImageFormat::G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16;
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:   return ImageFormat::G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16;
            case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:  return ImageFormat::G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16;
            case VK_FORMAT_R12X4_UNORM_PACK16:                          return ImageFormat::R12X4_UNORM_PACK16;
            case VK_FORMAT_R12X4G12X4_UNORM_2PACK16:                    return ImageFormat::R12X4G12X4_UNORM_2PACK16;
            case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16:          return ImageFormat::R12X4G12X4B12X4A12X4_UNORM_4PACK16;
            case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:      return ImageFormat::G12X4B12X4G12X4R12X4_422_UNORM_4PACK16;
            case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:      return ImageFormat::B12X4G12X4R12X4G12X4_422_UNORM_4PACK16;
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:  return ImageFormat::G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16;
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:   return ImageFormat::G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16;
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:  return ImageFormat::G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16;
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:   return ImageFormat::G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16;
            case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:  return ImageFormat::G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16;
            case VK_FORMAT_G16B16G16R16_422_UNORM:                      return ImageFormat::G16B16G16R16_422_UNORM;
            case VK_FORMAT_B16G16R16G16_422_UNORM:                      return ImageFormat::B16G16R16G16_422_UNORM;
            case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM:                return ImageFormat::G16_B16_R16_3PLANE_420_UNORM;
            case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM:                 return ImageFormat::G16_B16R16_2PLANE_420_UNORM;
            case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM:                return ImageFormat::G16_B16_R16_3PLANE_422_UNORM;
            case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM:                 return ImageFormat::G16_B16R16_2PLANE_422_UNORM;
            case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM:                return ImageFormat::G16_B16_R16_3PLANE_444_UNORM;
            case VK_FORMAT_G8_B8R8_2PLANE_444_UNORM:                    return ImageFormat::G8_B8R8_2PLANE_444_UNORM;
            case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16:   return ImageFormat::G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16;
            case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16:   return ImageFormat::G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16;
            case VK_FORMAT_G16_B16R16_2PLANE_444_UNORM:                 return ImageFormat::G16_B16R16_2PLANE_444_UNORM;
            case VK_FORMAT_A4R4G4B4_UNORM_PACK16:                       return ImageFormat::A4R4G4B4_UNORM_PACK16;
            case VK_FORMAT_A4B4G4R4_UNORM_PACK16:                       return ImageFormat::A4B4G4R4_UNORM_PACK16;
            case VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK:                       return ImageFormat::ASTC_4x4_SFLOAT_BLOCK;
            case VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK:                       return ImageFormat::ASTC_5x4_SFLOAT_BLOCK;
            case VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK:                       return ImageFormat::ASTC_5x5_SFLOAT_BLOCK;
            case VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK:                       return ImageFormat::ASTC_6x5_SFLOAT_BLOCK;
            case VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK:                       return ImageFormat::ASTC_6x6_SFLOAT_BLOCK;
            case VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK:                       return ImageFormat::ASTC_8x5_SFLOAT_BLOCK;
            case VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK:                       return ImageFormat::ASTC_8x6_SFLOAT_BLOCK;
            case VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK:                       return ImageFormat::ASTC_8x8_SFLOAT_BLOCK;
            case VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK:                      return ImageFormat::ASTC_10x5_SFLOAT_BLOCK;
            case VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK:                      return ImageFormat::ASTC_10x6_SFLOAT_BLOCK;
            case VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK:                      return ImageFormat::ASTC_10x8_SFLOAT_BLOCK;
            case VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK:                     return ImageFormat::ASTC_10x10_SFLOAT_BLOCK;
            case VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK:                     return ImageFormat::ASTC_12x10_SFLOAT_BLOCK;
            case VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK:                     return ImageFormat::ASTC_12x12_SFLOAT_BLOCK;
            case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:                 return ImageFormat::PVRTC1_2BPP_UNORM_BLOCK_IMG;
            case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:                 return ImageFormat::PVRTC1_4BPP_UNORM_BLOCK_IMG;
            case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:                 return ImageFormat::PVRTC2_2BPP_UNORM_BLOCK_IMG;
            case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:                 return ImageFormat::PVRTC2_4BPP_UNORM_BLOCK_IMG;
            case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:                  return ImageFormat::PVRTC1_2BPP_SRGB_BLOCK_IMG;
            case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:                  return ImageFormat::PVRTC1_4BPP_SRGB_BLOCK_IMG;
            case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:                  return ImageFormat::PVRTC2_2BPP_SRGB_BLOCK_IMG;
            case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:                  return ImageFormat::PVRTC2_4BPP_SRGB_BLOCK_IMG;
            case VK_FORMAT_R16G16_SFIXED5_NV:                           return ImageFormat::R16G16_SFIXED5_NV;
            case VK_FORMAT_A1B5G5R5_UNORM_PACK16_KHR:                   return ImageFormat::A1B5G5R5_UNORM_PACK16_KHR;
            case VK_FORMAT_A8_UNORM_KHR:                                return ImageFormat::A8_UNORM_KHR;

            case VK_FORMAT_MAX_ENUM: // Fallthrough
            default: ASTRAL_ERROR("Invalid VkFormat Given!");
        }
    }

}
