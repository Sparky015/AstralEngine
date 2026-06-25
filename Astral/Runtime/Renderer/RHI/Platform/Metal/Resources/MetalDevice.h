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

        /**
         * @brief Creates an empty render pass object
         * @return The created empty render pass object
         */
        RenderPassHandle CreateRenderPass() override;

        /**
         * @brief Creates a shader object from the shader source
         * @param shaderSource The source code of the shader
         * @return The shader object
         */
        ShaderHandle CreateShader(const ShaderSource& shaderSource) override;
        PipelineStateHandle CreateGraphicsPipelineState(const GraphicsPipelineStateCreateInfo& graphiscPipelineStateCreateInfo) override; // TODO
        PipelineStateHandle CreateComputePipelineState(const ComputePipelineStateCreateInfo& computePipelineStateCreateInfo) override; // TODO
        VertexBufferHandle CreateVertexBuffer(void* vertexData, uint32 sizeInBytes, VertexBufferLayout& bufferLayout, GPUMemoryType memoryType) override;
        IndexBufferHandle CreateIndexBuffer(uint32* indexData, uint32 sizeInBytes, GPUMemoryType memoryType) override;
        BufferHandle CreateStorageBuffer(void* data, uint32 size, GPUMemoryType memoryType) override;
        BufferHandle CreateUniformBuffer(void* data, uint32 size, GPUMemoryType memoryType) override;

        /**
         * @brief Creates an empty descriptor set object
         * @return The created empty descriptor set object
         */
        DescriptorSetHandle CreateDescriptorSet() override;

        /**
         * @brief Creates a 2D texture object
         * @param textureCreateInfo Info to create the texture with
         * @return The created texture object
         */
        TextureHandle CreateTexture(const TextureCreateInfo& textureCreateInfo) override;

        /**
         * @brief Creates a cubemap texture object
         * @param textureCreateInfo Info to create the cubemap texture with
         * @return The created cubemap texture object
         */
        TextureHandle CreateCubemap(const TextureCreateInfo& textureCreateInfo) override;

        /**
         * @brief Creates a 3D texture object
         * @param textureCreateInfo Info to create the 3D texture with
         * @return The created 3D texture object
         */
        TextureHandle Create3DTexture(const TextureCreateInfo& textureCreateInfo) override;

        /**
         * @brief Creates a 1D texture object
         * @param textureCreateInfo Info to create the 1D texture with
         * @return The created 1D texture object
         */
        TextureHandle Create1DTexture(const TextureCreateInfo& textureCreateInfo) override;

        /**
         * @brief Creates a 2D texture array object
         * @param textureCreateInfo Info to create the 2D texture array with
         * @return The created 2D texture array object
         */
        TextureHandle Create2DTextureArray(const TextureCreateInfo& textureCreateInfo) override;

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
