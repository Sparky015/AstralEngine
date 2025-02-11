/**
* @file SceneMetricsExporter.cpp
* @author Andrew Fagan
* @date 2/8/25
*/

#include "SceneMetricsExporter.h"
#include "Debug/Macros/Loggers.h"
#include <format>
#include <chrono>

#include <iostream>
namespace Core {

    SceneMetricsExporter::SceneMetricsExporter() : m_NumberOfSnapshots(0)
    {

    }


    void SceneMetricsExporter::OpenExportFile(const char* sceneName)
    {
        constexpr int MAX_SCENE_NAME_LENGTH = 75;
        unsigned int sceneNameLength = std::strlen(sceneName);
        if (sceneNameLength > MAX_SCENE_NAME_LENGTH) { sceneNameLength = MAX_SCENE_NAME_LENGTH; }

        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        char dateBuffer[20];
        std::strftime(dateBuffer, sizeof(dateBuffer), "%d-%m-%Y_%H-%M-%S", std::localtime(&now_c));

        char fileNameBuffer[100];
        const char* filePrefix = "MemoryProfile_";

        strcpy_s(fileNameBuffer, filePrefix);
        std::memcpy(fileNameBuffer + std::strlen(filePrefix), sceneName, sceneNameLength);
        fileNameBuffer[std::strlen(filePrefix) + sceneNameLength] = '_';  // Add null terminator after memcpy
        std::memcpy(fileNameBuffer + std::strlen(filePrefix) + sceneNameLength + 1, dateBuffer, 20);

        m_File.open(fileNameBuffer, std::ios::out);
    }


    void SceneMetricsExporter::CloseExportFile()
    {
         if (m_File.is_open())
         {
             m_File.close();

             if (m_File.fail())
             {
                 WARN("Memory profiling export file failed to close!")
             }
         }
    }

    void SceneMetricsExporter::WriteMemoryMetricsSnapshot(const MemoryMetrics& snapshot)
    {
        // Write MessagePack code
        m_NumberOfSnapshots++;
    }

}

