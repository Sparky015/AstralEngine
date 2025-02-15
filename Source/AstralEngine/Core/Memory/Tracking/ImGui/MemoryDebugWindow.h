/**
* @file MemoryDebugWindow.h
* @author Andrew Fagan
* @date 2/15/25
*/

#pragma once

#include "Core/Memory/Tracking/SceneMetricsStorage.h"
#include <filesystem>

#include "Core/Memory/Tracking/SceneMetricsImporter.h"

namespace Core {

    class MemoryDebugWindow
    {
    public:
        explicit MemoryDebugWindow(const SceneMetricsStorage& sceneMetricsStorage);
        ~MemoryDebugWindow() = default;

        bool LoadMemoryProfile(const std::filesystem::path& path);

        /**@brief Updates the memory debug window with new input */
        void ShowMemoryDebugWindow();

        /**@brief Shutdowns and closes the memory debug window. */
        void CloseMemoryDebugWindow();

    private:

        /**@brief Spawns a new ImGui window for the memory debugging info to be shown in. */
        bool SpawnNewImGuiWindow();

        /**@brief Shows the tabs at the top of the memory debug window that allows the user to select
         *        how they want the data to be shown by */
        void UsageBySelectorComponent();

        /**@brief Allows the user to select which category of data to graph (ex. memory usage, active allocations, peak memory usage) */
        void SelectDataCategoryComponent();

        /**@brief Shows the graph of the data that the user selected. */
        void ShowSelectedGraphComponent();

        SceneMetricsImporter m_SceneMetricsImporter;
    };

}
