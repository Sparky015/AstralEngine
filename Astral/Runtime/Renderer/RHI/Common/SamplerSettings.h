/**
* @file SamplerSettings.h
* @author Andrew Fagan
* @date 9/10/25
*/

#pragma once

namespace Astral {

    enum class SamplerFilter
    {
        NEAREST,
        LINEAR
    };

    enum class SamplerAddressMode
    {
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER,
        MIRROR_CLAMP_TO_EDGE
    };

}
