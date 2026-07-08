/**
* @file MetalRendererContext.h
* @author Andrew Fagan
* @date 3/23/26
*/

#pragma once

#include "Renderer/RHI/RendererContext.h"
#include "Core/Events/EventListener.h"

#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>
#include <GLFW/glfw3.h>

#define IMGUI_IMPL_METAL_CPP
#include "Common/MTLEnumConversions.h"
#include "Debug/ImGui/ImGuiDependencies/imgui_impl_metal.h"

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
        void InitImGuiForAPIBackend(RenderPassHandle renderPassHandle) override;

        /**
         * @brief Initializes a new frame for ImGui backend
         */
        void MarkNewImGuiFrame() override;

        /**
         * @brief Shuts down the ImGui rendering backend
         */
        void ShutdownImGuiForAPIBackend() override;

        /**
         * @brief Gets the primary command queue
         */
        CommandQueueHandle GetPrimaryCommandQueue();

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

        /**
         * @brief Gets the global residency set for all resources
         */
        MTL::ResidencySet* GetGlobalResidencySet();

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
         * @brief Creates the primary command queue RHI handle
         */
        void CreatePrimaryCommandQueue();

        /**
         * @brief Releases the primary command queue RHI handle
         */
        void DestroyPrimaryCommandQueue();

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

        /**
         * @brief Creates a global residency set for all metal resources created by engine
         */
        void CreateGlobalResidencySet();

        /**
         * @brief Frees the global residency set
         */
        void ReleaseGlobalResidencySet();

        /**
         * @brief Drains the current frame autorelease pool and then creates a new autorelease pool for the next frame
         */
        void DrainFrameAutoreleasePool();


        struct CommandAllocatorPool
        {
            std::unordered_set<MTL4::CommandAllocator*> AvailableCommandAllocators;
            std::unordered_set<MTL4::CommandAllocator*> UsedCommandAllocators;
        };


        GLFWwindow* m_Window;
        CA::MetalLayer* m_CAMetalLayer;
        GraphicsOwnedPtr<Device> m_Device;
        CommandQueueHandle m_PrimaryCommandQueue;

        GraphicsOwnedPtr<PipelineStateCache> m_PipelineStateCache;
        MTL4::Compiler* m_Compiler;
        MTL4::PipelineDataSetSerializer* m_PipelineDataSetSerializer;

        std::unordered_map<std::thread::id, CommandAllocatorPool> m_CommandAllocators;
        std::mutex m_CommandAllocatorsMutex;

        MTL::ResidencySet* m_GlobalResidencySet;
        NS::AutoreleasePool* m_FrameAutoreleasePool;
        EventListener<NewFrameEvent> m_NewFrameListener;

        AttachmentFormats m_ImGuiRenderPassAttachmentFormats;
    };

}
