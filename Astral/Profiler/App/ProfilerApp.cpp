/**
* @file ProfilerApp.cpp
* @author Andrew Fagan
* @date 3/29/2026
*/

#include "ProfilerApp.h"

#include "Panels/AllocationStacktracePanel.h"
#include "Panels/AllocationsListPanel.h"
#include "Panels/DataPointInfoPanel.h"
#include "Panels/GraphSelectorPanel.h"
#include "Panels/MemoryGraphPanel.h"
#include "Views/GraphViews.h"

#include "Core/Engine.h"
#include "Core/Utilities/Loggers.h"
#include "Window/WindowManager.h"
#include "Debug/ImGui/ImGuiManager.h"

#include "ImPlot/implot.h"
#include "imgui.h"


namespace Astral {

    ProfilerApp::ProfilerApp()
    {
    }

    ProfilerApp::~ProfilerApp()
    {
    }


    void ProfilerApp::Init()
    {
        Engine::Get().GetImGuiManager().EnableViewportDockSpace();
        // imguiManager.LoadImGuiConfigFile(std::string(ASTRAL_EDITOR_SOURCE_DIR) + "imgui-editor-config.ini");
        m_RenderImGuiListener.StartListening();

        Astral::Window& window = Astral::Engine::Get().GetWindowManager().GetWindow();
        window.SetWindowName("Astral Profiler");
        window.SetWindowDimensions(1600, 900);
    }


    void ProfilerApp::Shutdown()
    {
        m_RenderImGuiListener.StopListening();
    }


    void ProfilerApp::UpdateProfilerAppUI()
    {
        PROFILE_SCOPE("ProfilerApp::UpdateProfilerAppUI")


        ImGui::Begin("Memory Debug Window", &m_ShowDebugWindow);

        MenuComponent::Show();

        if (ImGui::BeginTabBar("MemoryDataType"))
        {
            if (ImGui::BeginTabItem("Global"))
            {
                GlobalGraphView(m_SceneMetricsImporter.GetSceneMetrics());
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Allocator"))
            {
                AllocatorGraphsView(m_SceneMetricsImporter.GetSceneMetrics());
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Region"))
            {
                RegionGraphsView(m_SceneMetricsImporter.GetSceneMetrics());
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Thread"))
            {
                ThreadGraphsView(m_SceneMetricsImporter.GetSceneMetrics());
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }

}
