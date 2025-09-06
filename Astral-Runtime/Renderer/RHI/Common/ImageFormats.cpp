/**
* @file ImageFormats.cpp
* @author Andrew Fagan
* @date 7/23/25
*/

#include "ImageFormats.h"

namespace Astral {

    uint32 GetBytesPerTexel(ImageFormat imageFormat)
    {
        switch (imageFormat)
        {
            // Single channel 8-bit
            case ImageFormat::R8_UNORM: return 1;
            case ImageFormat::R8_SNORM: return 1;
            case ImageFormat::R8_UINT: return 1;
            case ImageFormat::R8_SINT: return 1;
            case ImageFormat::R8_SRGB: return 1;

            // Dual channel 8-bit
            case ImageFormat::R8G8_UNORM: return 2;
            case ImageFormat::R8G8_SNORM: return 2;
            case ImageFormat::R8G8_UINT: return 2;
            case ImageFormat::R8G8_SINT: return 2;
            case ImageFormat::R8G8_SRGB: return 2;

            // Triple channel 8-bit
            case ImageFormat::R8G8B8_UNORM: return 3;
            case ImageFormat::R8G8B8_SNORM: return 3;
            case ImageFormat::R8G8B8_UINT: return 3;
            case ImageFormat::R8G8B8_SINT: return 3;
            case ImageFormat::R8G8B8_SRGB: return 3;
            case ImageFormat::B8G8R8_UNORM: return 3;
            case ImageFormat::B8G8R8_SNORM: return 3;
            case ImageFormat::B8G8R8_UINT: return 3;
            case ImageFormat::B8G8R8_SINT: return 3;
            case ImageFormat::B8G8R8_SRGB: return 3;

            // Quad channel 8-bit
            case ImageFormat::R8G8B8A8_UNORM: return 4;
            case ImageFormat::R8G8B8A8_SNORM: return 4;
            case ImageFormat::R8G8B8A8_UINT: return 4;
            case ImageFormat::R8G8B8A8_SINT: return 4;
            case ImageFormat::R8G8B8A8_SRGB: return 4;
            case ImageFormat::B8G8R8A8_UNORM: return 4;
            case ImageFormat::B8G8R8A8_SNORM: return 4;
            case ImageFormat::B8G8R8A8_UINT: return 4;
            case ImageFormat::B8G8R8A8_SINT: return 4;
            case ImageFormat::B8G8R8A8_SRGB: return 4;

            // 16-bit single channel
            case ImageFormat::R16_UNORM: return 2;
            case ImageFormat::R16_SNORM: return 2;
            case ImageFormat::R16_UINT: return 2;
            case ImageFormat::R16_SINT: return 2;
            case ImageFormat::R16_SFLOAT: return 2;

            // 16-bit dual channel
            case ImageFormat::R16G16_UNORM: return 4;
            case ImageFormat::R16G16_SNORM: return 4;
            case ImageFormat::R16G16_UINT: return 4;
            case ImageFormat::R16G16_SINT: return 4;
            case ImageFormat::R16G16_SFLOAT: return 4;

            // 16-bit triple channel
            case ImageFormat::R16G16B16_UNORM: return 6;
            case ImageFormat::R16G16B16_SNORM: return 6;
            case ImageFormat::R16G16B16_UINT: return 6;
            case ImageFormat::R16G16B16_SINT: return 6;
            case ImageFormat::R16G16B16_SFLOAT: return 6;

            // 16-bit quad channel (HDR)
            case ImageFormat::R16G16B16A16_UNORM: return 8;
            case ImageFormat::R16G16B16A16_SNORM: return 8;
            case ImageFormat::R16G16B16A16_UINT: return 8;
            case ImageFormat::R16G16B16A16_SINT: return 8;
            case ImageFormat::R16G16B16A16_SFLOAT: return 8;

            // 32-bit single channel
            case ImageFormat::R32_UINT: return 4;
            case ImageFormat::R32_SINT: return 4;
            case ImageFormat::R32_SFLOAT: return 4;

            // 32-bit dual channel
            case ImageFormat::R32G32_UINT: return 8;
            case ImageFormat::R32G32_SINT: return 8;
            case ImageFormat::R32G32_SFLOAT: return 8;

            // 32-bit triple channel
            case ImageFormat::R32G32B32_UINT: return 12;
            case ImageFormat::R32G32B32_SINT: return 12;
            case ImageFormat::R32G32B32_SFLOAT: return 12;

            // 32-bit quad channel
            case ImageFormat::R32G32B32A32_UINT: return 16;
            case ImageFormat::R32G32B32A32_SINT: return 16;
            case ImageFormat::R32G32B32A32_SFLOAT: return 16;

            // Packed Formats
            case ImageFormat::R5G6B5_UNORM_PACK16: return 2;
            case ImageFormat::B5G6R5_UNORM_PACK16: return 2;
            case ImageFormat::R4G4B4A4_UNORM_PACK16: return 2;
            case ImageFormat::B4G4R4A4_UNORM_PACK16: return 2;
            case ImageFormat::R5G5B5A1_UNORM_PACK16: return 2;
            case ImageFormat::B5G5R5A1_UNORM_PACK16: return 2;
            case ImageFormat::A1R5G5B5_UNORM_PACK16: return 2;

            // Depth/Stencil Formats
            case ImageFormat::D16_UNORM: return 2;
            case ImageFormat::D32_SFLOAT: return 4;
            case ImageFormat::D24_UNORM_S8_UINT: return 4;
            case ImageFormat::D32_SFLOAT_S8_UINT: return 5;  // 4 for depth + 1 for stencil
            case ImageFormat::D16_UNORM_S8_UINT: return 3;   // 2 for depth + 1 for stencil

        	// Compressed Formats
        	case ImageFormat::BC1_RGB_UNORM_BLOCK:   return 8;   // BC1
        	case ImageFormat::BC1_RGB_SRGB_BLOCK:    return 8;   // BC1 (sRGB)
        	case ImageFormat::BC1_RGBA_UNORM_BLOCK:  return 8;   // BC1 (with alpha)
        	case ImageFormat::BC1_RGBA_SRGB_BLOCK:   return 8;   // BC1 (sRGB)
        	case ImageFormat::BC2_UNORM_BLOCK:       return 16;  // BC2
        	case ImageFormat::BC2_SRGB_BLOCK:        return 16;  // BC2 (sRGB)
        	case ImageFormat::BC3_UNORM_BLOCK:       return 16;  // BC3
        	case ImageFormat::BC3_SRGB_BLOCK:        return 16;  // BC3 (sRGB)
        	case ImageFormat::BC4_UNORM_BLOCK:       return 8;   // BC4
        	case ImageFormat::BC4_SNORM_BLOCK:       return 8;   // BC4 (signed)
        	case ImageFormat::BC5_UNORM_BLOCK:       return 16;  // BC5
        	case ImageFormat::BC5_SNORM_BLOCK:       return 16;  // BC5 (signed)
        	case ImageFormat::BC6H_UFLOAT_BLOCK:     return 16;  // BC6H (unsigned float)
        	case ImageFormat::BC6H_SFLOAT_BLOCK:     return 16;  // BC6H (signed float)
        	case ImageFormat::BC7_UNORM_BLOCK:       return 16;  // BC7
        	case ImageFormat::BC7_SRGB_BLOCK:        return 16;  // BC7 (sRGB)

            // Special Formats
            case ImageFormat::B10G11R11_UFLOAT_PACK32: return 4;  // HDR without alpha
            case ImageFormat::E5B9G9R9_UFLOAT_PACK32: return 4;   // Shared exponent HDR

            // Undefined
            case ImageFormat::UNDEFINED: ASTRAL_ERROR("Undefined format cannot be applied to texture!");
            case ImageFormat::MAX_ENUM: ASTRAL_ERROR("Unsupported format!")
            default: ASTRAL_ERROR("Unsupported format!")
        }
    }


