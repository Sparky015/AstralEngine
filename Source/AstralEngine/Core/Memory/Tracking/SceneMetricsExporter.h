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
         std::fstream m_File;
        size_t m_NumberOfSnapshots;
    };

}
