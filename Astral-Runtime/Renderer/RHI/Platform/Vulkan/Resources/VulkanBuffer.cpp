/**
* @file VulkanBuffer.cpp
* @author Andrew Fagan
* @date 5/17/2025
*/

#include "VulkanBuffer.h"

#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Error.h"
#include "Debug/Utilities/Loggers.h"
#include "Renderer/RHI/RendererAPI.h"

namespace Astral {

    VulkanBuffer::VulkanBuffer(const VulkanBufferDesc& desc) :
        m_Device(desc.Device),
        m_Usage(desc.Usage),
        m_UsedMemorySize(desc.Size),
        m_DeviceMemoryProperties(desc.DeviceMemoryProperties),
        m_RequestedPropertyFlags(desc.RequestedMemoryPropertyFlags),
        m_Buffer(),
        m_BufferMemory(),
        m_BufferDeviceSize(),
        m_IsDeviceMemoryMapped(false)
    {
        CreateBuffer(&m_Buffer, m_UsedMemorySize);
        m_BufferDeviceSize = AllocateMemory(&m_BufferMemory, m_Buffer);

        m_MemoryType = desc.RequestedMemoryPropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT ? GPUMemoryType::DEVICE_LOCAL : GPUMemoryType::HOST_VISIBLE;
    }

    VulkanBuffer::~VulkanBuffer()
    {
        DestroyBuffer(m_Buffer);
        FreeMemory(m_BufferMemory);
    }


    uint32 VulkanBuffer::GetAllocatedSize()
    {
        return m_BufferDeviceSize;
    }


    void VulkanBuffer::MapPointer(void** cpuPtr)
    {
        if (!cpuPtr)
        {
            AE_WARN("[VulkanBuffer::MapPointer] Can't map to nullptr")
            return;
        }
        if (m_MemoryType == GPUMemoryType::DEVICE_LOCAL)
        {
            AE_WARN("[VulkanBuffer::MapPointer] Buffer does not support read/write from CPU on device local memory!")
            return;
        }
        if (m_IsDeviceMemoryMapped)
        {
            vkUnmapMemory(m_Device, m_BufferMemory);
        }

        VkResult result = vkMapMemory(m_Device, m_BufferMemory, 0, m_BufferDeviceSize, 0, cpuPtr);
        ASSERT(result == VK_SUCCESS, "Failed to map memory in buffer")

        m_IsDeviceMemoryMapped = true;
    }


    void VulkanBuffer::UnmapPointer()
    {
        if (m_IsDeviceMemoryMapped)
        {
            vkUnmapMemory(m_Device, m_BufferMemory);
            m_IsDeviceMemoryMapped = false;
        }
    }


    void VulkanBuffer::CopyDataToBuffer(void* data, uint32 size)
    {
        if (!data) { return; }

        ASSERT(size <= m_BufferDeviceSize, "Data does not fit in buffer!")

        void* memory = nullptr;
        MapPointer(&memory);
        memcpy(memory, data, size);
        UnmapPointer();

        if (size > m_UsedMemorySize) { m_UsedMemorySize = size; }
    }


    void VulkanBuffer::UploadToDeviceLocalBuffer(void* data, uint32 size)
    {
        if (!data || size == 0) { return; }

        ASSERT(size <= m_BufferDeviceSize, "Data does not fit in buffer!")

        if (m_MemoryType != GPUMemoryType::DEVICE_LOCAL)
        {
            AE_WARN("[VulkanBuffer::UploadToPrivateBuffer] A device local buffer is required to upload data!")
            return;
        }

        VulkanBufferDesc stagingBufferDesc = {
            .Device = m_Device,
            .Usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .Size = size,
            .DeviceMemoryProperties = m_DeviceMemoryProperties,
            .RequestedMemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        };

        VulkanBuffer stagingBuffer = VulkanBuffer(stagingBufferDesc);
        stagingBuffer.CopyDataToBuffer(data, size);

        this->CopyFromStagingBuffer(stagingBuffer, size);
    }


