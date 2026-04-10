/**
* @file BufferUsageFlags.h
* @author Andrew Fagan
* @date 4/10/26
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    enum BufferUsageFlagBits : uint8
    {
        BUFFER_USAGE_VERTEX_BUFFER   =  1u,
        BUFFER_USAGE_INDEX_BUFFER    =  1u << 1,
        BUFFER_USAGE_STORAGE_BUFFER  =  1u << 2,
        BUFFER_USAGE_UNIFORM_BUFFER  =  1u << 3,
        BUFFER_USAGE_STREAMABLE      =  1u << 4,
        BUFFER_USAGE_GPU_ONLY        =  1u << 5
    };

    using BufferUsageFlags = uint8;

}