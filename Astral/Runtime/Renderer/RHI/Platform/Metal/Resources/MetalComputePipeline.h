/**
* @file MetalComputePipeline.h
* @author Andrew Fagan
* @date 5/24/26
*/

#pragma once

#include "Renderer/RHI/Resources/PipelineState.h"

#include "Metal/MTLDevice.hpp"

namespace Astral {

    struct MetalComputePipelineStateDesc
    {
        MTL::Device* Device;
    };

    class MetalComputePipelineState : public PipelineState // TODO
    {
    public:

        MetalComputePipelineState(const MetalComputePipelineStateDesc& computePipelineStateDesc); // TODO
        ~MetalComputePipelineState() override; // TODO

        void* GetNativeHandle() override; // TODO

    private:


    };

}