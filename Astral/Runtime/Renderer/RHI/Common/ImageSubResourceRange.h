/**
* @file ImageSubResourceRange.h
* @author Andrew Fagan
* @date 7/13/2025
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    enum ImageAspectFlagBits : uint32
    {
        IMAGE_ASPECT_COLOR_BIT      =   1u,
        IMAGE_ASPECT_DEPTH_BIT      =   1u << 1,
        IMAGE_ASPECT_STENCIL_BIT    =   1u << 2,
        IMAGE_ASPECT_METADATA_BIT   =   1u << 3,
        IMAGE_ASPECT_PLANE_0_BIT    =   1u << 4,
        IMAGE_ASPECT_PLANE_1_BIT    =   1u << 5,
        IMAGE_ASPECT_PLANE_2_BIT    =   1u << 6,
    };

    using ImageAspectFlags = uint32;

    struct ImageSubResourceRange
    {
        ImageAspectFlags AspectMask;
        uint32 BaseMipLevel;
        uint32 LevelCount;
        uint32 BaseArrayLayer;
        uint32 LayerCount;
    };

}
