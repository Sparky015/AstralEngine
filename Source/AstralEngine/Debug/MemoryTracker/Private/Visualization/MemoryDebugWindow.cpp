/**
* @file MemoryDebugWindow.cpp
* @author Andrew Fagan
* @date 2/15/25
*/

#include "MemoryDebugWindow.h"

#include "ImPlot/implot.h"

#include "Debug/Utils/Public/Loggers.h"
#include "imgui.h"
#include "ImPlot/implot_internal.h"
#include "Debug/Tracking/Private/Visualization/Panels/AllocationsListPanel.h"
#include "Debug/Tracking/Private/Visualization/Panels/AllocationStacktracePanel.h"
#include "Debug/Tracking/Private/Visualization/Panels/DataPointInfoPanel.h"
#include "Debug/Tracking/Private/Visualization/Panels/GraphSelectorPanel.h"
#include "Debug/Tracking/Private/Visualization/Panels/MemoryGraphPanel.h"

namespace Core {

    MemoryDebugWindow::MemoryDebugWindow() :
        m_ImGuiRenderListener([this](RenderImGuiEvent e){ OnRenderImGuiEvent(e); }),
        m_ShowDebugWindow(false)
    {

    }

    bool MemoryDebugWindow::LoadMemoryProfile(const std::filesystem::path& path)
    {
        LOG("MemoryDebugWindow: Loading Memory Profile...")
        m_SceneMetricsImporter.ImportMemoryProfile(path);
        if (m_SceneMetricsImporter.HasFileLoaded())
        {
            m_ImGuiRenderListener.StartListening();
            m_ShowDebugWindow = true;
        }
        else
        {
            m_ImGuiRenderListener.StopListening();
        }
        return m_SceneMetricsImporter.HasFileLoaded();
    }


    void MemoryDebugWindow::ShowMemoryDebugWindow()
    {
        if (!m_SceneMetricsImporter.HasFileLoaded())
        {
            WARN("MemoryDebugWindow: No memory profile file is currently loaded!");
            return;
        }

        static bool showMemoryDebugWindow = true;

        if (showMemoryDebugWindow)
        {
            ImPlot::ShowDemoWindow(&showMemoryDebugWindow);
        }


        if (m_ShowDebugWindow)
        {
            ImGui::Begin("Memory Debug Window", &m_ShowDebugWindow);


            if (ImGui::BeginTabBar("MemoryDataType"))
            {
                if (ImGui::BeginTabItem("Global"))
                {
                    // Settings tab content
                    MemoryGraphSelection graphSelection = ShowGraphSelectorPanel();
                    size_t selectedDataPoint = AllocationsListPanel(m_SceneMetricsImporter.GetSceneMetrics());

                    ShowMemoryGraphPanel(m_SceneMetricsImporter.GetSceneMetrics(), MemoryMetricsDataScope::GLOBAL, graphSelection, selectedDataPoint);
                    ShowDataPointInfoPanel(m_SceneMetricsImporter.GetSceneMetrics(), selectedDataPoint);
                    AllocationStackTracePanel(m_SceneMetricsImporter.GetSceneMetrics(), selectedDataPoint);

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Allocator"))
                {
                    MemoryGraphSelection graphSelection = ShowGraphSelectorPanel();
                    size_t selectedDataPoint = AllocationsListPanel(m_SceneMetricsImporter.GetSceneMetrics());

                    ShowMemoryGraphPanel(m_SceneMetricsImporter.GetSceneMetrics(), MemoryMetricsDataScope::ALLOCATOR, graphSelection, selectedDataPoint);
                    ShowDataPointInfoPanel(m_SceneMetricsImporter.GetSceneMetrics(), selectedDataPoint);
                    AllocationStackTracePanel(m_SceneMetricsImporter.GetSceneMetrics(), selectedDataPoint);

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Region"))
                {
                    MemoryGraphSelection graphSelection = ShowGraphSelectorPanel();
                    size_t selectedDataPoint = AllocationsListPanel(m_SceneMetricsImporter.GetSceneMetrics());

                    ShowMemoryGraphPanel(m_SceneMetricsImporter.GetSceneMetrics(), MemoryMetricsDataScope::REGION, graphSelection, selectedDataPoint);
                    ShowDataPointInfoPanel(m_SceneMetricsImporter.GetSceneMetrics(), selectedDataPoint);
                    AllocationStackTracePanel(m_SceneMetricsImporter.GetSceneMetrics(), selectedDataPoint);

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Thread"))
                {
                    MemoryGraphSelection graphSelection = ShowGraphSelectorPanel();
                    size_t selectedDataPoint = AllocationsListPanel(m_SceneMetricsImporter.GetSceneMetrics());

                    ShowMemoryGraphPanel(m_SceneMetricsImporter.GetSceneMetrics(), MemoryMetricsDataScope::THREAD, graphSelection, selectedDataPoint);
                    ShowDataPointInfoPanel(m_SceneMetricsImporter.GetSceneMetrics(), selectedDataPoint);
                    AllocationStackTracePanel(m_SceneMetricsImporter.GetSceneMetrics(), selectedDataPoint);

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }
        else
        {
            CloseMemoryDebugWindow();
        }

    }

    void MemoryDebugWindow::CloseMemoryDebugWindow()
    {
        m_SceneMetricsImporter.CloseMemoryProfile();
        m_ImGuiRenderListener.StopListening();
        m_ShowDebugWindow = false;
    }

    void MemoryDebugWindow::OnRenderImGuiEvent(RenderImGuiEvent e)
    {
        ShowMemoryDebugWindow();
    }

}

