/**
* @file VulkanVertexBuffer.cpp
* @author Andrew Fagan
* @date 5/18/2025
*/

#include "VulkanVertexBuffer.h"

#include "Core/Utilities/Asserts.h"

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


    VulkanVertexBuffer::VulkanVertexBuffer(VulkanVertexBuffer&& other) noexcept :
        m_BufferLayout(std::move(other.m_BufferLayout)),
        m_VertexBuffer(std::move(other.m_VertexBuffer)),
        m_DataSize(other.m_DataSize)
    {
        other.m_DataSize = 0;
    }

    VulkanVertexBuffer& VulkanVertexBuffer::operator=(VulkanVertexBuffer&& other) noexcept
    {
        if (this == &other)
        {
            m_BufferLayout = std::move(other.m_BufferLayout);
            m_VertexBuffer = std::move(other.m_VertexBuffer);
            m_DataSize = other.m_DataSize;

            other.m_DataSize = 0;
        }

        return *this;
    }

    void VulkanVertexBuffer::CreateVertexBuffer(const VulkanVertexBufferDesc& desc)
    {
        if (desc.MemoryType == GPUMemoryType::DEVICE_LOCAL)
        {
            VulkanBufferDesc vertexBufferDesc = {
                .Device = desc.Device,
                .Size = desc.DataSize,
                .Usage = BUFFER_USAGE_VERTEX_BUFFER | BUFFER_USAGE_STREAMABLE,
                .MemoryType = GPUMemoryType::DEVICE_LOCAL,
                .DeviceMemoryProperties = desc.DeviceMemoryProperties,
            };

            m_VertexBuffer = VulkanBuffer{vertexBufferDesc};
            m_VertexBuffer.UploadToDeviceLocalBuffer(desc.VertexData, desc.DataSize);
        }
        else if (desc.MemoryType == GPUMemoryType::HOST_VISIBLE)
        {
            VulkanBufferDesc vertexBufferDesc = {
                .Device = desc.Device,
                .Size = desc.DataSize,
                .Usage = BUFFER_USAGE_VERTEX_BUFFER | BUFFER_USAGE_STREAMABLE,
                .MemoryType = GPUMemoryType::HOST_VISIBLE,
                .DeviceMemoryProperties = desc.DeviceMemoryProperties,
            };

            m_VertexBuffer = VulkanBuffer{vertexBufferDesc};
            m_VertexBuffer.CopyDataToBuffer(desc.VertexData, desc.DataSize);
        }
        else
        {
            AE_ERROR("[VulkanVertexBuffer::CreateVertexBuffer] Memory type not implemented!")
        }

    }

}
