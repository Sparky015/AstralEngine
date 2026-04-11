/**
* @file MetalVertexBuffer.cpp
* @author Andrew Fagan
* @date 4/6/26
*/

#include "MetalVertexBuffer.h"

namespace Astral {

    MetalVertexBuffer::MetalVertexBuffer(const MetalVertexBufferDesc& desc) :
        m_BufferLayout(desc.BufferLayout),
        m_VertexBuffer(),
        m_DataSize(desc.DataSize)
    {
        CreateVertexBuffer(desc);
    }


    MetalVertexBuffer::~MetalVertexBuffer()
    {
    }


    uint32 MetalVertexBuffer::GetSize() const
    {
        return m_DataSize;
    }


    const VertexBufferLayout& MetalVertexBuffer::GetBufferLayout() const
    {
        return m_BufferLayout;
    }


    void MetalVertexBuffer::MapPointer(void** cpuPtr)
    {
        m_VertexBuffer.MapPointer(cpuPtr);
    }


    void MetalVertexBuffer::UnmapPointer()
    {
        m_VertexBuffer.UnmapPointer();
    }


    void MetalVertexBuffer::CopyDataToBuffer(void* data, uint32 size)
    {
        m_VertexBuffer.CopyDataToBuffer(data, size);
    }


    void MetalVertexBuffer::UploadToDeviceLocalBuffer(void* data, uint32 size)
    {
        m_VertexBuffer.UploadToDeviceLocalBuffer(data, size);
    }


    void MetalVertexBuffer::ChangeMemoryType(GPUMemoryType memoryType)
    {
        m_VertexBuffer.ChangeMemoryType(memoryType);
    }


    void* MetalVertexBuffer::GetNativeHandle()
    {
        return m_VertexBuffer.GetNativeHandle();
    }


    MetalVertexBuffer::MetalVertexBuffer(MetalVertexBuffer&& other) noexcept :
        m_BufferLayout(std::move(other.m_BufferLayout)),
        m_VertexBuffer(std::move(other.m_VertexBuffer)),
        m_DataSize(other.m_DataSize)
    {
        other.m_DataSize = 0;
    }


    MetalVertexBuffer& MetalVertexBuffer::operator=(MetalVertexBuffer&& other) noexcept
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


    void MetalVertexBuffer::CreateVertexBuffer(const MetalVertexBufferDesc& desc)
    {
        if (desc.MemoryType == GPUMemoryType::DEVICE_LOCAL)
        {
            MetalBufferDesc vertexBufferDesc = {
                .Device = desc.Device,
                .Size = desc.DataSize,
                .MemoryType = GPUMemoryType::DEVICE_LOCAL,
            };

            m_VertexBuffer = MetalBuffer{vertexBufferDesc};
            m_VertexBuffer.UploadToDeviceLocalBuffer(desc.VertexData, desc.DataSize);
        }
        else if (desc.MemoryType == GPUMemoryType::HOST_VISIBLE)
        {
            MetalBufferDesc vertexBufferDesc = {
                .Device = desc.Device,
                .Size = desc.DataSize,
                .MemoryType = GPUMemoryType::HOST_VISIBLE,
            };

            m_VertexBuffer = MetalBuffer{vertexBufferDesc};
            m_VertexBuffer.CopyDataToBuffer(desc.VertexData, desc.DataSize);
        }
        else
        {
            AE_ERROR("[VulkanVertexBuffer::CreateVertexBuffer] Memory type not implemented!")
        }
    }

}


