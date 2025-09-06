/**
* @file ImageUsageFlags.h
* @author Andrew Fagan
* @date 6/13/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    enum ImageUsageFlagBits : uint32
    {
        IMAGE_USAGE_TRANSFER_SRC_BIT                       = 1u << 0,
        IMAGE_USAGE_TRANSFER_DST_BIT                       = 1u << 1,
        IMAGE_USAGE_SAMPLED_BIT                            = 1u << 2,
        IMAGE_USAGE_STORAGE_BIT                            = 1u << 3,
        IMAGE_USAGE_COLOR_ATTACHMENT_BIT                   = 1u << 4,
        IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT           = 1u << 5,
        IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT               = 1u << 6,
        IMAGE_USAGE_INPUT_ATTACHMENT_BIT                   = 1u << 7,
        IMAGE_USAGE_VIDEO_DECODE_DST_BIT                   = 1u << 8,
        IMAGE_USAGE_VIDEO_DECODE_SRC_BIT                   = 1u << 9,
        IMAGE_USAGE_VIDEO_DECODE_DPB_BIT                   = 1u << 10,
        IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT               = 1u << 11,
        IMAGE_USAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT   = 1u << 12,
        IMAGE_USAGE_VIDEO_ENCODE_DST_BIT                   = 1u << 13,
        IMAGE_USAGE_VIDEO_ENCODE_SRC_BIT                   = 1u << 14,
        IMAGE_USAGE_VIDEO_ENCODE_DPB_BIT                   = 1u << 15,
        IMAGE_USAGE_ATTACHMENT_FEEDBACK_LOOP_BIT_EXT       = 1u << 16,
    };

    using ImageUsageFlags = uint32;

}
