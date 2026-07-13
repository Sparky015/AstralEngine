/**
* @file SampleCount.h
* @author Andrew Fagan
* @date 9/2/25
*/

#pragma once

#include "Core/FixedIntegerTypes.h"
#include "Core/Utilities/Error.h"

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

    inline uint32 ConvertSampleCountToIntSampleCount(SampleCount sampleCount)
    {
        switch (sampleCount)
        {
            case SampleCount::SAMPLE_1_BIT:   return 1;
            case SampleCount::SAMPLE_2_BIT:   return 2;
            case SampleCount::SAMPLE_4_BIT:   return 4;
            case SampleCount::SAMPLE_8_BIT:   return 8;
            case SampleCount::SAMPLE_16_BIT:  return 16;
            case SampleCount::SAMPLE_32_BIT:  return 32;
            case SampleCount::SAMPLE_64_BIT:  return 64;
            default: AE_ERROR("Unsupported sample count")
        }
    }

}
