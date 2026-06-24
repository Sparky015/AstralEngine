/**
* @file MetalRendererContext.h
* @author Andrew Fagan
* @date 3/23/26
*/

#pragma once

#include "Renderer/RHI/RendererContext.h"

#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <GLFW/glfw3.h>

#include "Core/Memory/Pools/ObjectPool.h"

namespace Astral {

    /**
     * @brief Holds components needed for the lifetime of the Metal renderer such as the device and window layer
     */
    class MetalRenderingContext : public RenderingContext
    {
    public:
        explicit MetalRenderingContext(GLFWwindow* window);
        ~MetalRenderingContext() override = default;

        /**
         * @brief Initializes the Metal rendering context
         */
        void Init() override;

        /**
         * @brief Shuts down the Metal rendering context
         */
        void Shutdown() override;

        /**
         * @brief Gets the graphics device
         * @return The graphics device
         */
        Device& GetDevice() override;

        /**
         * @brief Gets the pipeline state cache
         * @return The pipeline state cache
         */
        PipelineStateCache& GetPipelineStateCache() override;

        /**
         * @brief This does nothing for Metal specifically.
         * @warning This does nothing. Metal does not have a debug callback for validation warnings and errors.
         */
        uint32 GetNumValidationErrorsAndWarnings() override;

        /**
         * @brief This does nothing for Metal specifically.
         * @warning This does nothing. Metal does not have a debug callback for validation warnings and errors.
         */
        void ClearNumValidationErrorsAndWarnings() override;

        /**
         * @brief This returns nullptr for Metal specifically.
         * @warning This always returns nullptr. Metal does not have an Instance concept.
         */
        void* GetInstanceHandle() override { return nullptr; }

        /**
         * @brief Initializes ImGui rendering backend
         */
        void InitImGuiForAPIBackend(RenderPassHandle renderPassHandle) override; // TODO

        /**
         * @brief Shuts down the ImGui rendering backend
         */
        void ShutdownImGuiForAPIBackend() override; // TODO

        /**
         * @brief Gets the metal compiler
         */
        MTL4::Compiler* GetCompiler();

        /**
         * @brief Gets the command allocator for the executing thread
         */
        MTL4::CommandAllocator* AcquireThreadCommandAllocator();

        /**
         * @brief Releases the given command allocator back to the thread's command allocator pool
         * @param commandAllocator The command allocator to release back to the thread's command allocator pool
         */
        void ReleaseThreadCommandAllocator(MTL4::CommandAllocator* commandAllocator);

    private:

        /**
         * @brief Creates the MTLDevice
         */
        void CreateDevice();

        /**
         * @brief Destroys the MTLDevice
         */
        void DestroyDevice();

        /**
         * @brief Creates the CAMetalLayer
         */
        void AllocateCAMetalLayer();

        /**
         * @brief Releases the CAMetalLayer
         */
        void ReleaseCAMetalLayer();

        /**
         * @brief This creates the CAMetalLayer and attaches it to the window
         */
        void AttachCALayerToWindow();

        /**
         * @brief Creates the metal pipeline serializer
         */
        void CreatePipelineDataSetSerializer();

        /**
         * @brief Releases the metal pipeline serializer
         */
        void ReleasePipelineDataSetSerializer();

        /**
         * @brief Creates the metal compiler
         */
        void CreateCompiler();

        /**
         * @brief Releases the metal compiler
         */
        void ReleaseCompiler();

        /**
         * @brief Releases all stored command allocators from memory
         */
        void ReleaseAllCommandAllocatorPools();


        struct CommandAllocatorPool
        {
            std::unordered_set<MTL4::CommandAllocator*> AvailableCommandAllocators;
            std::unordered_set<MTL4::CommandAllocator*> UsedCommandAllocators;
        };


        GLFWwindow* m_Window;
        CA::MetalLayer* m_CAMetalLayer;
        GraphicsOwnedPtr<Device> m_Device;
        GraphicsOwnedPtr<PipelineStateCache> m_PipelineStateCache;

        MTL4::Compiler* m_Compiler;
        MTL4::PipelineDataSetSerializer* m_PipelineDataSetSerializer;

        std::unordered_map<std::thread::id, CommandAllocatorPool> m_CommandAllocators;
        std::mutex m_CommandAllocatorsMutex;
    };

}

