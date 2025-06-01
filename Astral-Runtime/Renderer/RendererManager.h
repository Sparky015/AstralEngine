//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "RHI/RendererContext.h"
#include "RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class RendererManager : public SystemManager
    {
    public:
        RendererManager();
        ~RendererManager() override;

        void Init() override;
        void Shutdown() override;

        RendererManager(const RendererManager&) = delete;
        RendererManager& operator=(const RendererManager&) = delete;
        RendererManager(RendererManager&&) = delete;
        RendererManager& operator=(RendererManager&&) = delete;

        RenderingContext& GetContext();

    private:

        GraphicsOwnedPtr<RenderingContext> m_RenderContext;
    };

}
