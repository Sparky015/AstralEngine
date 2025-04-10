/**
* @file FixedIntegerTypes.h
* @author Andrew Fagan
* @date 3/8/2025
*/


#pragma once

/** Removing the _t from specific size types for ease of use. */
#include <cstddef>
#include <cstdint>

namespace Astral {

    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;

    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;

}
