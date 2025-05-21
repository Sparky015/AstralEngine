/**
* @file VulkanIndexBuffer.h
* @author Andrew Fagan
* @date 5/21/2025
*/

#pragma once

#include "VulkanBuffer.h"
#include "VulkanDevice.h"
#include "Renderer/RHI/Resources/IndexBuffer.h"

namespace Astral {

    struct VulkanIndexBufferDesc
    {
        VulkanDevice& VulkanDevice;
        VkDevice Device;
        VkPhysicalDeviceMemoryProperties DeviceMemoryProperties;
        void* IndiceData;
        uint32 SizeInBytes;
    };

    class VulkanIndexBuffer : public IndexBuffer
    {
    public:
        explicit VulkanIndexBuffer(const VulkanIndexBufferDesc& desc);
        ~VulkanIndexBuffer() override;

        void Bind(CommandBufferHandle commandBufferHandle) override;

    private:

        void CreateIndexBuffer(const VulkanIndexBufferDesc& desc);

        VkDevice m_Device;
        VulkanBuffer m_IndexBuffer;
        uint32 m_SizeInBytes;
        void* m_IndiceData;
    };


}
