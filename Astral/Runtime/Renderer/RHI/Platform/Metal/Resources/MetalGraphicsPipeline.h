/**
* @file MetalGraphicsPipeline.h
* @author Andrew Fagan
* @date 5/24/26
*/

#pragma once

#include "Renderer/RHI/Resources/PipelineState.h"

#include "Metal/MTLDevice.hpp"

namespace Astral {

    struct MetalGraphicsPipelineStateDesc
    {
        MTL::Device* Device;
    };

    class MetalGraphicsPipelineState : public PipelineState // TODO
    {
    public:

        MetalGraphicsPipelineState(const MetalGraphicsPipelineStateDesc& graphicsPipelineStateDesc); // TODO
        ~MetalGraphicsPipelineState() override; // TODO

        void* GetNativeHandle() override; // TODO

    private:


    };

}