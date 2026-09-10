/**
* @file SceneMetricsExporter.h
* @author Andrew Fagan
* @date 2/8/25
*/

#pragma once

#include "Core/Threading/ThreadPool.h"
#include "Core/Threading/Locks/SpinLock.h"
#include "Core/Time/Clock.h"
#include "Profiler/MemoryTracking/MemoryMetrics.h"
#include "MemoryMetricsSnapshot.h"

#include "msgpack.hpp"
#include <cpptrace/formatting.hpp>
#include <fstream>



namespace Astral {

    /**
     * @brief Exports snapshots of the memory metrics to a file
     */
    class SceneMetricsExporter
    {
    public:
        SceneMetricsExporter();
        ~SceneMetricsExporter() = default;

        void InitExportFile();

        /**
         * @brief Starts recording the memory metrics to a file.
         * @param sceneName The name of the scene.
         * @param initialMemoryMetrics
         * @return True if the opening the export file succeeded and false if the file failed to open.
         */
        [[nodiscard]] bool BeginScene(const char* sceneName, const MemoryMetrics& initialMemoryMetrics);

        /**
         * @brief Stops recording the memory metrics to a file and close export file
         */
        void EndScene();

        /**
         * @brief Checks if a scene is currently active
         * @return True if the scene is active and false if not
         */
        [[nodiscard]] bool IsSceneActive() const { return m_IsSceneActive; }

        /**
         * @brief Checks if the export file is open
         * @return True if the export file is open and false if not
         */
        [[nodiscard]] bool IsExportFileOpen() const { return GetExportFile().is_open(); }

        /**
         * @brief Saves the current scene time to a buffer for deferred processing
         */
        void SaveSceneProfilingTimeToBuffer();

        /**
         * @brief Saves the memory allocator operation data to a buffer for deferred processing
         */
        void SaveOperationDataToBuffer(const AllocationData& allocationData, bool isFreeOperation);

        /**
         * @brief Records the raw stacktrace of the current stack and saves it to a buffer for deferred processing
         */
        void CaptureRawStacktraceToBuffer();

    private:

        /**
         * @brief Opens a file for exporting scene memory metrics
         * @param sceneName The name of the scene (for labeling purposes)
         */
        void OpenExportFile(const char* sceneName);

        /**
         * @brief Closes the file for exporting scene memory metrics
         */
        void CloseExportFile();

        /**
         * @brief Processes the raw trace buffer into resolved stacktraces
         */
        void ProcessRawTraces();

        /**
         * @brief Writes the processed stacktraces to the scene export file
         */
        void WriteBufferedDataToFile();

        [[nodiscard]] std::fstream& GetExportFile() const
        {
            static std::fstream fileStream;
            return fileStream;
        }

        Clock m_SceneClock;
        std::atomic_bool m_IsSceneActive;
        std::atomic_size_t m_NumberOfSnapshots;

        std::vector<size_t> m_TimepointBuffer;
        std::vector<AllocationDataSerializeable> m_AllocationDataBuffer;

        SpinLock m_RawTraceQueueLock;
        SpinLock m_OperationTimeBufferLock;
        SpinLock m_OperationDataBufferLock;
        std::stack<std::pair<cpptrace::raw_trace, int>> m_RawTraceProcessQueue;
        std::vector<std::string> m_ResolvedStacktraceBuffer;
    };

}
