/**
* @file SceneMetricsExporter.cpp
* @author Andrew Fagan
* @date 2/8/25
*/

#include "SceneMetricsExporter.h"
#include <format>
#include <chrono>


namespace Core {

    SceneMetricsExporter::SceneMetricsExporter()
    {

    }


    bool SceneMetricsExporter::OpenExportFile(const char* sceneName)
    {
        auto now = std::chrono::system_clock::now();
        m_File.open(std::format("MemoryProfile_{}_{}", sceneName, std::format("{%d-%m-%Y_%H-%M-%S}", now)), std::ios::out);
        return m_File.is_open();
    }


    void SceneMetricsExporter::CloseExportFile()
    {
        if (m_File.is_open())
        {
            m_File.close();
            // TODO: Check if close failed
        }
    }

    void SceneMetricsExporter::WriteMemoryMetrics(const MemoryMetrics& memoryMetrics)
    {

    }

}

