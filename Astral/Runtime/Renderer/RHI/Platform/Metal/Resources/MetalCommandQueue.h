/**
* @file MetalCommandQueue.h
* @author Andrew Fagan
* @date 5/26/26
*/

#pragma once

#include "Renderer/RHI/Resources/CommandQueue.h"

#include "Metal/MTLDevice.hpp"

namespace Astral {

    struct MetalCommandQueueDesc
    {
        MTL::Device* Device;
    };

    class MetalCommandQueue : public CommandQueue // TODO
    {
    public:

        MetalCommandQueue(const MetalCommandQueueDesc& commandQueueDesc); // TODO
        ~MetalCommandQueue() override; // TODO

        void* GetNativeHandle() override; // TODO

    private:


    };

}

