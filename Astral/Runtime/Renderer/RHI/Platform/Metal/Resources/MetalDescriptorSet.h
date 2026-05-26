/**
* @file MetalDescriptorSet.h
* @author Andrew Fagan
* @date 5/24/26
*/

#pragma once

#include "Renderer/RHI/Resources/DescriptorSet.h"

#include "Metal/MTLDevice.hpp"

namespace Astral {

    struct MetalDescriptorSetDesc
    {
        MTL::Device* Device;
    };

    class MetalDescriptorSet : public DescriptorSet // TODO
    {
    public:

        MetalDescriptorSet(const MetalDescriptorSetDesc& descriptorSetDesc); // TODO
        ~MetalDescriptorSet() override; // TODO

        void* GetNativeHandle() override; // TODO

    private:


    };

}