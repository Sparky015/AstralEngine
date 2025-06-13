/**
* @file ImageUsageFlags.h
* @author Andrew Fagan
* @date 6/13/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    enum class ImageUsageFlags : uint8
    {
        TRANSFER_SRC_BIT,
        TRANSFER_DST_BIT,
        SAMPLED_BIT,
        STORAGE_BIT,
        COLOR_ATTACHMENT_BIT,
        DEPTH_STENCIL_ATTACHMENT_BIT,
        TRANSIENT_ATTACHMENT_BIT,
        INPUT_ATTACHMENT_BIT,
        HOST_TRANSFER_BIT,
        VIDEO_DECODE_DST_BIT,
        VIDEO_DECODE_SRC_BIT,
        VIDEO_DECODE_DPB_BIT,
        FRAGMENT_DENSITY_MAP_BIT,
        FRAGMENT_SHADING_RATE_ATTACHMENT_BIT,
        VIDEO_ENCODE_DST_BIT,
        VIDEO_ENCODE_SRC_BIT,
        VIDEO_ENCODE_DPB_BIT,
        ATTACHMENT_FEEDBACK_LOOP_BIT_EXT,
        VIDEO_ENCODE_QUANTIZATION_DELTA_MAP_BIT,
        VIDEO_ENCODE_EMPHASIS_MAP_BIT,
    };

}
