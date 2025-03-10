//
// Created by Andrew Fagan on 11/1/24.
//

#pragma once

#include "Core/SystemManager.h"
#include "Window/WindowEvents.h"

namespace Debug{

    class ImGuiManager : public SystemManager
    {
    public:

        static ImGuiManager& Get();

        void Init() override;
        void Shutdown() override;
        void RenderImGui() override;

        void BeginFrame();
        void EndFrame();

        void EnableViewportDockSpace() { m_ShowViewportDockSpace = true; }
        void DisableViewportDockSpace() { m_ShowViewportDockSpace = false; }

        ImGuiManager(const ImGuiManager&) = delete;
        ImGuiManager& operator=(const ImGuiManager&) = delete;
        ImGuiManager(ImGuiManager&&) = delete;
        ImGuiManager& operator=(ImGuiManager&&) = delete;

    private:
        ImGuiManager();
        ~ImGuiManager() override;

        void InitImGui() const;
        void ShutdownImGui() const;

        void OnKeyPress(KeyPressedEvent keyPressedEvent);

        static float m_Time;
        bool m_ShowDebugMenu{true};
        bool m_ShowViewportDockSpace{false};

        Core::EventListener<KeyPressedEvent> m_KeyPressedListener;
    };

    extern ImGuiManager& g_ImGuiManager;

}
