/**
* @file PipelineStateObject.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "CommandBuffer.h"
#include "DescriptorSet.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

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

    class PipelineStateObject
    {
    public:
        virtual ~PipelineStateObject() = default;

        virtual void Bind(CommandBufferHandle commandBufferHandle) = 0;
        virtual void BindDescriptorSet(CommandBufferHandle commandBufferHandle, DescriptorSetHandle descriptorSetHandle, uint32 binding) = 0;

        virtual void* GetPipelineLayout() = 0;
        virtual void* GetHandleHandle() = 0;
    };

    using PipelineStateObjectHandle = GraphicsRef<PipelineStateObject>;

}