    void VulkanBuffer::ChangeMemoryType(GPUMemoryType memoryType)
    {
        if (m_MemoryType == memoryType) { return; }

        if (memoryType == GPUMemoryType::DEVICE_LOCAL)
        {
            VulkanBufferDesc deviceLocalBufferDesc = {
                .Device = m_Device,
                .Usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                .Size = (uint32)m_BufferDeviceSize,
                .DeviceMemoryProperties = m_DeviceMemoryProperties,
                .RequestedMemoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
            };

            VulkanBuffer deviceLocalbuffer = VulkanBuffer(deviceLocalBufferDesc);

            void* bufferMemory;
            this->MapPointer(&bufferMemory);
            deviceLocalbuffer.UploadToDeviceLocalBuffer(bufferMemory, m_BufferDeviceSize);
            this->UnmapPointer();

            // Replace this buffer with new buffer
            *this = std::move(deviceLocalbuffer);
        }
        else if (memoryType == GPUMemoryType::HOST_VISIBLE)
        {
            VulkanBufferDesc hostVisibleBufferDesc = {
                .Device = m_Device,
                .Usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                .Size = (uint32)m_BufferDeviceSize,
                .DeviceMemoryProperties = m_DeviceMemoryProperties,
                .RequestedMemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
            };

            VulkanBuffer hostVisibleBuffer = VulkanBuffer(hostVisibleBufferDesc);
            hostVisibleBuffer.CopyFromStagingBuffer(*this, m_BufferDeviceSize); // Copies from the device local buffer back to a host visible buffer

            // Replace this buffer with new buffer
            *this = std::move(hostVisibleBuffer);
        }
        else
        {
            AE_ERROR("[MetalBuffer::ChangeMemoryType] Memory type not implemented!")
        }
    }


    void* VulkanBuffer::GetNativeHandle()
    {
        return m_Buffer;
    }


    void VulkanBuffer::CopyFromStagingBuffer(Buffer& stagingBuffer, uint32 size)
    {
        ASSERT(size <= m_BufferDeviceSize, "Data does not fit in buffer!")

        CommandBufferHandle commandBufferHandle = RendererAPI::GetDevice().AllocateCommandBuffer();
        VkCommandBuffer commandBuffer = (VkCommandBuffer)commandBufferHandle->GetNativeHandle();
        VkBuffer vkStagingBuffer = (VkBuffer)stagingBuffer.GetNativeHandle();

        VkBufferCopy bufferCopy = {
            .srcOffset = 0,
            .dstOffset = 0,
            .size = size
        };

        commandBufferHandle->BeginRecording();
        vkCmdCopyBuffer(commandBuffer, vkStagingBuffer, m_Buffer, 1, &bufferCopy);
        commandBufferHandle->EndRecording();

        CommandQueueHandle commandQueueHandle = RendererAPI::GetDevice().GetPrimaryCommandQueue();
        commandQueueHandle->SubmitSync(commandBufferHandle);
        commandQueueHandle->WaitIdle();
    }


    uint32 VulkanBuffer::GetUsedSize()
    {
        return m_UsedMemorySize;
    }


    void VulkanBuffer::ReallocateMemory(uint32 newSize)
    {
        if (newSize <= m_BufferDeviceSize)
        {
            AE_WARN("New size is less than or equal to current buffer size. Skipping reallocation!")
            return;
        }

        // Creating new buffer with at least newSize bytes of device memory
        VkBuffer newBuffer = nullptr;
        CreateBuffer(&newBuffer, newSize);
        VkDeviceMemory newDeviceMemory = nullptr;
        uint32 deviceBufferSize = AllocateMemory(&newDeviceMemory, newBuffer);

        // Copying data from current buffer to new buffer
        void* currentBufferPtr;
        MapPointer(&currentBufferPtr);

        void* newBufferPtr;
        VkResult result = vkMapMemory(m_Device, newDeviceMemory, 0, deviceBufferSize, 0, &newBufferPtr);
        ASSERT(result == VK_SUCCESS, "Failed to map new memory in buffer")

        memcpy(newBufferPtr, currentBufferPtr, m_BufferDeviceSize);

        UnmapPointer();
        vkUnmapMemory(m_Device, newDeviceMemory);

        m_Buffer = newBuffer;
        m_BufferMemory = newDeviceMemory;
        m_BufferDeviceSize = deviceBufferSize;

        // Cleaning up old buffer
        FreeMemory(m_BufferMemory);
        DestroyBuffer(m_Buffer);
    }


