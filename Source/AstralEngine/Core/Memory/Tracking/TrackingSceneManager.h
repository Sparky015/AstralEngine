/**
* @file TrackingSceneManager.h
* @author Andrew Fagan
* @date 2/8/25
*/

#pragma once

#include "MemoryMetrics.h"
#include "SceneMetricsExporter.h"

namespace Core {

    class TrackingSceneManager
    {
    public:
        TrackingSceneManager() = default;
        ~TrackingSceneManager() = default;

        /**@brief Starts recording the memory metrics to a file.
         * @param sceneName The name of the scene. */
        void BeginScene(const char* sceneName);

        /**@brief Stops recording the memory metrics to a file. */
        void EndScene();

        /**@brief Takes the current state of the MemoryMetrics and exports to a file */
        void RecordMemoryMetrics(const MemoryMetrics& memoryMetrics);


    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_SceneStartTimePoint;
        bool m_IsSceneActive;
        SceneMetricsExporter m_SceneMetricsExporter;
    };

}
