/**
* @file ProfilerApp.h
* @author Andrew Fagan
* @date 3/29/2026
*/

#pragma once


#include "Components/MenuComponent.h"
#include "Core/Events/EventListener.h"
#include "Debug/ImGui/ImGuiEvents.h"
#include "Profiler/MemoryTracking/Serialization/SceneMetricsImporter.h"
#include "Views/GraphViews.h"

namespace Astral {

    class ProfilerApp
    {
    public:
        ProfilerApp();
        ~ProfilerApp();

        /**
         * @brief Gets the ProfilerApp singleton instance
         * @return The ProfilerApp singleton instance
         */
        static ProfilerApp& Get();

        void Init();
        void Shutdown();

        void LoadMemoryScene(std::string filePath);

    private:

        void UpdateProfilerAppUI();

        static ProfilerApp* m_Instance; // Pointing to memory on the stack
        bool m_ShowDebugWindow = true;

        SceneMetricsImporter m_SceneMetricsImporter;
        GraphView m_GraphsView = {};

        Astral::EventListener<RenderImGuiEvent> m_RenderImGuiListener{[this](RenderImGuiEvent e){ UpdateProfilerAppUI(); }};
    };

}
