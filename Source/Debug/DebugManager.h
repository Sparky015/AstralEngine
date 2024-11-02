//
// Created by Andrew Fagan on 11/1/24.
//

#pragma once

#include "Core/SystemManager.h"

#include "Window/WindowEvents.h"

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
        DebugManager();
        ~DebugManager() override = default;

        void InitImGui();
        void ShutdownImGui();

        void onKeyPress(KeyPressedEvent keyPressedEvent);

        float m_Time{0};
        bool m_ShowDebugMenu{false};

        Event::EventListener<KeyPressedEvent> m_KeyPressedListener;
    };

    extern DebugManager& g_DebugManager;

}
