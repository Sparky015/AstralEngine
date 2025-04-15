/**
* @file SceneMetricsExporter.cpp
* @author Andrew Fagan
* @date 2/8/25
*/

#include "Debug/Utilities/Error.h"
#include "Debug/Utilities/Loggers.h"
#include "SceneMetricsExporter.h"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <cpptrace/formatting.hpp>

#include "cpptrace/include/cpptrace/cpptrace.hpp"

#include "Debug/MemoryTracking/MemoryTracker.h"


namespace Core {

    SceneMetricsExporter::SceneMetricsExporter() : m_IsSceneActive(false), m_NumberOfSnapshots(0)
    {
    }

    void SceneMetricsExporter::InitExportFile()
    {
        // Initializes the export file before any scenes start to prevent heap allocations being recorded as a
        // part of a scope's allocations when opening the export file for the first time. (Reference to PROFILE_SCOPE macro)
        // Basically, it avoids an allocation from being made on the first BeginScene() to avoid messing up
        // the memory tracking metrics.
        (void)GetExportFile();
    }

    bool SceneMetricsExporter::BeginScene(const char* sceneName)
    {
        [[unlikely]] if (m_IsSceneActive)
        {
            CloseExportFile();
            ASTRAL_ERROR("Can't begin a new scene when a scene is already active!")
        }
        OpenExportFile(sceneName);
        m_IsSceneActive = true;
        m_SceneClock.Reset();
        return IsExportFileOpen();
    }


    void SceneMetricsExporter::EndScene()
    {
        [[likely]] if (m_IsSceneActive)
        {
            // Pack the number of memory metrics snapshots in the file at the end
            msgpack::pack(GetExportFile(), m_NumberOfSnapshots);

            CloseExportFile();
        }

        m_NumberOfSnapshots = 0;
        m_IsSceneActive = false;
    }


    void SceneMetricsExporter::RecordMemoryMetrics(const MemoryMetrics& memoryMetrics, const AllocationData& allocationData)
    {

        if (GetExportFile().fail())
        {
            std::cout << "Memory profiling export file is in a failed state!\n";
            return;
        }

        if (m_IsSceneActive)
        {
            AllocationDataSerializeable allocationDataSerializable{};
            allocationDataSerializable.pointer = (uintptr_t)allocationData.pointer;
            allocationDataSerializable.region = allocationData.region;
            allocationDataSerializable.size = allocationData.size;
            allocationDataSerializable.allocatorType = allocationData.allocatorType;
            allocationDataSerializable.threadIDHash = memoryMetrics.GetThreadIDHash(allocationData.threadID);

            m_NumberOfSnapshots++;
            msgpack::pack(GetExportFile(), memoryMetrics);
            msgpack::pack(GetExportFile(), m_SceneClock.GetTimeMicroseconds());
            msgpack::pack(GetExportFile(), allocationDataSerializable);

            Core::MemoryTracker::Get().DisableTracking(); // To avoid allocations caused by cpptrace from being picked up by the memory tracker
            static cpptrace::formatter m_StacktraceFormatter = cpptrace::formatter{}
                .addresses(cpptrace::formatter::address_mode::none)
                .snippets(false)
                .colors(cpptrace::formatter::color_mode::none)
                .paths(cpptrace::formatter::path_mode::full);
            msgpack::pack(GetExportFile(), m_StacktraceFormatter.format(cpptrace::stacktrace::current(2)));

            Core::MemoryTracker::Get().EnableTracking();

        }

    }


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

        std::strftime(yearMonthBuffer, sizeof(yearMonthBuffer), "%Y-%m", &localTime);
        std::strftime(dayBuffer, sizeof(dayBuffer), "Day-%d", &localTime);
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


        // std::cout << "Stream state before open:" << std::endl;
        // std::cout << "- is_open(): " << GetExportFile().is_open() << std::endl;
        // std::cout << "- good(): " << GetExportFile().good() << std::endl;
        // std::cout << "- fail(): " << GetExportFile().fail() << std::endl;
        // std::cout << "- bad(): " << GetExportFile().bad() << std::endl;

        GetExportFile().open(filePathBuffer, std::ios::out | std::ios::binary);

        // std::cout << "Stream state after open:" << std::endl;
        // std::cout << "- is_open(): " << GetExportFile().is_open() << std::endl;
        // std::cout << "- good(): " << GetExportFile().good() << std::endl;
        // std::cout << "- fail(): " << GetExportFile().fail() << std::endl;
        // std::cout << "- bad(): " << GetExportFile().bad() << std::endl;
    }


    void SceneMetricsExporter::CloseExportFile()
    {
         if (GetExportFile().is_open())
         {
             // Log the stream state after opening
             std::cout << "Stream state before close:" << std::endl;
             std::cout << "- is_open(): " << GetExportFile().is_open() << std::endl;
             std::cout << "- good(): " << GetExportFile().good() << std::endl;
             std::cout << "- fail(): " << GetExportFile().fail() << std::endl;
             std::cout << "- bad(): " << GetExportFile().bad() << std::endl;

             GetExportFile().close();
             // GetExportFile().clear();

             std::cout << "Stream state after close:" << std::endl;
             std::cout << "- is_open(): " << GetExportFile().is_open() << std::endl;
             std::cout << "- good(): " << GetExportFile().good() << std::endl;
             std::cout << "- fail(): " << GetExportFile().fail() << std::endl;
             std::cout << "- bad(): " << GetExportFile().bad() << std::endl;

             if (GetExportFile().is_open())
             {
                 std::cout << "Memory profiling export file failed to close!";
             }
         }
    }

}

