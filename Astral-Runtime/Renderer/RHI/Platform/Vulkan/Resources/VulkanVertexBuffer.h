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
    };

    class VulkanVertexBuffer : public VertexBuffer
    {
    public:
        explicit VulkanVertexBuffer(const VulkanVertexBufferDesc& desc);
        ~VulkanVertexBuffer() override;

        void Bind(CommandBufferHandle commandBufferHandle) override;

        void* GetVerticeData() override { return m_VerticeData; }
        uint32 GetSize() override { return m_SizeInBytes; }

        void* GetNativeHande() override { return m_VertexBuffer.GetNativeHandle(); }

        static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions()
        {
            return {{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}};
        }

        static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions()
        {
            std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = 0;

            return attributeDescriptions;
        }


    private:

        void CreateVertexBuffer(const VulkanVertexBufferDesc& desc);

        VkDevice m_Device;
        VulkanBuffer m_VertexBuffer;
        uint32 m_SizeInBytes;
        void* m_VerticeData;
    };

}
