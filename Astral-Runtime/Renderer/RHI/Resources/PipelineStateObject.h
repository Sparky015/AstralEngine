/**
* @file PipelineStateObject.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "CommandBuffer.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class PipelineStateObject
    {
    public:
        virtual ~PipelineStateObject() = default;

        virtual void Bind(CommandBufferHandle commandBufferHandle) = 0;
    };

    using PipelineStateObjectHandle = GraphicsRef<PipelineStateObject>;

}
