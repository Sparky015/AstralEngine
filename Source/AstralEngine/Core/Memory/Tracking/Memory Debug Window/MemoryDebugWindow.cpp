/**
* @file MemoryDebugWindow.cpp
* @author Andrew Fagan
* @date 2/15/25
*/

#include "MemoryDebugWindow.h"

#include <ImPlot/implot.h>

#include "Debug/Macros/Loggers.h"
#include "imgui.h"
#include "ImPlot/implot_internal.h"
#include "Panels/AllocationStacktracePanel.h"
#include "Panels/DataPointInfoPanel.h"
#include "Panels/GraphSelectorPanel.h"
#include "Panels/MemoryGraphPanel.h"

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
                    size_t* selectedDataPoint = GetSelectedDataPointAddress();

                    ShowMemoryGraphPanel(m_SceneMetricsImporter.GetSceneMetrics(), graphSelection);
                    ShowDataPointInfoPanel(m_SceneMetricsImporter.GetSceneMetrics(), selectedDataPoint);
                    AllocationStackTracePanel(m_SceneMetricsImporter.GetSceneMetrics(), selectedDataPoint);

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Allocator"))
                {
                    // Tab 1 content goes here
                    ImGui::Text("This is tab 1");

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Region"))
                {


                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Thread"))
                {

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

