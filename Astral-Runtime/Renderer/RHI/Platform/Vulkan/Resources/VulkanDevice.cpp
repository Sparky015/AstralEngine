/**
* @file VulkanDevice.cpp
* @author Andrew Fagan
* @date 5/13/25
*/

#include "VulkanDevice.h"

#include "Debug/Utilities/Asserts.h"
#include "Debug/Utilities/Loggers.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSwapchain.h"
#include "VulkanCommandQueue.h"
#include "VulkanRenderpass.h"
#include "VulkanDescriptorSet.h"
#include "VulkanFramebuffer.h"
#include "VulkanPipelineStateObject.h"
#include "VulkanShader.h"
#include "VulkanVertexBuffer.h"

#include <GLFW/glfw3.h>
#include <utility>

#include "VulkanIndexBuffer.h"
#include "VulkanTexture.h"

namespace Astral {

    VulkanDevice::VulkanDevice(const VulkanDeviceDesc& desc) :
            m_PhysicalDevice(std::move(desc.PhysicalDevice)),
            m_QueueFamilyIndex(desc.QueueFamilyIndex),
            m_WindowSurface(desc.WindowSurface),
            m_Window(desc.Window),
            m_Device(VK_NULL_HANDLE),
            m_CommandPool(VK_NULL_HANDLE),
            m_Swapchain(nullptr)
    {
        CreateDevice();
        CreateCommandPool();
        m_Swapchain = VulkanDevice::CreateSwapchain(3);
    }


    VulkanDevice::~VulkanDevice()
    {
        DestroySwapchain();
        DestroyMemoryPool();
        DestroyDevice();
    }


    GraphicsOwnedPtr<Swapchain> VulkanDevice::CreateSwapchain(uint32 numberOfImages)
    {
        VulkanSwapchainDesc vulkanSwapchainDesc = {
            .Device = m_Device,
            .PhysicalDevice = m_PhysicalDevice,
            .WindowSurface = m_WindowSurface,
            .QueueFamilyIndex = m_QueueFamilyIndex,
            .NumberOfSwapchainImages = numberOfImages,
        };

        return CreateGraphicsOwnedPtr<VulkanSwapchain>(vulkanSwapchainDesc);
    }


    void VulkanDevice::DestroySwapchain()
    {
        m_Swapchain.reset();
    }


    GraphicsRef<CommandBuffer> VulkanDevice::AllocateCommandBuffer()
    {
        VulkanCommandBufferDesc vulkanCommandBufferDesc = {
            .Device = m_Device,
            .CommandPool = m_CommandPool
        };

        return CreateGraphicsRef<VulkanCommandBuffer>(vulkanCommandBufferDesc);
    }


    GraphicsRef<CommandQueue> VulkanDevice::GetCommandQueue()
    {
        VulkanCommandQueueDesc commandQueueDesc = {
            .Device = m_Device,
            .Swapchain = (VkSwapchainKHR)m_Swapchain->GetNativeHandle(),
            .QueueFamilyIndex = m_QueueFamilyIndex,
            .QueueIndex = 0
        };

        return CreateGraphicsRef<VulkanCommandQueue>(commandQueueDesc);
    }


    RenderPassHandle VulkanDevice::CreateRenderPass(RenderTargetHandle renderTargetHandle)
    {
        VkFormat format = *(VkFormat*)renderTargetHandle->GetImageFormat();
        VulkanRenderpassDesc vulkanRenderpassDesc = {
            .Device = m_Device,
            .Format = format,
        };

        glfwGetFramebufferSize(m_Window, &vulkanRenderpassDesc.WindowWidth, &vulkanRenderpassDesc.WindowHeight);

        return CreateGraphicsRef<VulkanRenderPass>(vulkanRenderpassDesc);
    }


    FramebufferHandle VulkanDevice::CreateFramebuffer(RenderPassHandle renderPassHandle, RenderTargetHandle renderTargetHandle)
    {
        VkRenderPass renderPass = (VkRenderPass)renderPassHandle->GetNativeHandle();
        VkImageView imageView = (VkImageView)renderTargetHandle->GetImageView();

        VulkanFramebufferDesc vulkanFramebufferDesc = {
            .Device = m_Device,
            .RenderPass = renderPass,
            .ImageView = imageView,
        };

        glfwGetFramebufferSize(m_Window, &vulkanFramebufferDesc.WindowWidth, &vulkanFramebufferDesc.WindowHeight);

        return CreateGraphicsRef<VulkanFramebuffer>(vulkanFramebufferDesc);
    }