    uint32 GetNumComponents(ImageFormat imageFormat)
    {
    	switch (imageFormat)
        {
            // Single channel 8-bit
            case ImageFormat::R8_UNORM: return 1;
            case ImageFormat::R8_SNORM: return 1;
            case ImageFormat::R8_UINT: return 1;
            case ImageFormat::R8_SINT: return 1;
            case ImageFormat::R8_SRGB: return 1;

            // Dual channel 8-bit
            case ImageFormat::R8G8_UNORM: return 2;
            case ImageFormat::R8G8_SNORM: return 2;
            case ImageFormat::R8G8_UINT: return 2;
            case ImageFormat::R8G8_SINT: return 2;
            case ImageFormat::R8G8_SRGB: return 2;

            // Triple channel 8-bit
            case ImageFormat::R8G8B8_UNORM: return 3;
            case ImageFormat::R8G8B8_SNORM: return 3;
            case ImageFormat::R8G8B8_UINT: return 3;
            case ImageFormat::R8G8B8_SINT: return 3;
            case ImageFormat::R8G8B8_SRGB: return 3;
            case ImageFormat::B8G8R8_UNORM: return 3;
            case ImageFormat::B8G8R8_SNORM: return 3;
            case ImageFormat::B8G8R8_UINT: return 3;
            case ImageFormat::B8G8R8_SINT: return 3;
            case ImageFormat::B8G8R8_SRGB: return 3;

            // Quad channel 8-bit
            case ImageFormat::R8G8B8A8_UNORM: return 4;
            case ImageFormat::R8G8B8A8_SNORM: return 4;
            case ImageFormat::R8G8B8A8_UINT: return 4;
            case ImageFormat::R8G8B8A8_SINT: return 4;
            case ImageFormat::R8G8B8A8_SRGB: return 4;
            case ImageFormat::B8G8R8A8_UNORM: return 4;
            case ImageFormat::B8G8R8A8_SNORM: return 4;
            case ImageFormat::B8G8R8A8_UINT: return 4;
            case ImageFormat::B8G8R8A8_SINT: return 4;
            case ImageFormat::B8G8R8A8_SRGB: return 4;

            // 16-bit single channel
            case ImageFormat::R16_UNORM: return 1;
            case ImageFormat::R16_SNORM: return 1;
            case ImageFormat::R16_UINT: return 1;
            case ImageFormat::R16_SINT: return 1;
            case ImageFormat::R16_SFLOAT: return 1;

            // 16-bit dual channel
            case ImageFormat::R16G16_UNORM: return 2;
            case ImageFormat::R16G16_SNORM: return 2;
            case ImageFormat::R16G16_UINT: return 2;
            case ImageFormat::R16G16_SINT: return 2;
            case ImageFormat::R16G16_SFLOAT: return 2;

            // 16-bit triple channel
            case ImageFormat::R16G16B16_UNORM: return 3;
            case ImageFormat::R16G16B16_SNORM: return 3;
            case ImageFormat::R16G16B16_UINT: return 3;
            case ImageFormat::R16G16B16_SINT: return 3;
            case ImageFormat::R16G16B16_SFLOAT: return 3;

            // 16-bit quad channel (HDR)
            case ImageFormat::R16G16B16A16_UNORM: return 4;
            case ImageFormat::R16G16B16A16_SNORM: return 4;
            case ImageFormat::R16G16B16A16_UINT: return 4;
            case ImageFormat::R16G16B16A16_SINT: return 4;
            case ImageFormat::R16G16B16A16_SFLOAT: return 4;

            // 32-bit single channel
            case ImageFormat::R32_UINT: return 1;
            case ImageFormat::R32_SINT: return 1;
            case ImageFormat::R32_SFLOAT: return 1;

            // 32-bit dual channel
            case ImageFormat::R32G32_UINT: return 2;
            case ImageFormat::R32G32_SINT: return 2;
            case ImageFormat::R32G32_SFLOAT: return 2;

            // 32-bit triple channel
            case ImageFormat::R32G32B32_UINT: return 3;
            case ImageFormat::R32G32B32_SINT: return 3;
            case ImageFormat::R32G32B32_SFLOAT: return 3;

            // 32-bit quad channel
            case ImageFormat::R32G32B32A32_UINT: return 4;
            case ImageFormat::R32G32B32A32_SINT: return 4;
            case ImageFormat::R32G32B32A32_SFLOAT: return 4;

            // Packed Formats
            case ImageFormat::R5G6B5_UNORM_PACK16: return 3;
            case ImageFormat::B5G6R5_UNORM_PACK16: return 3;
            case ImageFormat::R4G4B4A4_UNORM_PACK16: return 4;
            case ImageFormat::B4G4R4A4_UNORM_PACK16: return 4;
            case ImageFormat::R5G5B5A1_UNORM_PACK16: return 4;
            case ImageFormat::B5G5R5A1_UNORM_PACK16: return 4;
            case ImageFormat::A1R5G5B5_UNORM_PACK16: return 4;

            // Depth/Stencil Formats
            case ImageFormat::D16_UNORM: return 1;
            case ImageFormat::D32_SFLOAT: return 1;
            case ImageFormat::D24_UNORM_S8_UINT: return 2;
            case ImageFormat::D32_SFLOAT_S8_UINT: return 2;  // 4 for depth + 1 for stencil
            case ImageFormat::D16_UNORM_S8_UINT: return 2;   // 2 for depth + 1 for stencil

        	// Compressed Formats ()
        	case ImageFormat::BC1_RGB_UNORM_BLOCK:   return 0;   // BC1
        	case ImageFormat::BC1_RGB_SRGB_BLOCK:    return 0;   // BC1 (sRGB)
        	case ImageFormat::BC1_RGBA_UNORM_BLOCK:  return 0;   // BC1 (with alpha)
        	case ImageFormat::BC1_RGBA_SRGB_BLOCK:   return 0;   // BC1 (sRGB)
        	case ImageFormat::BC2_UNORM_BLOCK:       return 0;  // BC2
        	case ImageFormat::BC2_SRGB_BLOCK:        return 0;  // BC2 (sRGB)
        	case ImageFormat::BC3_UNORM_BLOCK:       return 0;  // BC3
        	case ImageFormat::BC3_SRGB_BLOCK:        return 0;  // BC3 (sRGB)
        	case ImageFormat::BC4_UNORM_BLOCK:       return 0;   // BC4
        	case ImageFormat::BC4_SNORM_BLOCK:       return 0;   // BC4 (signed)
        	case ImageFormat::BC5_UNORM_BLOCK:       return 0;  // BC5
        	case ImageFormat::BC5_SNORM_BLOCK:       return 0;  // BC5 (signed)
        	case ImageFormat::BC6H_UFLOAT_BLOCK:     return 0;  // BC6H (unsigned float)
        	case ImageFormat::BC6H_SFLOAT_BLOCK:     return 0;  // BC6H (signed float)
        	case ImageFormat::BC7_UNORM_BLOCK:       return 0;  // BC7
        	case ImageFormat::BC7_SRGB_BLOCK:        return 0;  // BC7 (sRGB)

            // Special Formats
            case ImageFormat::B10G11R11_UFLOAT_PACK32: return 3;  // HDR without alpha
            case ImageFormat::E5B9G9R9_UFLOAT_PACK32: return 3;   // Shared exponent HDR

            // Undefined
            case ImageFormat::UNDEFINED: ASTRAL_ERROR("Undefined format cannot be applied to texture!");
            case ImageFormat::MAX_ENUM: ASTRAL_ERROR("Unsupported format!")
            default: ASTRAL_ERROR("Unsupported format!")
        }
    }


