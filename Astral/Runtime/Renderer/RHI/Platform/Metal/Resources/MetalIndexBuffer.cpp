/**
* @file MetalIndexBuffer.cpp
* @author Andrew Fagan
* @date 4/10/26
*/

#include "MetalIndexBuffer.h"

namespace Astral {

    MetalIndexBuffer::MetalIndexBuffer(const MetalIndexBufferDesc& desc)
    {
        CreateIndexBuffer(desc);
    }


    MetalIndexBuffer::~MetalIndexBuffer()
    {
    }


    uint32 MetalIndexBuffer::GetCount() const
    {
        return m_DataSize / sizeof(uint32);
    }


    void MetalIndexBuffer::MapPointer(void** cpuPtr)
    {
        m_IndexBuffer.MapPointer(cpuPtr);
    }


    void MetalIndexBuffer::UnmapPointer()
    {
        m_IndexBuffer.UnmapPointer();
    }


    void MetalIndexBuffer::CopyDataToBuffer(void* data, uint32 size)
    {
        m_IndexBuffer.CopyDataToBuffer(data, size);
    }


    void MetalIndexBuffer::UploadToDeviceLocalBuffer(void* data, uint32 size)
    {
        m_IndexBuffer.UploadToDeviceLocalBuffer(data, size);
    }


    void MetalIndexBuffer::ChangeMemoryType(GPUMemoryType memoryType)
    {
        m_IndexBuffer.ChangeMemoryType(memoryType);
    }


    void* MetalIndexBuffer::GetNativeHandle()
    {
        return m_IndexBuffer.GetNativeHandle();
    }


    MetalIndexBuffer::MetalIndexBuffer(MetalIndexBuffer&& other) noexcept :
        m_IndexBuffer(std::move(other.m_IndexBuffer)),
        m_DataSize(other.m_DataSize)
    {
        other.m_DataSize = 0;
    }


    MetalIndexBuffer& MetalIndexBuffer::operator=(MetalIndexBuffer&& other) noexcept
    {
        if (this != &other)
        {
            m_IndexBuffer = std::move(other.m_IndexBuffer);
            m_DataSize = other.m_DataSize;

            other.m_DataSize = 0;
        }

        return *this;
    }


    void MetalIndexBuffer::CreateIndexBuffer(const MetalIndexBufferDesc& desc)
    {
        if (desc.MemoryType == GPUMemoryType::DEVICE_LOCAL)
        {
            MetalBufferDesc indexBufferDesc = {
                .Device = desc.Device,
                .Size = desc.DataSize,
                .MemoryType = GPUMemoryType::DEVICE_LOCAL,
            };

            m_IndexBuffer = MetalBuffer{indexBufferDesc};
            m_IndexBuffer.UploadToDeviceLocalBuffer(desc.IndexData, desc.DataSize);
        }
        else if (desc.MemoryType == GPUMemoryType::HOST_VISIBLE)
        {
            MetalBufferDesc indexBufferDesc = {
                .Device = desc.Device,
                .Size = desc.DataSize,
                .MemoryType = GPUMemoryType::HOST_VISIBLE,
            };

            m_IndexBuffer = MetalBuffer(indexBufferDesc);
            m_IndexBuffer.CopyDataToBuffer(desc.IndexData, desc.DataSize);
        }

        m_DataSize = desc.DataSize;
    }

}

