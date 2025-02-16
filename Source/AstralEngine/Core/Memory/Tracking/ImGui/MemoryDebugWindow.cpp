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

namespace Core {

    MemoryDebugWindow::MemoryDebugWindow() :
        m_ImGuiRenderListener([this](RenderImGuiEvent e){ OnRenderImGuiEvent(e); })
    {

    }

    bool MemoryDebugWindow::LoadMemoryProfile(const std::filesystem::path& path)
    {
        m_SceneMetricsImporter.ImportMemoryProfile(path);
        if (m_SceneMetricsImporter.HasFileLoaded())
        {
            m_ImGuiRenderListener.StartListening();
        }
        else
        {
            m_ImGuiRenderListener.StopListening();
        }
        return m_SceneMetricsImporter.HasFileLoaded();
    }


    void MemoryDebugWindow::ShowMemoryDebugWindow() const
    {
        if (!m_SceneMetricsImporter.HasFileLoaded())
        {
            WARN("MemoryDebugWindow: No memory profile file is currently loaded!");
            return;
        }

        static bool showMemoryDebugWindow = true;
        ImPlot::ShowDemoWindow(&showMemoryDebugWindow);
        ImGui::Begin("Memory Debug Window", &showMemoryDebugWindow);

        if (ImGui::BeginTabBar("Region"))
        {
            if (ImGui::BeginTabItem("Tab 1"))
            {
                // Tab 1 content goes here
                ImGui::Text("This is tab 1");

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Allocator"))
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

            if (ImGui::BeginTabItem("Global"))
            {
                // Settings tab content
                static float someValue = 2.0f;
                if (ImPlot::BeginPlot("Memory Usage"))
                {
                    const SceneMetricsStorage& storage = m_SceneMetricsImporter.GetSceneMetrics();
                    ImPlot::SetupAxes("Allocation Number", "Memory Usage");
                    int max = *std::max_element(storage.GetGlobalMemoryUsageOverTime().begin(), storage.GetGlobalMemoryUsageOverTime().end());
                    ImPlot::SetupAxisLimitsConstraints(ImAxis_X1, 0, storage.GetSnapshotCount());
                    ImPlot::SetupAxisLimitsConstraints(ImAxis_Y1, 0, max);
                    ImPlot::SetupAxisZoomConstraints(ImAxis_X1, 0, storage.GetSnapshotCount());
                    ImPlot::SetupAxisZoomConstraints(ImAxis_Y1, 0, max);
                    ImPlot::PlotLine("Memory Usage", (ImU64*)storage.GetGlobalMemoryUsageOverTime().data(), storage.GetSnapshotCount());
                    ImPlot::EndPlot();
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();


    }

    void MemoryDebugWindow::OnRenderImGuiEvent(RenderImGuiEvent e)
    {
        ShowMemoryDebugWindow();
    }

}

