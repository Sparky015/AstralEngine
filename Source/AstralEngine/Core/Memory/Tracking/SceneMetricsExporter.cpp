/**
* @file SceneMetricsExporter.cpp
* @author Andrew Fagan
* @date 2/8/25
*/

#include "SceneMetricsExporter.h"
#include "Debug/Macros/Loggers.h"
#include <chrono>
#include <filesystem>

namespace Core {

    SceneMetricsExporter::SceneMetricsExporter() : m_NumberOfSnapshots(0) {}


    void SceneMetricsExporter::OpenExportFile(const char* sceneName)
    {
        constexpr int MAX_SCENE_NAME_LENGTH = 90;
        unsigned int sceneNameLength = std::strlen(sceneName);
        if (sceneNameLength > MAX_SCENE_NAME_LENGTH) { sceneNameLength = MAX_SCENE_NAME_LENGTH; }

        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&now_c);

        char yearMonthBuffer[8];
        char dayBuffer[7];
        char timeBuffer[10];

        std::strftime(yearMonthBuffer, sizeof(yearMonthBuffer), "%Y-%#m", &localTime);
        std::strftime(dayBuffer, sizeof(dayBuffer), "Day-%#d", &localTime);
        std::strftime(timeBuffer, sizeof(timeBuffer), "%H-%M-%S", &localTime);

        constexpr std::string_view logFileDir = LOG_FILE_DIR;
        char filePathBuffer[logFileDir.length() + 105];

        std::memcpy(filePathBuffer, LOG_FILE_DIR, logFileDir.length());
        filePathBuffer[logFileDir.length()] = '\0';
        std::filesystem::create_directories(filePathBuffer);

        std::memcpy(filePathBuffer + logFileDir.length(), yearMonthBuffer, std::strlen(yearMonthBuffer));
        filePathBuffer[logFileDir.length() + std::strlen(yearMonthBuffer)] = '/';
        filePathBuffer[logFileDir.length() + std::strlen(yearMonthBuffer) + 1] = '\0';
        std::filesystem::create_directories(filePathBuffer);

        std::memcpy(filePathBuffer + logFileDir.length() + std::strlen(yearMonthBuffer) + 1, dayBuffer, std::strlen(dayBuffer));
        filePathBuffer[logFileDir.length() + std::strlen(yearMonthBuffer) + std::strlen(dayBuffer) + 1] = '/';
        filePathBuffer[logFileDir.length() + std::strlen(yearMonthBuffer) + std::strlen(dayBuffer) + 2] = '\0';
        std::filesystem::create_directories(filePathBuffer);

        char fileNameBuffer[150];
        const char* filePrefix = "MemoryProfile_";
        const char* fileExtension = ".ASTLMemProfile";

        memcpy(fileNameBuffer, filePrefix, strlen(filePrefix));
        memcpy(fileNameBuffer + strlen(filePrefix), sceneName, sceneNameLength);
        fileNameBuffer[strlen(filePrefix) + sceneNameLength] = '_';
        memcpy(fileNameBuffer + strlen(filePrefix) + sceneNameLength + 1, timeBuffer, strlen(timeBuffer));
        memcpy(fileNameBuffer + strlen(filePrefix) + sceneNameLength + strlen(timeBuffer), fileExtension, strlen(fileExtension));
        fileNameBuffer[strlen(filePrefix) + sceneNameLength + std::strlen(timeBuffer) + strlen(fileExtension)] = '\0';

        const int pathLength = std::strlen(filePathBuffer);
        std::memcpy(filePathBuffer + pathLength, fileNameBuffer, std::strlen(fileNameBuffer));
        filePathBuffer[pathLength + std::strlen(fileNameBuffer)] = '\0';


        m_File.open(filePathBuffer, std::ios::out);
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

    SceneMetricsAccumulator::SceneMetricsAccumulator() : m_IsSceneActive(false) {}


    bool SceneMetricsAccumulator::BeginScene(const char* sceneName)
    {
        [[unlikely]] if (m_IsSceneActive)
        {
            m_SceneMetricsExporter.CloseExportFile();
            ERROR("Can't begin a new scene when a scene is already active!")
        }
        m_IsSceneActive = true;
        m_SceneMetricsExporter.OpenExportFile(sceneName);
        return m_SceneMetricsExporter.IsExportFileOpen();
    }


    void SceneMetricsAccumulator::EndScene()
    {
        [[likely]] if (m_IsSceneActive)
        {
            m_SceneMetricsExporter.CloseExportFile();
        }

        m_IsSceneActive = false;
    }


    void SceneMetricsAccumulator::RecordMemoryMetrics(const MemoryMetrics& memoryMetrics)
    {
        if (m_IsSceneActive)
        {
            m_SceneMetricsExporter.WriteMemoryMetricsSnapshot(memoryMetrics);
        }
    }

}

