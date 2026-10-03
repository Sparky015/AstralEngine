/**
* @file AccessFlags.h
* @author Andrew Fagan
* @date 6/12/2025
*/

#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    enum AccessFlagBits : uint32
    {
        ACCESS_FLAGS_NONE                                         =     0u,
        ACCESS_FLAGS_INDIRECT_COMMAND_READ_BIT                    =     1u,
        ACCESS_FLAGS_INDEX_READ_BIT                               =     1u << 1,
        ACCESS_FLAGS_VERTEX_ATTRIBUTE_READ_BIT                    =     1u << 2,
        ACCESS_FLAGS_UNIFORM_READ_BIT                             =     1u << 3,
        ACCESS_FLAGS_INPUT_ATTACHMENT_READ_BIT                    =     1u << 4,
        ACCESS_FLAGS_SHADER_READ_BIT                              =     1u << 5,
        ACCESS_FLAGS_SHADER_WRITE_BIT                             =     1u << 6,
        ACCESS_FLAGS_COLOR_ATTACHMENT_READ_BIT                    =     1u << 7,
        ACCESS_FLAGS_COLOR_ATTACHMENT_WRITE_BIT                   =     1u << 8,
        ACCESS_FLAGS_DEPTH_STENCIL_ATTACHMENT_READ_BIT            =     1u << 9,
        ACCESS_FLAGS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT           =     1u << 10,
        ACCESS_FLAGS_TRANSFER_READ_BIT                            =     1u << 11,
        ACCESS_FLAGS_TRANSFER_WRITE_BIT                           =     1u << 12,
        ACCESS_FLAGS_HOST_READ_BIT                                =     1u << 13,
        ACCESS_FLAGS_HOST_WRITE_BIT                               =     1u << 14,
        ACCESS_FLAGS_MEMORY_READ_BIT                              =     1u << 15,
        ACCESS_FLAGS_MEMORY_WRITE_BIT                             =     1u << 16,
        ACCESS_FLAGS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT             =     1u << 17,
        ACCESS_FLAGS_TRANSFORM_FEEDBACK_COUNTER_READ_BIT_EXT      =     1u << 18,
        ACCESS_FLAGS_TRANSFORM_FEEDBACK_COUNTER_WRITE_BIT_EXT     =     1u << 19,
        ACCESS_FLAGS_CONDITIONAL_RENDERING_READ_BIT_EXT           =     1u << 20,
        ACCESS_FLAGS_COLOR_ATTACHMENT_READ_NONCOHERENT_BIT_EXT    =     1u << 21,
        ACCESS_FLAGS_FRAGMENT_DENSITY_MAP_READ_BIT_EXT            =     1u << 22,
        ACCESS_FLAGS_COMMAND_PREPROCESS_READ_BIT_EXT              =     1u << 23,
        ACCESS_FLAGS_COMMAND_PREPROCESS_WRITE_BIT_EXT             =     1u << 24,
    };

    using AccessFlags = uint32;

    static constexpr uint32 QueueFamilyIgnored = -1;

}