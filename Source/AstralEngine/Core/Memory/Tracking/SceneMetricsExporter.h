/**
* @file SceneMetricsExporter.h
* @author Andrew Fagan
* @date 2/8/25
*/

#pragma once

#include <fstream>
#include "MemoryMetrics.h"

namespace Core {

    /**@brief Exports snapshots of the memory metrics to a file */
    class SceneMetricsExporter
    {
    public:
        SceneMetricsExporter();

        /**@brief Opens a file for exporting scene memory metrics */
        bool OpenExportFile(const char* sceneName);

        /**@brief Closes the file for exporting scene memory metrics */
        void CloseExportFile();

        /**@brief Writes a snapshot of the memory metrics to the file */
        void WriteMemoryMetrics(const MemoryMetrics& snapshot);

    private:
        // std::fstream m_File;
        size_t m_NumberOfSnapshots;
    };

}
