/**
* @file VulkanVertexBuffer.h
* @author Andrew Fagan
* @date 5/18/2025
*/


#pragma once

#include "Renderer/RHI/Resources/VertexBuffer.h"
#include "Renderer/RHI/Resources/Buffer.h"
#include "VulkanPhysicalDevices.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"

#include <vulkan/vulkan_core.h>


namespace Astral {

    struct VulkanVertexBufferDesc
    {
        VkDevice Device;
        void* VerticeData;
        uint32 DataSize;
        VertexBufferLayout& BufferLayout;
        GPUMemoryType MemoryType;
        VkPhysicalDeviceMemoryProperties DeviceMemoryProperties;
    };

    class VulkanVertexBuffer : public VertexBuffer
    {
    public:
        explicit VulkanVertexBuffer(const VulkanVertexBufferDesc& desc);
        ~VulkanVertexBuffer() override;

        uint32 GetSize() const override;
        const VertexBufferLayout& GetBufferLayout() const override;

        void MapPointer(void** cpuPtr) override;
        void UnmapPointer() override;
        void CopyDataToBuffer(void* data, uint32 size) override;

        void* GetNativeHandle() override;

    private:

        void CreateVertexBuffer(const VulkanVertexBufferDesc& desc);

        VertexBufferLayout m_BufferLayout;
        VulkanBuffer m_VertexBuffer;
        uint32 m_DataSize;
    };

}
