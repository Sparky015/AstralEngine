/**
* @file SceneMetricsExporter.h
* @author Andrew Fagan
* @date 2/8/25
*/

#pragma once

#include "Debug/MemoryTracking/MemoryMetrics.h"
#include "MemoryMetricsSnapshot.h"
#include <fstream>
#include <cpptrace/formatting.hpp>

#include "msgpack.hpp"

#include "Core/Time/Clock.h"


namespace Core {

    /**@brief Exports snapshots of the memory metrics to a file */
    class SceneMetricsExporter
    {
    public:
        SceneMetricsExporter();
        ~SceneMetricsExporter() = default;

        void InitExportFile();

        /**@brief Starts recording the memory metrics to a file.
         * @param sceneName The name of the scene.
         * @return True if the opening the export file succeeded and false if the file failed to open. */
        [[nodiscard]] bool BeginScene(const char* sceneName);

        /**@brief Stops recording the memory metrics to a file and close export file */
        void EndScene();

        /**@brief Checks if a scene is currently active
         * @return True if the scene is active and false if not */
        [[nodiscard]] bool IsSceneActive() const { return m_IsSceneActive; }

        /**@brief Checks if the export file is open
         * @return True if the export file is open and false if not */
        [[nodiscard]] bool IsExportFileOpen() const { return GetExportFile().is_open(); }

        /**@brief Takes the current state of the MemoryMetrics and exports to a file */
        void RecordMemoryMetrics(const MemoryMetrics& memoryMetrics, const AllocationData& allocationData);

    private:

        /**@brief Opens a file for exporting scene memory metrics
         * @param sceneName The name of the scene (for labeling purposes) */
        void OpenExportFile(const char* sceneName);

        /**@brief Closes the file for exporting scene memory metrics */
        void CloseExportFile();

        [[nodiscard]] std::fstream& GetExportFile() const
        {
            static std::fstream fileStream;
            return fileStream;
        }

        Clock m_SceneClock;
        bool m_IsSceneActive;
        size_t m_NumberOfSnapshots;
    };

}
