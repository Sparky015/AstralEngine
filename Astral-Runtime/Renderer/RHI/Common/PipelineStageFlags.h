/**
* @file PipelineStageFlags.h
* @author Andrew Fagan
* @date 7/2/2025
*/


#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    enum class PipelineStageFlags : uint8
    {
        TOP_OF_PIPE_BIT,
        DRAW_INDIRECT_BIT,
        VERTEX_INPUT_BIT,
        VERTEX_SHADER_BIT,
        TESSELLATION_CONTROL_SHADER_BIT,
        TESSELLATION_EVALUATION_SHADER_BIT,
        GEOMETRY_SHADER_BIT,
        FRAGMENT_SHADER_BIT,
        EARLY_FRAGMENT_TESTS_BIT,
        LATE_FRAGMENT_TESTS_BIT,
        COLOR_ATTACHMENT_OUTPUT_BIT,
        COMPUTE_SHADER_BIT,
        TRANSFER_BIT,
        BOTTOM_OF_PIPE_BIT,
        HOST_BIT,
        ALL_GRAPHICS_BIT,
        ALL_COMMANDS_BIT,
        NONE,
        ACCELERATION_STRUCTURE_BUILD_BIT,
        RAY_TRACING_SHADER_BIT,
        FRAGMENT_SHADING_RATE_ATTACHMENT_BIT
    };

}
