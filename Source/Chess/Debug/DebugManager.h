//
// Created by Andrew Fagan on 1/2/25.
//

#pragma once


#include "Core/SystemManager.h"

#include "Window/WindowEvents.h"

namespace Debug {

    class DebugManager : public SystemManager
    {
    public:

        static DebugManager& Get();

        void Init() override;
        void Shutdown() override;
        void RenderImGui() override;


        DebugManager(const DebugManager&) = delete;
        DebugManager& operator=(const DebugManager&) = delete;
        DebugManager(DebugManager&&) = delete;
        DebugManager& operator=(DebugManager&&) = delete;

    private:
        DebugManager() = default;
        ~DebugManager() override = default;

    };

    extern DebugManager& g_DebugManager;

}
