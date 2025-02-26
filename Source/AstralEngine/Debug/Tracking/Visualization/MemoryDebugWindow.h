/**
* @file MemoryDebugWindow.h
* @author Andrew Fagan
* @date 2/15/25
*/

#pragma once

#include "Debug/Tracking/Serialization/SceneMetricsStorage.h"
#include "Debug/Tracking/Serialization/SceneMetricsImporter.h"

#include <filesystem>

#include "Debug/ImGui/ImGuiEvents.h"

namespace Core {

    class MemoryDebugWindow
    {
    public:
        MemoryDebugWindow();
        ~MemoryDebugWindow() = default;

        bool LoadMemoryProfile(const std::filesystem::path& path);

        /**@brief Updates the memory debug window with new input */
        void ShowMemoryDebugWindow();

        /**@brief Shutdowns and closes the memory debug window. */
        void CloseMemoryDebugWindow();

    private:


        void OnRenderImGuiEvent(RenderImGuiEvent e);

        EventListener<RenderImGuiEvent> m_ImGuiRenderListener;
        SceneMetricsImporter m_SceneMetricsImporter;

        bool m_ShowDebugWindow;
    };

}