    bool IsCompressed(ImageFormat format)
    {
    	switch (format)
    	{
    		case ImageFormat::BC1_RGB_UNORM_BLOCK:    // fallthrough
    		case ImageFormat::BC1_RGB_SRGB_BLOCK:     // fallthrough
    		case ImageFormat::BC1_RGBA_UNORM_BLOCK:   // fallthrough
    		case ImageFormat::BC1_RGBA_SRGB_BLOCK:    // fallthrough
    		case ImageFormat::BC2_UNORM_BLOCK:        // fallthrough
    		case ImageFormat::BC2_SRGB_BLOCK:         // fallthrough
    		case ImageFormat::BC3_UNORM_BLOCK:        // fallthrough
    		case ImageFormat::BC3_SRGB_BLOCK:         // fallthrough
    		case ImageFormat::BC4_UNORM_BLOCK:        // fallthrough
    		case ImageFormat::BC4_SNORM_BLOCK:        // fallthrough
    		case ImageFormat::BC5_UNORM_BLOCK:        // fallthrough
    		case ImageFormat::BC5_SNORM_BLOCK:        // fallthrough
    		case ImageFormat::BC6H_UFLOAT_BLOCK:      // fallthrough
    		case ImageFormat::BC6H_SFLOAT_BLOCK:      // fallthrough
    		case ImageFormat::BC7_UNORM_BLOCK:        // fallthrough
    		case ImageFormat::BC7_SRGB_BLOCK:         // fallthrough

	        case ImageFormat::EAC_R11_UNORM_BLOCK:    // fallthrough
	        case ImageFormat::EAC_R11_SNORM_BLOCK:    // fallthrough
	        case ImageFormat::EAC_R11G11_UNORM_BLOCK: // fallthrough
	        case ImageFormat::EAC_R11G11_SNORM_BLOCK: // fallthrough

	        case ImageFormat::ASTC_4x4_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_4x4_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_5x4_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_5x4_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_5x5_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_5x5_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_6x5_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_6x5_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_6x6_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_6x6_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_8x5_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_8x5_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_8x6_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_8x6_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_8x8_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_8x8_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_10x5_UNORM_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_10x5_SRGB_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_10x6_UNORM_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_10x6_SRGB_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_10x8_UNORM_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_10x8_SRGB_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_10x10_UNORM_BLOCK: // fallthrough
	        case ImageFormat::ASTC_10x10_SRGB_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_12x10_UNORM_BLOCK: // fallthrough
	        case ImageFormat::ASTC_12x10_SRGB_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_12x12_UNORM_BLOCK: // fallthrough
	        case ImageFormat::ASTC_12x12_SRGB_BLOCK:  // fallthrough

	        case ImageFormat::ASTC_4x4_SFLOAT_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_5x4_SFLOAT_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_5x5_SFLOAT_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_6x5_SFLOAT_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_6x6_SFLOAT_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_8x5_SFLOAT_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_8x6_SFLOAT_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_8x8_SFLOAT_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_10x5_SFLOAT_BLOCK: // fallthrough
	        case ImageFormat::ASTC_10x6_SFLOAT_BLOCK: // fallthrough
	        case ImageFormat::ASTC_10x8_SFLOAT_BLOCK: // fallthrough
	        case ImageFormat::ASTC_10x10_SFLOAT_BLOCK: // fallthrough
	        case ImageFormat::ASTC_12x10_SFLOAT_BLOCK: // fallthrough
	        case ImageFormat::ASTC_12x12_SFLOAT_BLOCK: // fallthrough
    			return true;

		    default: return false;
    	}
    }


