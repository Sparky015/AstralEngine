/**
* @file DescriptorSet.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class DescriptorSet
    {
    public:
        virtual ~DescriptorSet() = default;

        virtual void* GetLayout() = 0;
        virtual void* GetNativeHandle() = 0;
    };

    using DescriptorSetHandle = GraphicsRef<DescriptorSet>;

}
