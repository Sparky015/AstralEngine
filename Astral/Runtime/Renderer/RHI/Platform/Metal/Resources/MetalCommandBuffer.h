/**
* @file MetalCommandBuffer.h
* @author Andrew Fagan
* @date 5/24/26
*/

#pragma once

#include "Renderer/RHI/Resources/CommandBuffer.h"

#include "Metal/MTLDevice.hpp"

namespace Astral {

    struct MetalCommandBufferDesc
    {
        MTL::Device* Device;
    };

    class MetalCommandBuffer : public CommandBuffer // TODO
    {
    public:

        MetalCommandBuffer(const MetalCommandBufferDesc& computePipelineStateDesc); // TODO
        ~MetalCommandBuffer() override; // TODO

        void* GetNativeHandle() override; // TODO

    private:


    };

}
