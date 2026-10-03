/**
* @file Device.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "CommandQueue.h"
#include "IndexBuffer.h"
#include "PipelineState.h"
#include "Renderpass.h"
#include "Shader.h"
#include "Swapchain.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "Renderer/RHI/Resources/CommandBuffer.h"

namespace Astral {

    /**
     * @brief A wrapper around the graphics API device object
     */
    class Device
    {
    public:
        virtual ~Device() = default;

        /**
         * @brief Initializes the Metal device
         */
        virtual void Init() = 0;

        /**
         * @brief Gets the swapchain instance
         */
        virtual Swapchain& GetSwapchain() = 0;

        /**
         * @brief Allocates a command buffer from the metal rendering context
         */
        virtual CommandBufferHandle AllocateCommandBuffer() = 0;

        /**
         * @brief Gets the primary command queue from the metal rendering context
         */
        virtual CommandQueueHandle GetPrimaryCommandQueue() = 0;

        /**
         * @brief Gets a dedicated command queue for asynchronous operations
         */
        virtual CommandQueueHandle GetAsyncCommandQueue() = 0;

        /**
         * @brief Creates an empty render pass object
         * @return The created empty render pass object
         */
        virtual RenderPassHandle CreateRenderPass() = 0;

        /**
         * @brief Creates a shader object from the shader source
         * @param shaderSource The source code of the shader
         * @return The shader object
         */
        virtual ShaderHandle CreateShader(const ShaderSource& shaderSource) = 0;

        /**
         * @brief Creates a graphics pipeline state
         * @param graphiscPipelineStateCreateInfo A description of how to create the graphics pipeline
         * @return A graphics pipeline state handle
         */
        virtual PipelineStateHandle CreateGraphicsPipelineState(const GraphicsPipelineStateCreateInfo& graphiscPipelineStateCreateInfo) = 0;

        /**
         * @brief Creates a compute pipeline state
         * @param computePipelineStateCreateInfo A description of how to create the compute pipeline
         * @return A compute pipeline state handle
         */
        virtual PipelineStateHandle CreateComputePipelineState(const ComputePipelineStateCreateInfo& computePipelineStateCreateInfo) = 0;

        /**
         * @brief Creates a vertex buffer
         * @param verticeData The vertex data to populate the buffer with (or nullptr if the buffer should not be populated with data)
         * @param sizeInBytes The length of the buffer to create
         * @param bufferLayout The vertex attribute layout of the buffer
         * @param memoryType The GPU memory type to create the buffer with
         * @return A handle to the created vertex buffer
         */
        virtual VertexBufferHandle CreateVertexBuffer(void* verticeData, uint32 sizeInBytes, VertexBufferLayout& bufferLayout, GPUMemoryType memoryType) = 0;

        /**
         * @brief Creates an index buffer
         * @param indices The index data to populate the buffer with (or nullptr if the buffer should not be populated with data)
         * @param sizeInBytes The length of the buffer to create
         * @param memoryType The GPU memory type to create the buffer with
         * @return A handle to the created index buffer
         */
        virtual IndexBufferHandle CreateIndexBuffer(uint32* indices, uint32 sizeInBytes, GPUMemoryType memoryType) = 0;

        /**
         * @brief Creates a storage buffer
         * @param data The index data to populate the buffer with (or nullptr if the buffer should not be populated with data)
         * @param size The length of the buffer to create
         * @param memoryType The GPU memory type to create the buffer with
         * @return A handle to the created index buffer
         */
        virtual BufferHandle CreateStorageBuffer(void* data, uint32 size, GPUMemoryType memoryType) = 0;

        /**
         * @brief Creates a uniform buffer
         * @param data The uniform data to populate the buffer with (or nullptr if the buffer should not be populated with data)
         * @param size The length of the buffer to create
         * @param memoryType The GPU memory type to create the buffer with
         * @return A handle to the created uniform buffer
         */
        virtual BufferHandle CreateUniformBuffer(void* data, uint32 size, GPUMemoryType memoryType) = 0;

        /**
         * @brief Creates an empty descriptor set object
         * @return The created empty descriptor set object
         */
        virtual DescriptorSetHandle CreateDescriptorSet() = 0;

        /**
         * @brief Creates a 2D texture object
         * @param textureCreateInfo Info to create the texture with
         * @return The created texture object
         */
        virtual TextureHandle CreateTexture(const TextureCreateInfo& textureCreateInfo) = 0;

        /**
         * @brief Creates a cubemap texture object
         * @param textureCreateInfo Info to create the cubemap texture with
         * @return The created cubemap texture object
         */
        virtual TextureHandle CreateCubemap(const TextureCreateInfo& textureCreateInfo) = 0;

        /**
         * @brief Creates a 3D texture object
         * @param textureCreateInfo Info to create the 3D texture with
         * @return The created 3D texture object
         */
        virtual TextureHandle Create3DTexture(const TextureCreateInfo& textureCreateInfo) = 0;

        /**
         * @brief Creates a 1D texture object
         * @param textureCreateInfo Info to create the 1D texture with
         * @return The created 1D texture object
         */
        virtual TextureHandle Create1DTexture(const TextureCreateInfo& textureCreateInfo) = 0;

        /**
         * @brief Creates a 2D texture array object
         * @param textureCreateInfo Info to create the 2D texture array with
         * @return The created 2D texture array object
         */
        virtual TextureHandle Create2DTextureArray(const TextureCreateInfo& textureCreateInfo) = 0;

        /**
         * @brief Checks if anisotropy is supported by the GPU device
         * @return True if anisotropy is supported by the GPU device, false otherwise
         */
        virtual bool IsBlitSupportedByFormat(ImageFormat imageFormat) = 0;

        /**
         * @brief Checks if anisotropy is supported by the GPU device
         * @return True if anisotropy is supported by the GPU device, false otherwise
         */
        virtual bool IsAnisotropySupported() = 0;

        /**
         * @brief Gets the max anisotropy supported by the GPU device
         * @return The max anisotropy supported by the GPU device
         */
        virtual float GetMaxAnisotropySupported() = 0;

        /**
         * @brief Gets the rendering API name and version
         * @return The rendering API name and version
         */
        virtual std::string_view GetRenderingAPI() = 0;

        /**
         * @brief Gets the GPU's vendor name
         * @return The GPU's vendor name
         */
        virtual std::string_view GetGPUVendor() = 0;

        /**
         * @brief Gets the graphics processor name
         * @return The graphics processor name
         */
        virtual std::string_view GetGraphicsProcessorName() = 0;

        /**
         * @brief Blocks the current CPU thread until the GPU finishes all of its work
         */
        virtual void WaitIdle() = 0;

        /**
         * @brief Gets the native graphics API handle of the device
         * @return The native graphics API handle of the device (MTL::Device*)
         */
        virtual void* GetNativeHandle() = 0;

    protected:

        virtual GraphicsOwnedPtr<Swapchain> CreateSwapchain(uint32 numberOfImages) = 0;
    };

}
