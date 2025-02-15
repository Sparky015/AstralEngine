/**
* @file MemoryDebugWindow.cpp
* @author Andrew Fagan
* @date 2/15/25
*/

#include "MemoryDebugWindow.h"

#include "Debug/Macros/Loggers.h"
#include "imgui.h"

namespace Core {

    MemoryDebugWindow::MemoryDebugWindow() :
        m_ImGuiRenderListener([this](RenderImGuiEvent& e){ OnRenderImGuiEvent(e); })
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


    void MemoryDebugWindow::ShowMemoryDebugWindow()
    {
        if (!m_SceneMetricsImporter.HasFileLoaded())
        {
            WARN("MemoryDebugWindow: No memory profile file is currently loaded!");
            return;
        }

        ImGui::Begin("Memory Debug Window");

        if (ImGui::BeginTabBar("MyTabBar")) {
            if (ImGui::BeginTabItem("Tab 1")) {
                // Tab 1 content goes here
                ImGui::Text("This is tab 1");

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Tab 2")) {
                // Tab 2 content goes here
                ImGui::Text("Welcome to tab 2");

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Settings")) {
                // Settings tab content
                static float someValue = 2.0f;
                ImGui::SliderFloat("Value", &someValue, 0.0f, 1.0f);

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();


    }

    void MemoryDebugWindow::OnRenderImGuiEvent(RenderImGuiEvent& e)
    {
        ShowMemoryDebugWindow();
    }

}

