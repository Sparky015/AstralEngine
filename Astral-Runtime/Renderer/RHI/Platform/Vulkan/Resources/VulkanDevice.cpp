/**
* @file VulkanDevice.cpp
* @author Andrew Fagan
* @date 5/13/25
*/

#include "VulkanDevice.h"

#include "VulkanCommandBuffer.h"
#include "VulkanSwapchain.h"
#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Loggers.h"

#include <utility>

#include "VulkanCommandQueue.h"

namespace Astral {

    VulkanDevice::VulkanDevice(VulkanPhysicalDevice physicalDevice, uint32 queueFamilyIndex, VkSurfaceKHR windowSurface) :
            m_PhysicalDevice(std::move(physicalDevice)),
            m_Device(VK_NULL_HANDLE),
            m_QueueFamilyIndex(queueFamilyIndex),
            m_WindowSurface(windowSurface),
            m_CommandPool(VK_NULL_HANDLE)
    {
        CreateDevice();
        CreateCommandPool();
    }


    VulkanDevice::~VulkanDevice()
    {
        DestroyMemoryPool();
        DestroyDevice();
    }


    Swapchain* VulkanDevice::CreateSwapchain()
    {
        return new VulkanSwapchain(*this, m_PhysicalDevice, m_WindowSurface, m_QueueFamilyIndex);
    }


    CommandBuffer* VulkanDevice::AllocateCommandBuffer()
    {
        VkCommandBuffer commandBuffer;
        VkCommandBufferAllocateInfo commandBufferAllocate = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandPool = m_CommandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            .commandBufferCount = 1,
        };

        VkResult result = vkAllocateCommandBuffers(m_Device, &commandBufferAllocate, &commandBuffer);
        ASSERT(result == VK_SUCCESS, "Vulkan command buffer(s) failed to allocate!");

        return new VulkanCommandBuffer(m_Device, m_CommandPool, commandBuffer);
    }


    CommandQueue* VulkanDevice::GetCommandQueue()
    {
        return new VulkanCommandQueue(m_Device, m_QueueFamilyIndex, 0);
    }


    void VulkanDevice::CreateDevice()
    {
        float priorities[] = { 1.0f };

        VkDeviceQueueCreateInfo deviceQueueCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = m_QueueFamilyIndex,
            .queueCount = 1,
            .pQueuePriorities = &priorities[0]
        };

        std::vector<const char*> devExts = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME
        };

        VkPhysicalDeviceFeatures deviceFeatures = { 0 };

        if (m_PhysicalDevice.features.geometryShader == VK_FALSE)
        { LOG("Vulkan: Geometry Shader is not supported!") }
        else
        { deviceFeatures.geometryShader = VK_TRUE; }

        if (m_PhysicalDevice.features.tessellationShader == VK_FALSE)
        { LOG("Vulkan: Tessellation Shader is not supported!") }
        else
        { deviceFeatures.tessellationShader = VK_TRUE; }

        VkDeviceCreateInfo deviceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &deviceQueueCreateInfo,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = (uint32)devExts.size(),
            .ppEnabledExtensionNames = devExts.data(),
            .pEnabledFeatures = &deviceFeatures
        };

        VkResult result = vkCreateDevice(m_PhysicalDevice.physicalDevice, &deviceCreateInfo, nullptr, &m_Device);
        ASSERT(result == VK_SUCCESS, "Vulkan device failed on creation!");
    }


    void VulkanDevice::DestroyDevice()
    {
        vkDestroyDevice(m_Device, nullptr);
    }


    void VulkanDevice::CreateCommandPool()
    {
        VkCommandPoolCreateInfo commandPoolCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = m_QueueFamilyIndex
        };

        VkResult result = vkCreateCommandPool(m_Device, &commandPoolCreateInfo, nullptr, &m_CommandPool);
        ASSERT(result == VK_SUCCESS, "Vulkan command pool failed to create!");
    }


    void VulkanDevice::DestroyMemoryPool()
    {
        vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);
    }

}
