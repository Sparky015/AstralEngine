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
                    // Tab 2 content goes here
                    ImGui::Text("Welcome to tab 2");

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Thread"))
                {
                    ImGui::Columns(2, "MyColumns"); // Create 2 columns

                    float availableHeight = ImGui::GetContentRegionAvail().y;
                    float topHeight = availableHeight * 0.5f; // Adjust the split ratio as needed


                    // Left Column (Tree Node)
                    ImGui::BeginChild("LeftPane_Top", ImVec2(0, topHeight), true); // Create a child window for scrolling
                    if (ImGui::TreeNode("Tree Node")) {
                        // Your tree node content here
                        ImGui::Text("Item 1");
                        ImGui::Text("Item 2");
                        ImGui::Text("Item 3");
                        ImGui::TreePop();
                    }
                    ImGui::EndChild();

                    ImGui::BeginChild("LeftPane_Bot", ImVec2(0, 0), true); // Create a child window for scrolling
                    if (ImGui::TreeNode("Tree Node 2")) {
                        // Your tree node content here
                        ImGui::Text("Item 4");
                        ImGui::Text("Item 5");
                        ImGui::Text("Item 6");
                        ImGui::TreePop();
                    }
                    ImGui::EndChild();


                    ImGui::NextColumn(); // Move to the next column

                    // Right Column (ImPlot Graph)
                    ImGui::BeginChild("RightPane", ImVec2(0, 0), true);
                    if (ImPlot::BeginPlot("My Plot")) {
                        // Your ImPlot plotting code here
                        double x[100], y[100];
                        for (int i = 0; i < 100; ++i) {
                            x[i] = i * 0.1;
                            y[i] = sin(x[i]);
                        }
                        ImPlot::PlotLine("sin(x)", x, y, 100);
                        ImPlot::EndPlot();
                    }
                    ImGui::EndChild();

                    ImGui::Columns(1); // Reset to a single column
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }

    }

    void MemoryDebugWindow::CloseMemoryDebugWindow()
    {
        m_SceneMetricsImporter.CloseMemoryProfile();
        m_ShowDebugWindow = false;
    }

    void MemoryDebugWindow::OnRenderImGuiEvent(RenderImGuiEvent e)
    {
        ShowMemoryDebugWindow();
    }

}

