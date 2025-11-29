//
// Created by Andrew Fagan on 11/29/24.
//

#pragma once

#include "SceneRendererImpl.h"

namespace Astral {

    /**
     * @class SceneRenderer
     * @brief Defines the static interface for the scene renderer
     */
    class SceneRenderer
    {
    public:

        /**
         * @brief Initializes the scene renderer
         */
        static void Init();

        /**
         * @brief Shuts down the scene renderer
         */
        static void Shutdown();

        /**
         * @brief Marks the beginning of submitting objects to render
         * @param sceneDescription The description of the scene to render
         * @post   @ref EndScene is called to close object submission
         */
        static void BeginScene(const SceneDescription& sceneDescription);

        /**
         * @brief Marks the end of scene object submission to renderer
         * @pre   @ref BeginScene is called first to initialize object submission
         */
        static void EndScene();

        /**
         * @brief Submits an object to the renderer
         * @param mesh The mesh of the object
         * @param material The material of the object
         * @param transform The model transform of the object
         */
        static void Submit(const Ref<Mesh>& mesh, const Ref<Material>& material, const Mat4& transform);

        /**
         * @brief Updates the renderer settings with the given renderer settings
         * @param rendererSettings The renderer settings to apply to the renderer
         */
        static void SetRendererSettings(const RendererSettings& rendererSettings);

        /**
         * @brief Gets the current renderer settings
         * @return The current renderer settings
         */
        static const RendererSettings& GetRendererSettings();

        /**
         * @brief Gets the viewport texture of the current frame
         * @return The viewport texture of the current frame
         */
        static DescriptorSetHandle GetViewportTexture();

        /**
         * @brief Resizes the viewport being rendered to
         * @param width Width to set viewport to
         * @param height Height to set viewport to
         */
        static void ResizeViewport(uint32 width, uint32 height);

        /**
         * @brief Gets the dimensions of the viewport
         * @return The dimensions of the viewport
         */
        static UVec2 GetViewportSize() { return m_Renderer->GetViewportSize(); }

        /**
         * @brief Gets debug stats for the renderer
         * @return The debug stats for the renderer
         */
        static RendererDebugStats GetRendererDebugStats();

        /**
         * @brief Gets the renderer path being used
         * @return The renderer path being used
         */
        static RendererType GetRendererType();

        /**
         * @brief Gets the backend rendering API being used
         * @return The backend rendering API being used
         */
        static API GetRendererAPIBackend();

        /**
         * @brief Sets the renderer path being used
         * @param rendererType The new renderer path to use
         */
        static void SetRendererType(RendererType rendererType);

    private:

        static GraphicsOwnedPtr<SceneRendererImpl> m_Renderer;
    };

} // Astral
