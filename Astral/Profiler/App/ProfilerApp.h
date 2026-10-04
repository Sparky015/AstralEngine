/**
* @file ProfilerApp.h
* @author Andrew Fagan
* @date 3/29/2026
*/

#pragma once

#include "Components/MenuComponent.h"
#include "ProfilerSceneDataCache.h"
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

        /**
         * @brief Initializes the profiler
         */
        void Init();

        /**
         * @brief Shuts down the profiler
         */
        void Shutdown();

        /**
         * @brief Loads a memory profile from the given file path
         * @param filePath The file path of the memory profile to load from disk
         */
        void LoadMemoryScene(std::string filePath);

        /**
         * @brief Gets the scene's data cache for preprocessed data
         * @return The scene's data cache for preprocessed data
         */
        ProfilerSceneDataCache& GetSceneDataCache();

    private:

        void UpdateProfilerAppUI();

        static ProfilerApp* m_Instance; // Pointing to memory on the stack
        bool m_ShowDebugWindow = true;

        SceneMetricsImporter m_SceneMetricsImporter;
        GraphView m_GraphsView = {};
        ProfilerSceneDataCache m_SceneDataCache;

        Astral::EventListener<RenderImGuiEvent> m_RenderImGuiListener{[this](RenderImGuiEvent e){ UpdateProfilerAppUI(); }};
    };

}
