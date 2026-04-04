/**
* @file MetalBuffer.h
* @author Andrew Fagan
* @date 4/4/26
*/

#pragma once

#include "Metal/MTLBuffer.hpp"
#include "Renderer/RHI/Resources/Buffer.h"

namespace Astral {

    struct MetalBufferDesc
    {
        MTL::Device* Device;
    };

    class MetalBuffer : public Buffer
    {
    public:
        MetalBuffer(const MetalBufferDesc& metalBufferDesc);
        virtual ~MetalBuffer() = default;

        uint32 GetAllocatedSize() override;
        uint32 GetUsedSize() override;
        void ReallocateMemory(uint32 newSize) override;
        void MapPointer(void** cpuPtr) override;
        void UnmapPointer() override;
        void CopyDataToBuffer(void* data, uint32 size) override;

        void* GetNativeHandle() override;

    private:

        void CreateBuffer();
        void DestroyBuffer();

        MTL::Device* m_Device;
        MTL::Buffer* m_Buffer;
    };

}