/**
* @file ProfilerApp.h
* @author Andrew Fagan
* @date 3/29/2026
*/

#pragma once

#include "MemoryDebugWindow.h"

#include "Components/MenuComponent.h"
#include "Core/Events/EventListener.h"
#include "Debug/ImGui/ImGuiEvents.h"

namespace Astral {

    class ProfilerApp
    {
    public:
        ProfilerApp();
        ~ProfilerApp();

        void Init();
        void Shutdown();

    private:

        void UpdateProfilerAppUI();

        bool m_ShowDebugWindow = true;

        SceneMetricsImporter m_SceneMetricsImporter;

        Astral::EventListener<RenderImGuiEvent> m_RenderImGuiListener{[this](RenderImGuiEvent e){ UpdateProfilerAppUI(); }};
    };

}