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


        GLFWwindow* m_Window;
        CA::MetalLayer* m_CAMetalLayer;
        GraphicsOwnedPtr<Device> m_Device;
        GraphicsOwnedPtr<PipelineStateCache> m_PipelineStateCache;
    };

}

