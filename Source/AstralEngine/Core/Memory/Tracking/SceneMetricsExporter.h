/**
* @file SceneMetricsExporter.h
* @author Andrew Fagan
* @date 2/8/25
*/

#pragma once

#include "MemoryMetrics.h"
#include <fstream>

namespace Core {

    /**@brief Exports snapshots of the memory metrics to a file */
    class SceneMetricsExporter
    {
    public:
        SceneMetricsExporter();

        /**@brief Opens a file for exporting scene memory metrics */
        void OpenExportFile(const char* sceneName);

        /**@brief Closes the file for exporting scene memory metrics */
        void CloseExportFile();

        [[nodiscard]] bool IsExportFileOpen() const { return m_File.is_open(); }

        /**@brief Writes a snapshot of the memory metrics to the file */
        void WriteMemoryMetricsSnapshot(const MemoryMetrics& snapshot);

    private:

        /**@brief Starts recording the memory metrics to a file.
         * @param sceneName The name of the scene.
         * @return True if the opening the export file succeeded and false if the file failed to open. */
        [[nodiscard]] bool BeginScene(const char* sceneName);

        /**@brief Stops recording the memory metrics to a file and close export file */
        void EndScene();

        /**@brief Takes the current state of the MemoryMetrics and exports to a file */
        void RecordMemoryMetrics(const MemoryMetrics& memoryMetrics);

        [[nodiscard]] bool IsSceneActive() const { return m_IsSceneActive; }

        bool m_IsSceneActive;
        std::fstream m_File;
        size_t m_NumberOfSnapshots;
    };

}
