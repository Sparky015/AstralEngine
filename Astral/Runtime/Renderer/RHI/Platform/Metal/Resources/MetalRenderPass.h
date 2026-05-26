/**
* @file MetalRenderPass.h
* @author Andrew Fagan
* @date 5/24/26
*/

#pragma once

#include "Renderer/RHI/Resources/Renderpass.h"

#include "Metal/MTLDevice.hpp"

namespace Astral {

    struct MetalRenderPassDesc
    {
        MTL::Device* Device;
    };

    class MetalRenderPass : public RenderPass // TODO
    {
    public:

        MetalRenderPass(const MetalRenderPassDesc& renderPassDesc); // TODO
        ~MetalRenderPass() override; // TODO

        void* GetNativeHandle() override; // TODO

    private:


    };

}
