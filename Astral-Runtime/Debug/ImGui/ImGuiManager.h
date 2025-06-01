//
// Created by Andrew Fagan on 11/1/24.
//

#pragma once

#include <string_view>

#include "Core/SystemManager.h"
#include "Window/WindowEvents.h"

namespace Debug{

    class ImGuiManager : public SystemManager
    {
    public:
        ImGuiManager();
        ~ImGuiManager() override;

        void Init() override;
        void Shutdown() override;
        void RenderImGui() override;

        void BeginFrame();
        void EndFrame();

        void EnableViewportDockSpace() { m_ShowViewportDockSpace = true; }
        void DisableViewportDockSpace() { m_ShowViewportDockSpace = false; }
        void LoadImGuiConfigFile(const std::string_view& filePath);

        ImGuiManager(const ImGuiManager&) = delete;
        ImGuiManager& operator=(const ImGuiManager&) = delete;
        ImGuiManager(ImGuiManager&&) = delete;
        ImGuiManager& operator=(ImGuiManager&&) = delete;

    private:


        void InitImGui() const;
        void ShutdownImGui() const;

        void OnKeyPress(KeyPressedEvent keyPressedEvent);

        static float m_Time;
        bool m_ShowDebugMenu{true};
        bool m_ShowViewportDockSpace{false};

        Astral::EventListener<KeyPressedEvent> m_KeyPressedListener;
    };

}
