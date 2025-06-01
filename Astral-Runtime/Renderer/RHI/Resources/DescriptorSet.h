/**
* @file DescriptorSet.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Buffer.h"
#include "Sampler.h"
#include "Texture.h"
#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    enum class ShaderStage : uint8
    {
        VERTEX,
        FRAGMENT
    };

    class DescriptorSet
    {
    public:
        virtual ~DescriptorSet() = default;

        virtual void BeginBuildingSet() = 0;
        virtual void AddDescriptorStorageBuffer(BufferHandle bufferHandle, ShaderStage shaderStage) = 0;
        virtual void AddDescriptorUniformBuffer(BufferHandle bufferHandle, ShaderStage shaderStage) = 0;
        virtual void AddDescriptorImageSampler(TextureHandle textureHandle, ShaderStage bindStage) = 0;
        virtual void EndBuildingSet() = 0;

        virtual void* GetLayout() = 0;
        virtual void* GetNativeHandle() = 0;
    };

    using DescriptorSetHandle = GraphicsRef<DescriptorSet>;

}
