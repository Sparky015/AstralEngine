/**
* @file MetalDevice.h
* @author Andrew Fagan
* @date 3/23/26
*/

#pragma once

#include "Renderer/RHI/Resources/Device.h"

#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace Astral {

    struct MetalDeviceDesc
    {
        CA::MetalLayer* CAMetalLayer;
    };

    class MetalDevice : public Device
    {
    public:
        explicit MetalDevice(const MetalDeviceDesc& desc); // TODO
        ~MetalDevice() override; // TODO

        void Init() override; // TODO

        Swapchain& GetSwapchain() override; // TODO
        CommandBufferHandle AllocateCommandBuffer() override; // TODO
        CommandQueueHandle GetPrimaryCommandQueue() override; // TODO
        CommandQueueHandle GetAsyncCommandQueue() override; // TODO
        RenderPassHandle CreateRenderPass() override; // TODO
        FramebufferHandle CreateFramebuffer(RenderPassHandle renderPassHandle) override; // TODO
        ShaderHandle CreateShader(const ShaderSource& shaderSource) override; // TODO
        PipelineStateHandle CreateGraphicsPipelineState(const GraphicsPipelineStateCreateInfo& graphiscPipelineStateCreateInfo) override; // TODO
        PipelineStateHandle CreateComputePipelineState(const ComputePipelineStateCreateInfo& computePipelineStateCreateInfo) override; // TODO
        VertexBufferHandle CreateVertexBuffer(void* vertexData, uint32 sizeInBytes, VertexBufferLayout& bufferLayout, GPUMemoryType memoryType) override;
        IndexBufferHandle CreateIndexBuffer(uint32* indexData, uint32 sizeInBytes, GPUMemoryType memoryType) override;
        BufferHandle CreateStorageBuffer(void* data, uint32 size) override;
        BufferHandle CreateUniformBuffer(void* data, uint32 size) override;
        DescriptorSetHandle CreateDescriptorSet() override; // TODO
        TextureHandle CreateTexture(const TextureCreateInfo& textureCreateInfo) override; // TODO
        TextureHandle CreateCubemap(const TextureCreateInfo& textureCreateInfo) override; // TODO
        TextureHandle Create3DTexture(const TextureCreateInfo& textureCreateInfo) override; // TODO
        TextureHandle Create1DTexture(const TextureCreateInfo& textureCreateInfo) override; // TODO
        TextureHandle Create2DTextureArray(const TextureCreateInfo& textureCreateInfo) override; // TODO

        bool IsBlitSupportedByFormat(ImageFormat imageFormat) override; // TODO
        bool IsAnisotropySupported() override; // TODO
        float GetMaxAnisotropySupported() override; // TODO

        std::string_view GetRenderingAPI() override; // TODO
        std::string_view GetGPUVendor() override; // TODO
        std::string_view GetGraphicsProcessorName() override; // TODO

        void WaitIdle() override; // TODO

        void* GetNativeHandle() override;

    private:

        /**
         * @brief Allocates the default system device
         */
        void CreateDevice();

        /**
         * @brief Releases the device
         */
        void DestroyDevice();

        /**
         * @brief Allocates a swapchain instance
         */
        GraphicsOwnedPtr<Swapchain> CreateSwapchain(uint32 numberOfImages) override; // TODO

        /**
         * @brief Destroys the swapchain instance
         */
        void DestroySwapchain();


        MTL::Device* m_Device;
        CA::MetalLayer* m_CAMetalLayer;
        GraphicsOwnedPtr<Swapchain> m_Swapchain;
    };

}
