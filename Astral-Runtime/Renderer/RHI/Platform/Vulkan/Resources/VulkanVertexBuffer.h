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
        VulkanDevice& VulkanDevice;
        VkDevice Device;
        void* VerticeData;
        uint32 Size;
        VkPhysicalDeviceMemoryProperties DeviceMemoryProperties;
    };

    class VulkanVertexBuffer : public VertexBuffer
    {
    public:
        explicit VulkanVertexBuffer(const VulkanVertexBufferDesc& desc);
        ~VulkanVertexBuffer() override;

        void* GetVerticeData() override { return m_VerticeData; }
        uint32 GetSize() override { return m_Size; }

        void* GetNativeHande() override { return m_VertexBuffer.GetNativeHandle(); }

    private:

        void CreateVertexBuffer(const VulkanVertexBufferDesc& desc);
        void DestroyVertexBuffer();

        VkDevice m_Device;
        VulkanBuffer m_VertexBuffer;
        uint32 m_Size;
        void* m_VerticeData;
    };

}
