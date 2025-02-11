/**
* @file TrackingSceneManager.cpp
* @author Andrew Fagan
* @date 2/8/25
*/

#include "SceneMetricsAccumulator.h"
#include "Debug/Macros/Asserts.h"
#include "Debug/Macros/Error.h"

namespace Core {

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