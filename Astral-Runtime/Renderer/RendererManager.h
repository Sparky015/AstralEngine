//
// Created by Andrew Fagan on 11/28/24.
//

#pragma once

#include "Core/SystemManager.h"


namespace Astral {

    class RendererManager : public SystemManager
    {
    public:
        RendererManager();
        ~RendererManager() override;

        static RendererManager& Get();
        void Init() override;
        void Update() override;
        void Shutdown() override;

        RendererManager(const RendererManager&) = delete;
        RendererManager& operator=(const RendererManager&) = delete;
        RendererManager(RendererManager&&) = delete;
        RendererManager& operator=(RendererManager&&) = delete;

    private:

    };

    extern RendererManager& g_RendererManager;
}