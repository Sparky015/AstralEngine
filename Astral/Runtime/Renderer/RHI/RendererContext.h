//
// Created by Andrew Fagan on 10/8/24.
//

#pragma once

#include "Common/GraphicsSmartPointers.h"
#include "Resources/Device.h"
#include "Resources/PipelineStateCache.h"

#include <string_view>

namespace Astral {

    /**
     * RendererContext is an interface to which each Rendering API will override and provide their own way to create
     * their rendering context.
     */
    class RenderingContext
    {
    public:
        virtual ~RenderingContext() = default;

        /**
         * @brief Initializes the graphics API rendering context
         */
        virtual void Init() = 0;

        /**
         * @brief Shuts down the graphics API rendering context
         */
        virtual void Shutdown() = 0;

        /**
         * @brief Gets the logical device
         * @return The logical device
         */
        virtual Device& GetDevice() = 0;

        /**
         * @brief Gets the pipeline state cache
         * @return The pipeline state cache
         */
        virtual PipelineStateCache& GetPipelineStateCache() = 0;

        /**
         * @brief Gets the number of validation errors and warnings triggered
         * @return The number of validation errors and warnings triggered
         */
        virtual uint32 GetNumValidationErrorsAndWarnings() = 0;

        /**
         * @brief Clears the counter of the number of validation errors and warnings triggered
         */
        virtual void ClearNumValidationErrorsAndWarnings() = 0;

        /**
         * @brief Gets the framebuffer size of the main application window
         */
        UVec2 GetWindowFramebufferDimensions();

        /**
         * @brief Gets the native instance handle of the graphics API context
         * @return The native instance handle of the graphics API context
         */
        virtual void* GetInstanceHandle() = 0;

        /**
         * @brief Initializes ImGui rendering backend
         */
        virtual void InitImGuiForAPIBackend(RenderPassHandle renderPassHandle) = 0;

        /**
         * @brief Initializes a new frame for ImGui backend
         */
        virtual void MarkNewImGuiFrame() = 0;

        /**
         * @brief Shuts down the ImGui rendering backend
         */
        virtual void ShutdownImGuiForAPIBackend() = 0;

        /**
         * @brief Creates a rendering context instance
         */
        static RenderingContext* CreateRendererContext();
    };

}
