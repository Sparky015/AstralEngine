/**
* @file DescriptorSet.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include <complex.h>
#include <complex.h>

#include "Buffer.h"
#include "Sampler.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    enum class BindStage : uint8
    {
        VERTEX,
        FRAGMENT
    };

    class DescriptorSet
    {
    public:
        virtual ~DescriptorSet() = default;

        virtual void BeginBuildingSet() = 0;
        virtual void AddDescriptorStorageBuffer(BufferHandle bufferHandle, BindStage bindStage) = 0;
        virtual void AddDescriptorUniformBuffer(BufferHandle bufferHandle) = 0;
        virtual void AddDescriptorImageSampler(SamplerHandle samplerHandle) = 0;
        virtual void EndBuildingSet() = 0;

        virtual void* GetLayout() = 0;
        virtual void* GetNativeHandle() = 0;
    };

    using DescriptorSetHandle = GraphicsRef<DescriptorSet>;

}
