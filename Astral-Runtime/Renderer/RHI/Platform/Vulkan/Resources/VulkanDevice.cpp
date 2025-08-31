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
#include "VulkanPipelineState.h"
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
    }


    VulkanDevice::~VulkanDevice()
    {
        DestroySwapchain();
        DestroyMemoryPool();
        DestroyDevice();
    }


    void VulkanDevice::Init()
    {
        CreateDevice();
        CreateCommandPool();
        m_Swapchain = VulkanDevice::CreateSwapchain(3);
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


    GraphicsRef<CommandQueue> VulkanDevice::GetPrimaryCommandQueue()
    {
        VulkanCommandQueueDesc commandQueueDesc = {
            .Device = m_Device,
            .Swapchain = *m_Swapchain,
            .QueueFamilyIndex = m_QueueFamilyIndex,
            .QueueIndex = 0
        };

        return CreateGraphicsRef<VulkanCommandQueue>(commandQueueDesc);
    }


    CommandQueueHandle VulkanDevice::GetAsyncCommandQueue()
    {
        uint32 asyncQueueIndex = 1;
        while (m_PhysicalDevice.queueFamilyProperties[m_QueueFamilyIndex].queueCount <= asyncQueueIndex) { asyncQueueIndex--; }

        VulkanCommandQueueDesc commandQueueDesc = {
            .Device = m_Device,
            .Swapchain = *m_Swapchain,
            .QueueFamilyIndex = m_QueueFamilyIndex,
            .QueueIndex = asyncQueueIndex
        };
        return CreateGraphicsRef<VulkanCommandQueue>(commandQueueDesc);
    }


    RenderPassHandle VulkanDevice::CreateRenderPass()
    {
        VulkanRenderpassDesc vulkanRenderpassDesc = {
            .Device = m_Device,
        };

        return CreateGraphicsRef<VulkanRenderPass>(vulkanRenderpassDesc);
    }


    FramebufferHandle VulkanDevice::CreateFramebuffer(RenderPassHandle renderPassHandle)
    {
        VkRenderPass renderPass = (VkRenderPass)renderPassHandle->GetNativeHandle();

        VulkanFramebufferDesc vulkanFramebufferDesc = {
            .Device = m_Device,
            .RenderPass = renderPass
        };

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


    PipelineStateHandle VulkanDevice::CreateGraphicsPipelineState(const GraphicsPipelineStateCreateInfo& pipelineStateCreateInfo)
    {
        VulkanGraphicsPipelineStateDesc pipelineStateObjectDesc = {
            .Device = m_Device,
            .RenderPass = pipelineStateCreateInfo.RenderPass,
            .VertexShader = pipelineStateCreateInfo.VertexShader,
            .FragmentShader = pipelineStateCreateInfo.FragmentShader,
            .DescriptorSets = pipelineStateCreateInfo.DescriptorSets,
            .VertexBufferLayout = pipelineStateCreateInfo.BufferLayout,
            .SubpassIndex = pipelineStateCreateInfo.SubpassIndex,
            .IsAlphaBlended = pipelineStateCreateInfo.IsAlphaBlended
        };

        glfwGetFramebufferSize(m_Window, &pipelineStateObjectDesc.WindowWidth, &pipelineStateObjectDesc.WindowHeight);

        return CreateGraphicsRef<VulkanPipelineState>(pipelineStateObjectDesc);
    }


    PipelineStateHandle VulkanDevice::CreateComputePipelineState(const ComputePipelineStateCreateInfo& computePipelineStateCreateInfo)
    {
        VulkanComputePipelineStateDesc pipelineStateObjectDesc = {
            .Device = m_Device,
            .ComputeShader = computePipelineStateCreateInfo.ComputeShader,
            .DescriptorSets = computePipelineStateCreateInfo.DescriptorSets,
        };

        return CreateGraphicsRef<VulkanPipelineState>(pipelineStateObjectDesc);
    }


    VertexBufferHandle VulkanDevice::CreateVertexBuffer(void* verticeData, uint32 sizeInBytes, VertexBufferLayout& bufferLayout)
    {
        VulkanVertexBufferDesc vertexBufferDesc = {
            .VulkanDevice = *this,
            .Device = m_Device,
            .VerticeData = verticeData,
            .SizeInBytes = sizeInBytes,
            .DeviceMemoryProperties = m_PhysicalDevice.memoryProperties,
            .BufferLayout = bufferLayout
        };

        return CreateGraphicsRef<VulkanVertexBuffer>(vertexBufferDesc);
    }


    IndexBufferHandle VulkanDevice::CreateIndexBuffer(uint32* indiceData, uint32 sizeInBytes)
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


    TextureHandle VulkanDevice::CreateTexture(const TextureCreateInfo& textureCreateInfo)
    {
        VulkanTextureDesc textureDesc = {
            .VulkanDevice = this,
            .Device = m_Device,
            .PhysicalDeviceMemoryProperties = m_PhysicalDevice.memoryProperties,
            .ImageData = textureCreateInfo.ImageData,
            .ImageDataLength = textureCreateInfo.ImageDataLength,
            .ImageFormat = textureCreateInfo.Format,
            .ImageLayout = textureCreateInfo.Layout,
            .ImageUsageFlags = textureCreateInfo.UsageFlags,
            .ImageWidth = textureCreateInfo.Dimensions.x,
            .ImageHeight = textureCreateInfo.Dimensions.y,
            .NumLayers = textureCreateInfo.LayerCount > 0 ? textureCreateInfo.LayerCount : 1,
            .NumMipLevels = textureCreateInfo.MipMapCount > 0 ? textureCreateInfo.MipMapCount : 1,
            .GenerateMipMaps = textureCreateInfo.GenerateMipMaps,
            .TextureType = TextureType::IMAGE_2D
        };

        return CreateGraphicsRef<VulkanTexture>(textureDesc);
    }


    TextureHandle VulkanDevice::CreateCubemap(const TextureCreateInfo& textureCreateInfo)
    {
        VulkanTextureDesc textureDesc = {
            .VulkanDevice = this,
            .Device = m_Device,
            .PhysicalDeviceMemoryProperties = m_PhysicalDevice.memoryProperties,
            .ImageData = textureCreateInfo.ImageData,
            .ImageDataLength = textureCreateInfo.ImageDataLength,
            .ImageFormat = textureCreateInfo.Format,
            .ImageLayout = textureCreateInfo.Layout,
            .ImageUsageFlags = textureCreateInfo.UsageFlags,
            .ImageWidth = textureCreateInfo.Dimensions.x,
            .ImageHeight = textureCreateInfo.Dimensions.y,
            .NumLayers = 6,
            .NumMipLevels = textureCreateInfo.MipMapCount > 0 ? textureCreateInfo.MipMapCount : 1,
            .GenerateMipMaps = textureCreateInfo.GenerateMipMaps,
            .TextureType = TextureType::CUBEMAP
        };

        return CreateGraphicsRef<VulkanTexture>(textureDesc);
    }


    TextureHandle VulkanDevice::Create3DTexture(const TextureCreateInfo& textureCreateInfo)
    {
        VulkanTextureDesc textureDesc = {
            .VulkanDevice = this,
            .Device = m_Device,
            .PhysicalDeviceMemoryProperties = m_PhysicalDevice.memoryProperties,
            .ImageData = textureCreateInfo.ImageData,
            .ImageDataLength = textureCreateInfo.ImageDataLength,
            .ImageFormat = textureCreateInfo.Format,
            .ImageLayout = textureCreateInfo.Layout,
            .ImageUsageFlags = textureCreateInfo.UsageFlags,
            .ImageWidth = textureCreateInfo.Dimensions.x,
            .ImageHeight = textureCreateInfo.Dimensions.y,
            .NumLayers = textureCreateInfo.LayerCount > 0 ? textureCreateInfo.LayerCount : 1,
            .NumMipLevels = textureCreateInfo.MipMapCount > 0 ? textureCreateInfo.MipMapCount : 1,
            .GenerateMipMaps = textureCreateInfo.GenerateMipMaps,
            .TextureType = TextureType::IMAGE_3D
        };

        return CreateGraphicsRef<VulkanTexture>(textureDesc);
    }


    bool VulkanDevice::IsBlitSupportedByFormat(ImageFormat imageFormat)
    {
        VkFormat format = ConvertImageFormatToVkFormat(imageFormat);
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(m_PhysicalDevice.physicalDevice, format, &formatProperties);

        return (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_SRC_BIT) &&
               (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT);
    }


    bool VulkanDevice::IsAnisotropySupported()
    {
        VkPhysicalDeviceFeatures physicalDeviceFeatures;
        vkGetPhysicalDeviceFeatures(m_PhysicalDevice.physicalDevice, &physicalDeviceFeatures);
        return physicalDeviceFeatures.samplerAnisotropy;
    }


    float VulkanDevice::GetMaxAnisotropySupported()
    {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(m_PhysicalDevice.physicalDevice, &physicalDeviceProperties);
        return physicalDeviceProperties.limits.maxSamplerAnisotropy;
    }


    void VulkanDevice::WaitIdle()
    {
        vkDeviceWaitIdle(m_Device);
    }


    void VulkanDevice::CreateDevice()
    {
        float priorities[] = { 1.0f, 0.5f };

        uint32 queueCount = m_PhysicalDevice.queueFamilyProperties[m_QueueFamilyIndex].queueCount;
        LOG("Queue family has " << queueCount << " queue(s)!")

        VkDeviceQueueCreateInfo deviceQueueCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = m_QueueFamilyIndex,
            .queueCount = queueCount >= 2 ? 2u : 1u,
            .pQueuePriorities = priorities
        };

        std::vector<const char*> devExts = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME
        };

        VkPhysicalDeviceFeatures deviceFeatures = { 0 };

        if (m_PhysicalDevice.features.geometryShader == VK_FALSE)
        {
            LOG("Vulkan: Geometry Shader is not supported!")
        }
        else
        {
            deviceFeatures.geometryShader = VK_TRUE;
        }

        if (m_PhysicalDevice.features.tessellationShader == VK_FALSE)
        {
            LOG("Vulkan: Tessellation Shader is not supported!")
        }
        else
        {
            deviceFeatures.tessellationShader = VK_TRUE;
        }

        if (m_PhysicalDevice.features.samplerAnisotropy == VK_FALSE)
        {
            LOG("Vulkan: Sampler Anisotropy is not supported!")
        }
        else
        {
            deviceFeatures.samplerAnisotropy = VK_TRUE;
        }

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
