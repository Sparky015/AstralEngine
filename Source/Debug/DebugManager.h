//
// Created by Andrew Fagan on 11/1/24.
//

#pragma once

#include "Core/SystemManager.h"

namespace Debug{

    class DebugManager : public SystemManager
    {
    public:

        static DebugManager& Get();

        void Init() override;
        void Shutdown() override;
        void Update() override;
        void RenderImGui() override;

        void ImGuiBegin();
        void ImGuiEnd();

        DebugManager(const DebugManager&) = delete;
        DebugManager& operator=(const DebugManager&) = delete;
        DebugManager(DebugManager&&) = delete;
        DebugManager& operator=(DebugManager&&) = delete;

    private:
        DebugManager() = default;
        ~DebugManager() override = default;

        void InitImGui();
        void ShutdownImGui();

        float m_Time{0};
    };

    extern DebugManager& g_DebugManager;

}
