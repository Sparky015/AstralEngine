/**
* @file MetalDevice.cpp
* @author Andrew Fagan
* @date 3/23/26
*/

#include "MetalDevice.h"

#include "Core/Utilities/Asserts.h"
#include "MetalBuffer.h"
#include "MetalDescriptorSet.h"
#include "MetalIndexBuffer.h"
#include "MetalRenderPass.h"
#include "MetalShader.h"
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
        MetalRenderPassDesc renderPassDesc = {
            .Device = m_Device,
        };

        return CreateGraphicsRef<MetalRenderPass>(renderPassDesc);
    }


    FramebufferHandle MetalDevice::CreateFramebuffer(RenderPassHandle renderPassHandle)
    {
        return nullptr;
    }


    ShaderHandle MetalDevice::CreateShader(const ShaderSource& shaderSource)
    {
        MetalShaderDesc shaderDesc = {
            .Device = m_Device,
            .ShaderSource = shaderSource,
        };

        return CreateGraphicsRef<MetalShader>(shaderDesc);
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
        MetalDescriptorSetDesc metalDescriptorSetDesc
        {
            .Device = m_Device
        };

        return CreateGraphicsRef<MetalDescriptorSet>(metalDescriptorSetDesc);
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
        return true; // Universal available based on gpu family feature set for Metal 4;
    }


    float MetalDevice::GetMaxAnisotropySupported()
    {
        return 16; // Universal limit based on gpu family feature set for Metal 4;
    }


    std::string_view MetalDevice::GetRenderingAPI()
    {
        return "Metal 4";
    }


    std::string_view MetalDevice::GetGPUVendor()
    {
        return "Apple";
    }


    std::string_view MetalDevice::GetGraphicsProcessorName()
    {
        static char buffer[64];
        snprintf(buffer, sizeof(buffer), "%s", m_Device->name()->cString(NS::ASCIIStringEncoding));
        return buffer;
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

        if (!m_Device->supportsFamily(MTL::GPUFamilyMetal4))
        {
            AE_ERROR("This device does not support Metal 4!")
        }
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
            .CAMetalLayer = m_CAMetalLayer
        };

        return CreateGraphicsOwnedPtr<MetalSwapchain>(metalSwapchainDesc);
    }


    void MetalDevice::DestroySwapchain()
    {
        m_Swapchain.reset();
    }

}
