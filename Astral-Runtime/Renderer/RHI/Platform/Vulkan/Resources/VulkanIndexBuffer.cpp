/**
* @file VulkanIndexBuffer.cpp
* @author Andrew Fagan
* @date 5/21/2025
*/

#include "VulkanIndexBuffer.h"

namespace Astral {

    VulkanIndexBuffer::VulkanIndexBuffer(const VulkanIndexBufferDesc& desc) :
        m_IndexBuffer(),
        m_DataSize(desc.DataSize)
    {
        CreateIndexBuffer(desc);
    }

    VulkanIndexBuffer::~VulkanIndexBuffer()
    {

    }


    void VulkanIndexBuffer::MapPointer(void** cpuPtr)
    {
        m_IndexBuffer.MapPointer(cpuPtr);
    }


    void VulkanIndexBuffer::UnmapPointer()
    {
        m_IndexBuffer.UnmapPointer();
    }


    void VulkanIndexBuffer::CopyDataToBuffer(void* data, uint32 size)
    {
        m_IndexBuffer.CopyDataToBuffer(data, size);
    }


    void VulkanIndexBuffer::CreateIndexBuffer(const VulkanIndexBufferDesc& desc)
    {
        if (desc.MemoryType == GPUMemoryType::DEVICE_LOCAL)
        {
            VulkanBufferDesc indexBufferDesc = {
                .Device = desc.Device,
                .Size = desc.DataSize,
                .Usage = BUFFER_USAGE_INDEX_BUFFER,
                .MemoryType = GPUMemoryType::DEVICE_LOCAL,
                .DeviceMemoryProperties = desc.DeviceMemoryProperties,
            };

            m_IndexBuffer = VulkanBuffer{indexBufferDesc};
            m_IndexBuffer.UploadToDeviceLocalBuffer(desc.IndiceData, desc.DataSize);
        }
        else if (desc.MemoryType == GPUMemoryType::HOST_VISIBLE)
        {
            VulkanBufferDesc stagingBufferDesc = {
                .Device = desc.Device,
                .Size = desc.DataSize,
                .Usage = BUFFER_USAGE_INDEX_BUFFER,
                .MemoryType = GPUMemoryType::HOST_VISIBLE,
                .DeviceMemoryProperties = desc.DeviceMemoryProperties,
            };

            m_IndexBuffer = VulkanBuffer(stagingBufferDesc);
            m_IndexBuffer.CopyDataToBuffer(desc.IndiceData, desc.DataSize);
        }
    }

}
