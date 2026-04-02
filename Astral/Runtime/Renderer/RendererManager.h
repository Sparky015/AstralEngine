//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "RHI/RendererContext.h"
#include "RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    /**
     * @class RendererManager
     * @brief Manages the lifetime of the rendering context and the scene renderer
     */
    class RendererManager : public SystemManager
    {
    public:
        RendererManager();
        ~RendererManager() override;

        /**
         * @brief Initializes the rendering context and the scene renderer
         */
        void Init() override;

        /**
         * @brief Shuts down the scene renderer and the rendering context
         */
        void Shutdown() override;

        RendererManager(const RendererManager&) = delete;
        RendererManager& operator=(const RendererManager&) = delete;
        RendererManager(RendererManager&&) = delete;
        RendererManager& operator=(RendererManager&&) = delete;

        RenderingContext& GetContext();

    private:

        /**
         * @brief Loads commonly used renderer assets
         */
        void LoadRendererAssets();

        GraphicsOwnedPtr<RenderingContext> m_RenderContext;
    };

}
