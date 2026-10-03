/**
* @file D3D12Device.h
* @author Andrew Fagan
* @date 3/24/2026
*/

#pragma once

#include "Renderer/RHI/Resources/Device.h"



namespace Astral {

    struct D3D12DeviceDesc
    {

    };

    class D3D12Device : public Device
    {
    public:
        explicit D3D12Device(const D3D12DeviceDesc& desc); // TODO
        ~D3D12Device() override; // TODO

        void Init() override; // TODO

        Swapchain& GetSwapchain() override; // TODO
        CommandBufferHandle AllocateCommandBuffer() override; // TODO
        CommandQueueHandle GetPrimaryCommandQueue() override; // TODO
        CommandQueueHandle GetAsyncCommandQueue() override; // TODO
        RenderPassHandle CreateRenderPass() override; // TODO
        ShaderHandle CreateShader(const ShaderSource& shaderSource) override; // TODO
        PipelineStateHandle CreateGraphicsPipelineState(const GraphicsPipelineStateCreateInfo& graphicsPipelineStateCreateInfo) override; // TODO
        PipelineStateHandle CreateComputePipelineState(const ComputePipelineStateCreateInfo& computePipelineStateCreateInfo) override; // TODO
        VertexBufferHandle CreateVertexBuffer(void* verticeData, uint32 sizeInBytes, VertexBufferLayout& bufferLayout, GPUMemoryType memoryType) override; // TODO
        IndexBufferHandle CreateIndexBuffer(uint32* indices, uint32 sizeInBytes, GPUMemoryType memoryType) override; // TODO
        BufferHandle CreateStorageBuffer(void* data, uint32 size, GPUMemoryType memoryType) override;
        BufferHandle CreateUniformBuffer(void* data, uint32 size, GPUMemoryType memoryType) override;
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
         * @brief
         */
        void CreateDevice(); // TODO

        /**
         * @brief
         */
        void DestroyDevice();  // TODO

        /**
         * @brief Allocates a swapchain instance
         */
        GraphicsOwnedPtr<Swapchain> CreateSwapchain(uint32 numberOfImages) override; // TODO

        /**
         * @brief Destroys the swapchain instance
         */
        void DestroySwapchain();


        GraphicsOwnedPtr<Swapchain> m_Swapchain;
    };

}
