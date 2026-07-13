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
    };

    using ImageUsageFlags = uint32;

}