    VulkanBuffer::VulkanBuffer(VulkanBuffer&& other) noexcept :
        m_Device(other.m_Device),
        m_Usage(other.m_Usage),
        m_UsedMemorySize(other.m_UsedMemorySize),
        m_DeviceMemoryProperties(other.m_DeviceMemoryProperties),
        m_RequestedPropertyFlags(other.m_RequestedPropertyFlags),
        m_Buffer(other.m_Buffer),
        m_BufferMemory(other.m_BufferMemory),
        m_BufferDeviceSize(other.m_BufferDeviceSize),
        m_IsDeviceMemoryMapped(other.m_IsDeviceMemoryMapped),
        m_MemoryType(other.m_MemoryType)
    {
        other.m_Device = VK_NULL_HANDLE;
        other.m_Usage = 0;
        other.m_UsedMemorySize = 0;
        other.m_DeviceMemoryProperties = VkPhysicalDeviceMemoryProperties();
        other.m_RequestedPropertyFlags = 0;

        other.m_Buffer = VK_NULL_HANDLE;
        other.m_BufferMemory = VK_NULL_HANDLE;
        other.m_BufferDeviceSize = 0;
        other.m_IsDeviceMemoryMapped = false;
    }


    VulkanBuffer& VulkanBuffer::operator=(VulkanBuffer&& other) noexcept
    {
        m_Usage = other.m_Usage;
        m_UsedMemorySize = other.m_UsedMemorySize;
        m_DeviceMemoryProperties = other.m_DeviceMemoryProperties;
        m_RequestedPropertyFlags = other.m_RequestedPropertyFlags;
        m_MemoryType = other.m_MemoryType;

        m_Device = other.m_Device;
        m_Buffer = other.m_Buffer;
        m_BufferMemory = other.m_BufferMemory;
        m_BufferDeviceSize = other.m_BufferDeviceSize;
        m_IsDeviceMemoryMapped = other.m_IsDeviceMemoryMapped;

        other.m_Device = VK_NULL_HANDLE;
        other.m_Usage = 0;
        other.m_UsedMemorySize = 0;
        other.m_DeviceMemoryProperties = VkPhysicalDeviceMemoryProperties();
        other.m_RequestedPropertyFlags = 0;

        other.m_Buffer = VK_NULL_HANDLE;
        other.m_BufferMemory = VK_NULL_HANDLE;
        other.m_BufferDeviceSize = 0;
        other.m_IsDeviceMemoryMapped = false;

        return *this;
    }


    void VulkanBuffer::CreateBuffer(VkBuffer* outBuffer, uint32 length)
    {
        VkBufferCreateInfo bufferInfo = {
            .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size = length,
            .usage = m_Usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        VkResult result = vkCreateBuffer(m_Device, &bufferInfo, nullptr, outBuffer);
        ASSERT(result == VK_SUCCESS, "Failed to create buffer!");
    }


    uint32 VulkanBuffer::AllocateMemory(VkDeviceMemory* outDeviceMemory, VkBuffer buffer)
    {
        VkMemoryRequirements memoryRequirements = {};
        vkGetBufferMemoryRequirements(m_Device, buffer, &memoryRequirements);

        uint32_t memoryTypeIndex = GetMemoryTypeIndex(memoryRequirements.memoryTypeBits);

        VkMemoryAllocateInfo memoryAllocationInfo = {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memoryRequirements.size,
            .memoryTypeIndex = memoryTypeIndex
        };

        VkResult result = vkAllocateMemory(m_Device, &memoryAllocationInfo, nullptr, outDeviceMemory);
        ASSERT(result == VK_SUCCESS, "Failed to allocate memory!");

        result = vkBindBufferMemory(m_Device, buffer, *outDeviceMemory, 0);
        ASSERT(result == VK_SUCCESS, "Failed to bind buffer memory!");

        return memoryRequirements.size;
    }


    void VulkanBuffer::DestroyBuffer(VkBuffer buffer)
    {
        if (buffer)
        {
            vkDestroyBuffer(m_Device, buffer, nullptr);
        }
    }


    void VulkanBuffer::FreeMemory(VkDeviceMemory deviceMemory)
    {
        if (deviceMemory)
        {
            vkFreeMemory(m_Device, deviceMemory, nullptr);
        }
    }


    uint32 VulkanBuffer::GetMemoryTypeIndex(uint32 memoryTypeBitsMask)
    {
        for (uint32 i = 0; i < m_DeviceMemoryProperties.memoryTypeCount; i++)
        {
            const VkMemoryType& memoryType = m_DeviceMemoryProperties.memoryTypes[i];
            uint32 currentBitmask = (1 << i);
            bool isCurrentMemoryTypeSupported = (memoryTypeBitsMask & currentBitmask);
            bool hasRequiredMemoryProperties = ((memoryType.propertyFlags & m_RequestedPropertyFlags) == m_RequestedPropertyFlags);
            if (isCurrentMemoryTypeSupported && hasRequiredMemoryProperties) { return i; }
        }

        AE_ERROR("Failed to find a suitable memory type for Buffer object!");
    }

}
