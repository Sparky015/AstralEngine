/**
* @file PipelineStageFlags.h
* @author Andrew Fagan
* @date 7/2/2025
*/


#pragma once

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    enum PipelineStageFlagBits : uint32
    {
        PIPELINE_STAGE_NONE                                  =  0u,
        PIPELINE_STAGE_TOP_OF_PIPE_BIT                       =  1u,
        PIPELINE_STAGE_DRAW_INDIRECT_BIT                     =  1u << 1,
        PIPELINE_STAGE_VERTEX_INPUT_BIT                      =  1u << 2,
        PIPELINE_STAGE_VERTEX_SHADER_BIT                     =  1u << 3,
        PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT       =  1u << 4,
        PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT    =  1u << 5,
        PIPELINE_STAGE_GEOMETRY_SHADER_BIT                   =  1u << 6,
        PIPELINE_STAGE_FRAGMENT_SHADER_BIT                   =  1u << 7,
        PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT              =  1u << 8,
        PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT               =  1u << 9,
        PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT           =  1u << 10,
        PIPELINE_STAGE_COMPUTE_SHADER_BIT                    =  1u << 11,
        PIPELINE_STAGE_TRANSFER_BIT                          =  1u << 12,
        PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT                    =  1u << 13,
        PIPELINE_STAGE_HOST_BIT                              =  1u << 14,
        PIPELINE_STAGE_ALL_GRAPHICS_BIT                      =  1u << 15,
        PIPELINE_STAGE_ALL_COMMANDS_BIT                      =  1u << 16,
        PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT      =  1u << 17,
        PIPELINE_STAGE_RAY_TRACING_SHADER_BIT                =  1u << 18,
        PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT  =  1u << 19
    };

    using PipelineStageFlags = uint32;

}
