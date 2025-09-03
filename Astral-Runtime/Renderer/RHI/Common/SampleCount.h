/**
* @file SampleCount.h
* @author Andrew Fagan
* @date 9/2/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    enum class SampleCount : uint32
    {
        SAMPLE_1_BIT,
        SAMPLE_2_BIT,
        SAMPLE_4_BIT,
        SAMPLE_8_BIT,
        SAMPLE_16_BIT,
        SAMPLE_32_BIT,
        SAMPLE_64_BIT
    };

}
