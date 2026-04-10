/**
* @file VulkanIndexBuffer.h
* @author Andrew Fagan
* @date 5/21/2025
*/

#pragma once

#include "VulkanBuffer.h"
#include "VulkanDevice.h"
#include "Renderer/RHI/Common/MemoryTypes.h"
#include "Renderer/RHI/Resources/IndexBuffer.h"

namespace Astral {

    struct VulkanIndexBufferDesc
    {
        VkDevice Device;
        uint32* IndiceData;
        uint32 DataSize;
        GPUMemoryType MemoryType;
        VkPhysicalDeviceMemoryProperties DeviceMemoryProperties;
    };

    class VulkanIndexBuffer : public IndexBuffer
    {
    public:
        explicit VulkanIndexBuffer(const VulkanIndexBufferDesc& desc);
        ~VulkanIndexBuffer() override;

        void MapPointer(void** cpuPtr) override;
        void UnmapPointer() override;
        void CopyDataToBuffer(void* data, uint32 size) override;

        uint32 GetCount() const override { return m_DataSize / sizeof(uint32); }

        void* GetNativeHandle() override { return m_IndexBuffer.GetNativeHandle(); }

    private:

        void CreateIndexBuffer(const VulkanIndexBufferDesc& desc);

        VulkanBuffer m_IndexBuffer;
        uint32 m_DataSize;
    };


}