    ShaderHandle VulkanDevice::CreateShader(const ShaderSource& shaderSource)
    {
        VulkanShaderDesc shaderDesc = {
            .Device = m_Device,
            .ShaderSource = shaderSource,
        };

        return CreateGraphicsRef<VulkanShader>(shaderDesc);
    }


    PipelineStateObjectHandle VulkanDevice::CreatePipelineStateObject(RenderPassHandle renderPassHandle,
                                                                      ShaderHandle vertexShader, ShaderHandle fragmentShader, DescriptorSetHandle descriptorSetHandle)
    {
        VkRenderPass renderPass = (VkRenderPass)renderPassHandle->GetNativeHandle();

        VulkanPipelineStateObjectDesc pipelineStateObjectDesc = {
            .Device = m_Device,
            .RenderPass = renderPass,
            .VertexShader = vertexShader,
            .FragmentShader = fragmentShader,
            .DescriptorSet = descriptorSetHandle,
        };

        glfwGetFramebufferSize(m_Window, &pipelineStateObjectDesc.WindowWidth, &pipelineStateObjectDesc.WindowHeight);

        return CreateGraphicsRef<VulkanPipelineStateObject>(pipelineStateObjectDesc);
    }


    VertexBufferHandle VulkanDevice::CreateVertexBuffer(void* verticeData, uint32 sizeInBytes)
    {
        VulkanVertexBufferDesc vertexBufferDesc = {
            .VulkanDevice = *this,
            .Device = m_Device,
            .VerticeData = verticeData,
            .SizeInBytes = sizeInBytes,
            .DeviceMemoryProperties = m_PhysicalDevice.memoryProperties
        };

        return CreateGraphicsRef<VulkanVertexBuffer>(vertexBufferDesc);
    }


    IndexBufferHandle VulkanDevice::CreateIndexBuffer(void* indiceData, uint32 sizeInBytes)
    {
        VulkanIndexBufferDesc indexBufferDesc = {
            .VulkanDevice = *this,
            .Device = m_Device,
            .DeviceMemoryProperties = m_PhysicalDevice.memoryProperties,
            .IndiceData = indiceData,
            .SizeInBytes = sizeInBytes
        };

        return CreateGraphicsRef<VulkanIndexBuffer>(indexBufferDesc);
    }


    BufferHandle VulkanDevice::CreateStorageBuffer(void* data, uint32 size)
    {
        VulkanBufferDesc storageBufferDesc = {
            .Device = m_Device,
            .Usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
            .Size = size,
            .DeviceMemoryProperties = m_PhysicalDevice.memoryProperties,
            .RequestedMemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        BufferHandle bufferHandle = CreateGraphicsRef<VulkanBuffer>(storageBufferDesc);
        bufferHandle->CopyDataToBuffer(data, size);
        return bufferHandle;
    }


    BufferHandle VulkanDevice::CreateUniformBuffer(void* data, uint32 size)
    {
        VulkanBufferDesc storageBufferDesc = {
            .Device = m_Device,
            .Usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            .Size = size,
            .DeviceMemoryProperties = m_PhysicalDevice.memoryProperties,
            .RequestedMemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        };

        BufferHandle bufferHandle = CreateGraphicsRef<VulkanBuffer>(storageBufferDesc);
        bufferHandle->CopyDataToBuffer(data, size);
        return bufferHandle;
    }


    DescriptorSetHandle VulkanDevice::CreateDescriptorSet()
    {
        VulkanDescriptorSetDesc descriptorSetDesc = {
            .Device = m_Device
        };

        return CreateGraphicsRef<VulkanDescriptorSet>(descriptorSetDesc);
    }


    TextureHandle VulkanDevice::CreateTexture(unsigned char* imageData)
    {
        VulkanTextureDesc textureDesc = {
            .Device = m_Device,
            .ImageData = imageData,
            .ImageFormat = VK_FORMAT_R8G8B8A8_SRGB,
        };

        return CreateGraphicsRef<VulkanTexture>(textureDesc);
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
            .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
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
