/**
* @file VulkanVertexBuffer.cpp
* @author Andrew Fagan
* @date 5/18/2025
*/

#include "VulkanVertexBuffer.h"

#include "Debug/Utilities/Asserts.h"

namespace Astral {

    VulkanVertexBuffer::VulkanVertexBuffer(const VulkanVertexBufferDesc& desc) :
        m_BufferLayout(desc.BufferLayout),
        m_VertexBuffer(),
        m_DataSize(desc.DataSize)
    {
        CreateVertexBuffer(desc);
    }


    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
    }


    uint32 VulkanVertexBuffer::GetSize() const
    {
        return m_DataSize;
    }


    const VertexBufferLayout& VulkanVertexBuffer::GetBufferLayout() const
    {
        return m_BufferLayout;
    }


    void VulkanVertexBuffer::MapPointer(void** cpuPtr)
    {
        m_VertexBuffer.MapPointer(cpuPtr);
    }


    void VulkanVertexBuffer::UnmapPointer()
    {
        m_VertexBuffer.UnmapPointer();
    }


    void VulkanVertexBuffer::CopyDataToBuffer(void* data, uint32 size)
    {
        m_VertexBuffer.CopyDataToBuffer(data, size);
    }


    void VulkanVertexBuffer::UploadToDeviceLocalBuffer(void* data, uint32 size)
    {
        m_VertexBuffer.UploadToDeviceLocalBuffer(data, size);
    }


    void VulkanVertexBuffer::ChangeMemoryType(GPUMemoryType memoryType)
    {
        m_VertexBuffer.ChangeMemoryType(memoryType);
    }


    void* VulkanVertexBuffer::GetNativeHandle()
    {
        return m_VertexBuffer.GetNativeHandle();
    }


    void VulkanVertexBuffer::CreateVertexBuffer(const VulkanVertexBufferDesc& desc)
    {
        if (desc.MemoryType == GPUMemoryType::DEVICE_LOCAL)
        {
            VulkanBufferDesc vertexBufferDesc = {
                .Device = desc.Device,
                .Size = desc.DataSize,
                .Usage = BUFFER_USAGE_VERTEX_BUFFER,
                .MemoryType = GPUMemoryType::DEVICE_LOCAL,
                .DeviceMemoryProperties = desc.DeviceMemoryProperties,
            };

            m_VertexBuffer = VulkanBuffer{vertexBufferDesc};
            m_VertexBuffer.UploadToDeviceLocalBuffer(desc.VerticeData, desc.DataSize);
        }
        else if (desc.MemoryType == GPUMemoryType::HOST_VISIBLE)
        {
            VulkanBufferDesc vertexBufferDesc = {
                .Device = desc.Device,
                .Size = desc.DataSize,
                .Usage = BUFFER_USAGE_VERTEX_BUFFER,
                .MemoryType = GPUMemoryType::HOST_VISIBLE,
                .DeviceMemoryProperties = desc.DeviceMemoryProperties,
            };

            m_VertexBuffer = VulkanBuffer{vertexBufferDesc};
            m_VertexBuffer.CopyDataToBuffer(desc.VerticeData, desc.DataSize);
        }
        else
        {
            AE_ERROR("[VulkanVertexBuffer::CreateVertexBuffer] Memory type not implemented!")
        }

    }

}
