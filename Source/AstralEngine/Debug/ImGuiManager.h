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
        void Update() override;
        void RenderImGui() override;

        static void ImGuiBeginFrame();
        static void ImGuiEndFrame();

        ImGuiManager(const ImGuiManager&) = delete;
        ImGuiManager& operator=(const ImGuiManager&) = delete;
        ImGuiManager(ImGuiManager&&) = delete;
        ImGuiManager& operator=(ImGuiManager&&) = delete;

    private:
        ImGuiManager();
        ~ImGuiManager() override;

        void InitImGui();
        void ShutdownImGui();

        void OnKeyPress(KeyPressedEvent keyPressedEvent);

        static float m_Time;
        bool m_ShowDebugMenu{false};

        Event::EventListener<KeyPressedEvent> m_KeyPressedListener;
    };

    extern ImGuiManager& g_ImGuiManager;

}
