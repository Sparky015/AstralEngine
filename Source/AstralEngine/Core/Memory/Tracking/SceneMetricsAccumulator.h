/**
* @file TrackingSceneManager.h
* @author Andrew Fagan
* @date 2/8/25
*/

#pragma once

#include "MemoryMetrics.h"
#include "SceneMetricsExporter.h"

namespace Core {

    class SceneMetricsAccumulator
    {
    public:
        SceneMetricsAccumulator();
        ~SceneMetricsAccumulator() = default;

        /**@brief Starts recording the memory metrics to a file.
         * @param sceneName The name of the scene.
         * @return True if the opening the export file succeeded and false if the file failed to open. */
        [[nodiscard]] bool BeginScene(const char* sceneName);

        /**@brief Stops recording the memory metrics to a file and close export file */
        void EndScene();

        /**@brief Takes the current state of the MemoryMetrics and exports to a file */
        void RecordMemoryMetrics(const MemoryMetrics& memoryMetrics);

        [[nodiscard]] bool IsSceneActive() const { return m_IsSceneActive; }


    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_SceneStartTimePoint;
        bool m_IsSceneActive;
        SceneMetricsExporter m_SceneMetricsExporter;
    };

}
