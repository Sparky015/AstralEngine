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

        // std::time_t now_c = std::chrono::system_clock::to_time_t(now); // Convert to time_t
        // // Format the time string using std::strftime (or std::format if you prefer)
        // char buffer[20]; // Sufficient size for the format
        // std::strftime(buffer, sizeof(buffer), "%d-%m-%Y_%H-%M-%S", std::localtime(&now_c));

        // m_File.open(std::format("MemoryProfile_{}_{}", sceneName ), std::ios::out);
        return false; //m_File.is_open();
    }


    void SceneMetricsExporter::CloseExportFile()
    {
        // if (m_File.is_open())
        // {
        //     m_File.close();
        //     // TODO: Check if close failed
        // }
    }

    void SceneMetricsExporter::WriteMemoryMetrics(const MemoryMetrics& memoryMetrics)
    {

    }

}

