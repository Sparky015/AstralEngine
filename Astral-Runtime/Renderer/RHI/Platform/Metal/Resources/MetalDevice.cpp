/**
* @file MetalDevice.cpp
* @author Andrew Fagan
* @date 3/23/26
*/

#include "MetalDevice.h"

#include "Debug/Utilities/Asserts.h"
#include "MetalBuffer.h"
#include "MetalIndexBuffer.h"
#include "MetalSwapchain.h"
#include "MetalVertexBuffer.h"

namespace Astral {

    MetalDevice::MetalDevice(const MetalDeviceDesc& desc) :
        m_Device(nullptr),
        m_CAMetalLayer(desc.CAMetalLayer)
    {}


    MetalDevice::~MetalDevice()
    {
        DestroySwapchain();
        DestroyDevice();
    }


    void MetalDevice::Init()
    {
        CreateDevice();
        m_Swapchain = CreateSwapchain(3);
    }


    Swapchain& MetalDevice::GetSwapchain()
    {
        ASSERT(m_Swapchain, "Metal Swapchain has not been created! Cannot retrieve null instance!");
        return *m_Swapchain;
    }


    CommandBufferHandle MetalDevice::AllocateCommandBuffer()
    {
        return nullptr;
    }


    CommandQueueHandle MetalDevice::GetPrimaryCommandQueue()
    {
        return nullptr;
    }


    CommandQueueHandle MetalDevice::GetAsyncCommandQueue()
    {
        return nullptr;
    }


    RenderPassHandle MetalDevice::CreateRenderPass()
    {
        return nullptr;
    }


    FramebufferHandle MetalDevice::CreateFramebuffer(RenderPassHandle renderPassHandle)
    {
        return nullptr;
    }


    ShaderHandle MetalDevice::CreateShader(const ShaderSource& shaderSource)
    {
        return nullptr;
    }


    PipelineStateHandle MetalDevice::CreateGraphicsPipelineState(const GraphicsPipelineStateCreateInfo& graphiscPipelineStateCreateInfo)
    {
        return nullptr;
    }


    PipelineStateHandle MetalDevice::CreateComputePipelineState(const ComputePipelineStateCreateInfo& computePipelineStateCreateInfo)
    {
        return nullptr;
    }


    VertexBufferHandle MetalDevice::CreateVertexBuffer(void* vertexData, uint32 sizeInBytes, VertexBufferLayout& bufferLayout, GPUMemoryType memoryType)
    {
        MetalVertexBufferDesc metalVertexBufferDesc = {
            .Device = m_Device,
            .VertexData = vertexData,
            .DataSize = sizeInBytes,
            .BufferLayout = bufferLayout,
            .MemoryType = memoryType
        };

        return CreateGraphicsRef<MetalVertexBuffer>(metalVertexBufferDesc);
    }


    IndexBufferHandle MetalDevice::CreateIndexBuffer(uint32* indexData, uint32 sizeInBytes, GPUMemoryType memoryType)
    {
        MetalIndexBufferDesc metalIndexBufferDesc = {
            .Device = m_Device,
            .IndexData = indexData,
            .DataSize = sizeInBytes,
            .MemoryType = memoryType
        };

        return CreateGraphicsRef<MetalIndexBuffer>(metalIndexBufferDesc);
    }


    BufferHandle MetalDevice::CreateStorageBuffer(void* data, uint32 size, GPUMemoryType memoryType)
    {
        MetalBufferDesc metalStorageBufferDesc = {
            .Device = m_Device,
            .Size = size,
            .MemoryType = memoryType,
        };

        BufferHandle bufferHandle = CreateGraphicsRef<MetalBuffer>(metalStorageBufferDesc);
        bufferHandle->CopyDataToBuffer(data, size);
        return bufferHandle;
    }


    BufferHandle MetalDevice::CreateUniformBuffer(void* data, uint32 size, GPUMemoryType memoryType)
    {
        MetalBufferDesc metalUniformBufferDesc = {
            .Device = m_Device,
            .Size = size,
            .MemoryType = memoryType,
        };

        BufferHandle bufferHandle = CreateGraphicsRef<MetalBuffer>(metalUniformBufferDesc);
        bufferHandle->CopyDataToBuffer(data, size);
        return bufferHandle;
    }


    DescriptorSetHandle MetalDevice::CreateDescriptorSet()
    {
        return nullptr;
    }


    TextureHandle MetalDevice::CreateTexture(const TextureCreateInfo& textureCreateInfo)
    {
        return nullptr;
    }


    TextureHandle MetalDevice::CreateCubemap(const TextureCreateInfo& textureCreateInfo)
    {
        return nullptr;
    }


    TextureHandle MetalDevice::Create3DTexture(const TextureCreateInfo& textureCreateInfo)
    {
        return nullptr;
    }


    TextureHandle MetalDevice::Create1DTexture(const TextureCreateInfo& textureCreateInfo)
    {
        return nullptr;
    }


    TextureHandle MetalDevice::Create2DTextureArray(const TextureCreateInfo& textureCreateInfo)
    {
        return nullptr;
    }


    bool MetalDevice::IsBlitSupportedByFormat(ImageFormat imageFormat)
    {
        return false;
    }


    bool MetalDevice::IsAnisotropySupported()
    {
        return false;
    }


    float MetalDevice::GetMaxAnisotropySupported()
    {
        return 0;
    }


    std::string_view MetalDevice::GetRenderingAPI()
    {

    }


    std::string_view MetalDevice::GetGPUVendor()
    {

    }


    std::string_view MetalDevice::GetGraphicsProcessorName()
    {

    }


    void MetalDevice::WaitIdle()
    {

    }


    void* MetalDevice::GetNativeHandle()
    {
        return m_Device;
    }


    void MetalDevice::CreateDevice()
    {
        m_Device = MTL::CreateSystemDefaultDevice();
    }


    void MetalDevice::DestroyDevice()
    {
        if (m_Device)
        {
            m_Device->release();
        }
    }


    GraphicsOwnedPtr<Swapchain> MetalDevice::CreateSwapchain(uint32 numberOfImages)
    {
        MetalSwapchainDesc metalSwapchainDesc = {

        };

        return CreateGraphicsOwnedPtr<MetalSwapchain>(metalSwapchainDesc);
    }


    void MetalDevice::DestroySwapchain()
    {
        m_Swapchain.reset();
    }

}