	Vec2 GetCompressedFormatBlockExtent(ImageFormat format)
	{
    	switch (format)
    	{
    		case ImageFormat::BC1_RGB_UNORM_BLOCK:    // fallthrough
    		case ImageFormat::BC1_RGB_SRGB_BLOCK:     // fallthrough
    		case ImageFormat::BC1_RGBA_UNORM_BLOCK:   // fallthrough
    		case ImageFormat::BC1_RGBA_SRGB_BLOCK:    // fallthrough
    		case ImageFormat::BC2_UNORM_BLOCK:        // fallthrough
    		case ImageFormat::BC2_SRGB_BLOCK:         // fallthrough
    		case ImageFormat::BC3_UNORM_BLOCK:        // fallthrough
    		case ImageFormat::BC3_SRGB_BLOCK:         // fallthrough
    		case ImageFormat::BC4_UNORM_BLOCK:        // fallthrough
    		case ImageFormat::BC4_SNORM_BLOCK:        // fallthrough
    		case ImageFormat::BC5_UNORM_BLOCK:        // fallthrough
    		case ImageFormat::BC5_SNORM_BLOCK:        // fallthrough
    		case ImageFormat::BC6H_UFLOAT_BLOCK:      // fallthrough
    		case ImageFormat::BC6H_SFLOAT_BLOCK:      // fallthrough
    		case ImageFormat::BC7_UNORM_BLOCK:        // fallthrough
    		case ImageFormat::BC7_SRGB_BLOCK:         // fallthrough
    			return Vec2{4, 4};

	        case ImageFormat::EAC_R11_UNORM_BLOCK:    // fallthrough
	        case ImageFormat::EAC_R11_SNORM_BLOCK:    // fallthrough
	        case ImageFormat::EAC_R11G11_UNORM_BLOCK: // fallthrough
	        case ImageFormat::EAC_R11G11_SNORM_BLOCK: // fallthrough
    			return Vec2{4, 4};

	        case ImageFormat::ASTC_4x4_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_4x4_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_4x4_SFLOAT_BLOCK:  // fallthrough
    			return Vec2{4, 4};

	        case ImageFormat::ASTC_5x4_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_5x4_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_5x4_SFLOAT_BLOCK:  // fallthrough
    			return Vec2{5, 4};

	        case ImageFormat::ASTC_5x5_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_5x5_SRGB_BLOCK:    // fallthrough
    		case ImageFormat::ASTC_5x5_SFLOAT_BLOCK:  // fallthrough
    			return Vec2{5, 5};

	        case ImageFormat::ASTC_6x5_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_6x5_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_6x5_SFLOAT_BLOCK:  // fallthrough
    			return Vec2{6, 5};

	        case ImageFormat::ASTC_6x6_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_6x6_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_6x6_SFLOAT_BLOCK:  // fallthrough
    			return Vec2{6, 6};

	        case ImageFormat::ASTC_8x5_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_8x5_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_8x5_SFLOAT_BLOCK:  // fallthrough
    			return Vec2{8, 5};

	        case ImageFormat::ASTC_8x6_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_8x6_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_8x6_SFLOAT_BLOCK:  // fallthrough
    			return Vec2{8, 6};

	        case ImageFormat::ASTC_8x8_UNORM_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_8x8_SRGB_BLOCK:    // fallthrough
	        case ImageFormat::ASTC_8x8_SFLOAT_BLOCK:  // fallthrough
    			return Vec2{8, 8};

	        case ImageFormat::ASTC_10x5_UNORM_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_10x5_SRGB_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_10x5_SFLOAT_BLOCK: // fallthrough
    			return Vec2{10, 5};

	        case ImageFormat::ASTC_10x6_UNORM_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_10x6_SRGB_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_10x6_SFLOAT_BLOCK: // fallthrough
    			return Vec2{10, 6};

	        case ImageFormat::ASTC_10x8_UNORM_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_10x8_SRGB_BLOCK:   // fallthrough
	        case ImageFormat::ASTC_10x8_SFLOAT_BLOCK: // fallthrough
    			return Vec2{10, 8};

	        case ImageFormat::ASTC_10x10_UNORM_BLOCK: // fallthrough
	        case ImageFormat::ASTC_10x10_SRGB_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_10x10_SFLOAT_BLOCK: // fallthrough
    			return Vec2{10, 10};

	        case ImageFormat::ASTC_12x10_UNORM_BLOCK: // fallthrough
	        case ImageFormat::ASTC_12x10_SRGB_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_12x10_SFLOAT_BLOCK: // fallthrough
    			return Vec2{12, 10};

	        case ImageFormat::ASTC_12x12_UNORM_BLOCK: // fallthrough
	        case ImageFormat::ASTC_12x12_SRGB_BLOCK:  // fallthrough
	        case ImageFormat::ASTC_12x12_SFLOAT_BLOCK: // fallthrough
    			return Vec2{12, 12};

		    default: ASTRAL_ERROR("Unsupported format! Expected a compressed format!");
    	}
	}

}
