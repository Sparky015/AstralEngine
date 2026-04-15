/**
* @file MetalBuffer.cpp
* @author Andrew Fagan
* @date 4/4/26
*/

#include "MetalBuffer.h"

#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Loggers.h"
#include "Renderer/RHI/RendererAPI.h"

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


    uint32 MetalBuffer::GetAllocatedSize() const
    {
        if (!m_Buffer) { AE_WARN("Buffer has not been created! Can't get allocated size!"); return 0; }
        return m_Buffer->allocatedSize();
    }


    uint32 MetalBuffer::GetUsedSize() const
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
            *cpuPtr = nullptr;
            return;
        }

        if (m_Buffer->storageMode() == MTL::StorageModePrivate)
        {
            AE_WARN("[MetalBuffer::MapPointer] Buffer does not support read/write from CPU on private memory!")
            *cpuPtr = nullptr;
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
            AE_WARN("[MetalBuffer::CopyDataToBuffer] Private buffer type does not support read/write from CPU!")
            return;
        }

        void* memory = nullptr;
        MapPointer(&memory);
        memcpy(memory, data, size);
        UnmapPointer();

       if (size > m_UsedMemorySize) { m_UsedMemorySize = size; }
    }


    void MetalBuffer::UploadToDeviceLocalBuffer(void* data, uint32 size)
    {
        if (!data || size == 0) { return; }

        ASSERT(size <= m_BufferLength, "Data does not fit in buffer!")

        if (m_Buffer->storageMode() != MTL::StorageModePrivate)
        {
            AE_WARN("[MetalBuffer::UploadToPrivateBuffer] A private buffer is required to upload data!")
            return;
        }

        MetalBufferDesc stagingBufferDesc = {
            .Device = m_Device,
            .Size = size,
            .MemoryType = GPUMemoryType::HOST_VISIBLE
        };

        MetalBuffer stagingBuffer = MetalBuffer(stagingBufferDesc);
        stagingBuffer.CopyDataToBuffer(data, size);

        this->CopyFromStagingBuffer(stagingBuffer, size);
    }


    void MetalBuffer::ChangeMemoryType(GPUMemoryType memoryType)
    {
        if (m_MemoryType == memoryType) { return; }

        if (memoryType == GPUMemoryType::DEVICE_LOCAL)
        {
            MetalBufferDesc deviceLocalBufferDesc = {
                .Device = m_Device,
                .Size = m_BufferLength,
                .MemoryType = GPUMemoryType::DEVICE_LOCAL
            };

            MetalBuffer deviceLocalbuffer = MetalBuffer(deviceLocalBufferDesc);
            deviceLocalbuffer.UploadToDeviceLocalBuffer(m_Buffer->contents(), m_BufferLength);

            // Replace this buffer with new buffer
            *this = std::move(deviceLocalbuffer);
        }
        else if (memoryType == GPUMemoryType::HOST_VISIBLE)
        {
            MetalBufferDesc hostVisibleBufferDesc = {
                .Device = m_Device,
                .Size = m_BufferLength,
                .MemoryType = GPUMemoryType::HOST_VISIBLE
            };

            MetalBuffer hostVisibleBuffer = MetalBuffer(hostVisibleBufferDesc);
            hostVisibleBuffer.CopyFromStagingBuffer(*this, m_BufferLength); // Copies from the private buffer back to a shared buffer

            // Replace this buffer with new buffer
            *this = std::move(hostVisibleBuffer);
        }
        else
        {
            AE_ERROR("[MetalBuffer::ChangeMemoryType] Memory type not implemented!")
        }
    }


    void* MetalBuffer::GetNativeHandle()
    {
        return m_Buffer;
    }


    void MetalBuffer::CopyFromStagingBuffer(Buffer& stagingBuffer, uint32 size)
    {
        ASSERT(size <= m_BufferLength, "Data does not fit in buffer!")
        MTL::Buffer* metalStagingBuffer = (MTL::Buffer*)stagingBuffer.GetNativeHandle();

        CommandBufferHandle commandBufferHandle = RendererAPI::GetDevice().AllocateCommandBuffer();
        MTL::CommandBuffer* commandBuffer = (MTL::CommandBuffer*)commandBufferHandle->GetNativeHandle();


        commandBufferHandle->BeginRecording();
        MTL::BlitCommandEncoder* blitEncoder = commandBuffer->blitCommandEncoder();

        blitEncoder->copyFromBuffer(metalStagingBuffer, 0, m_Buffer, 0, size);

        blitEncoder->endEncoding();
        commandBufferHandle->EndRecording();

        CommandQueueHandle commandQueueHandle = RendererAPI::GetDevice().GetPrimaryCommandQueue();
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
        if (this != &other)
        {
            DestroyBuffer(m_Buffer); // Clean up old buffer if one exists

            m_Device = other.m_Device;
            m_Buffer = other.m_Buffer;
            m_BufferLength = other.m_BufferLength;
            m_UsedMemorySize = other.m_UsedMemorySize;
            m_MemoryType = other.m_MemoryType;

            other.m_Device = nullptr;
            other.m_Buffer = nullptr;
            other.m_BufferLength = 0;
            other.m_UsedMemorySize = 0;
        }

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
