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
        VulkanDevice& VulkanDevice;
        VkDevice Device;
        VkPhysicalDeviceMemoryProperties DeviceMemoryProperties;
        uint32* IndiceData;
        uint32 SizeInBytes;
        GPUMemoryType MemoryType;
    };

    class VulkanIndexBuffer : public IndexBuffer
    {
    public:
        explicit VulkanIndexBuffer(const VulkanIndexBufferDesc& desc);
        ~VulkanIndexBuffer() override;

        void MapPointer(void** cpuPtr) override;
        void UnmapPointer() override;
        void CopyDataToBuffer(void* data, uint32 size) override;

        uint32 GetCount() const override { return m_SizeInBytes / sizeof(uint32); }

        void* GetNativeHandle() override { return m_IndexBuffer.GetNativeHandle(); }

    private:

        void CreateIndexBuffer(const VulkanIndexBufferDesc& desc);

        VkDevice m_Device;
        VulkanBuffer m_IndexBuffer;
        uint32 m_SizeInBytes;
        void* m_IndiceData;
    };


}
