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
        uint32 SizeInBytes;
        VkPhysicalDeviceMemoryProperties DeviceMemoryProperties;
        VertexBufferLayout& BufferLayout;
    };

    class VulkanVertexBuffer : public VertexBuffer
    {
    public:
        explicit VulkanVertexBuffer(const VulkanVertexBufferDesc& desc);
        ~VulkanVertexBuffer() override;

        void Bind(CommandBufferHandle commandBufferHandle) override;

        void* GetVerticeData() override { return m_VerticeData; }
        uint32 GetSize() override { return m_SizeInBytes; }
        VertexBufferLayout& GetBufferLayout() override { return m_BufferLayout; }

        void* GetNativeHande() override { return m_VertexBuffer.GetNativeHandle(); }

    private:

        void CreateVertexBuffer(const VulkanVertexBufferDesc& desc);

        VkDevice m_Device;
        VertexBufferLayout m_BufferLayout;

        VulkanBuffer m_VertexBuffer;
        uint32 m_SizeInBytes;
        void* m_VerticeData;
    };

}
