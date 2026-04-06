/**
* @file MetalBuffer.cpp
* @author Andrew Fagan
* @date 4/4/26
*/

#include "MetalBuffer.h"

#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Loggers.h"
#include "Metal/MTLBlitCommandEncoder.hpp"
#include "Metal/MTLCommandBuffer.hpp"

#include "Metal/MTLDevice.hpp"

namespace Astral {

    MetalBuffer::MetalBuffer(const MetalBufferDesc& metalBufferDesc) :
        m_Device(metalBufferDesc.Device),
        m_Buffer(nullptr),
        m_BufferLength(0),
        m_UsedMemorySize(0),
        m_MemoryType(metalBufferDesc.MemoryType)
    {
        ASSERT(metalBufferDesc.Device, "[MetalBuffer::MetalBuffer] Device argument was null!")

        CreateBuffer(&m_Buffer, metalBufferDesc.Size);
        m_BufferLength = m_Buffer->allocatedSize();
    }


    MetalBuffer::~MetalBuffer()
    {
        DestroyBuffer(m_Buffer);
    }


    uint32 MetalBuffer::GetAllocatedSize()
    {
        if (!m_Buffer) { AE_WARN("Buffer has not been created! Can't get allocated size!"); return 0; }
        return m_Buffer->allocatedSize();
    }


    uint32 MetalBuffer::GetUsedSize()
    {
        return m_UsedMemorySize;
    }


    void MetalBuffer::ReallocateMemory(uint32 newSize)
    {
        if (newSize <= m_BufferLength)
        {
            AE_WARN("New size is less than or equal to current buffer size. Skipping reallocation!")
            return;
        }

        if (m_Buffer->storageMode() == MTL::StorageModePrivate)
        {
            AE_WARN("[MetalBuffer::ReallocateMemory] Buffer does not support read/write from CPU!")
            return;
        }

        // Transfer data to a newly created buffer of the new size
        MTL::Buffer* newBuffer;
        CreateBuffer(&newBuffer, newSize);

        void* currentBufferMemory;
        MapPointer(&currentBufferMemory);

        memcpy(newBuffer->contents(), currentBufferMemory, m_BufferLength);

        UnmapPointer();

        // Clean up old buffer and update variables to new buffer

        DestroyBuffer(m_Buffer);
        m_BufferLength = newBuffer->allocatedSize();
        m_Buffer = newBuffer;
    }


    void MetalBuffer::MapPointer(void** cpuPtr)
    {
        if (!cpuPtr)
        {
            AE_WARN("[MetalBuffer::MapPointer] Can't map to nullptr")
            return;
        }

        if (!m_Buffer)
        {
            AE_WARN("[MetalBuffer::MapPointer] Buffer has not been created! Can't map pointer to buffer!")
            return;
        }

        if (m_Buffer->storageMode() == MTL::StorageModePrivate)
        {
            AE_WARN("[MetalBuffer::MapPointer] Buffer does not support read/write from CPU on private memory!")
            return;
        }

        *cpuPtr = m_Buffer->contents();
    }


    void MetalBuffer::UnmapPointer()
    {
        // Not supported by Metal
    }


    void MetalBuffer::CopyDataToBuffer(void* data, uint32 size)
    {
        if (!data || size == 0) { return; }

        ASSERT(size <= m_BufferLength, "Data does not fit in buffer!")

        if (m_Buffer->storageMode() == MTL::StorageModePrivate)
        {
            AE_WARN("[MetalBuffer::CopyDataToBuffer] Buffer does not support read/write from CPU!")
            return;
        }

        void* memory = nullptr;
        MapPointer(&memory);
        memcpy(memory, data, size);
        UnmapPointer();

       if (size > m_UsedMemorySize) { m_UsedMemorySize = size; }
    }


    void* MetalBuffer::GetNativeHandle()
    {
        return m_Buffer;
    }


    void MetalBuffer::CopyFromStagingBuffer(Device& device, Buffer& stagingBuffer, uint32 size)
    {
        ASSERT(size <= m_BufferLength, "Data does not fit in buffer!")
        MTL::Buffer* metalStagingBuffer = (MTL::Buffer*)stagingBuffer.GetNativeHandle();

        CommandBufferHandle commandBufferHandle = device.AllocateCommandBuffer();
        MTL::CommandBuffer* commandBuffer = (MTL::CommandBuffer*)commandBufferHandle->GetNativeHandle();


        commandBufferHandle->BeginRecording();
        MTL::BlitCommandEncoder* blitEncoder = commandBuffer->blitCommandEncoder();

        blitEncoder->copyFromBuffer(metalStagingBuffer, 0, m_Buffer, 0, size);

        blitEncoder->endEncoding();
        commandBufferHandle->EndRecording();

        CommandQueueHandle commandQueueHandle = device.GetPrimaryCommandQueue();
        commandQueueHandle->SubmitSync(commandBufferHandle);
        commandQueueHandle->WaitIdle();
    }


    MetalBuffer::MetalBuffer(MetalBuffer&& other) noexcept :
        m_Device(other.m_Device),
        m_Buffer(other.m_Buffer),
        m_BufferLength(other.m_BufferLength),
        m_UsedMemorySize(other.m_UsedMemorySize),
        m_MemoryType(other.m_MemoryType)
    {
        other.m_Device = nullptr;
        other.m_Buffer = nullptr;
        other.m_BufferLength = 0;
        other.m_UsedMemorySize = 0;
    }


    MetalBuffer& MetalBuffer::operator=(MetalBuffer&& other) noexcept
    {
        m_Device = other.m_Device;
        m_Buffer = other.m_Buffer;
        m_BufferLength = other.m_BufferLength;
        m_UsedMemorySize = other.m_UsedMemorySize;
        m_MemoryType = other.m_MemoryType;

        other.m_Device = nullptr;
        other.m_Buffer = nullptr;
        other.m_BufferLength = 0;
        other.m_UsedMemorySize = 0;

        return *this;
    }


    void MetalBuffer::CreateBuffer(MTL::Buffer** outBuffer, uint32 bufferLength)
    {
        if (!outBuffer)
        {
            AE_WARN("[MetalBuffer::CreateBuffer] Can't create new buffer to null address")
            return;
        }

        MTL::ResourceOptions resourceOptions = MTL::ResourceOptions();

        switch (m_MemoryType)
        {
            case GPUMemoryType::DEVICE_LOCAL: resourceOptions |= MTL::ResourceStorageModePrivate; break;
            case GPUMemoryType::HOST_VISIBLE: resourceOptions |= MTL::ResourceStorageModeShared; break;
            default: AE_ERROR("[MetalBuffer::CreateBuffer] Given memory type is not implemented!")
        }

        *outBuffer = m_Device->newBuffer(bufferLength, resourceOptions);
        ASSERT(*outBuffer, "MetalBuffer failed to be created!")
    }


    void MetalBuffer::DestroyBuffer(MTL::Buffer* buffer)
    {
        if (buffer)
        {
            buffer->release();
        }
    }

}
