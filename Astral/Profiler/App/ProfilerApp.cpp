/**
* @file ProfilerApp.cpp
* @author Andrew Fagan
* @date 3/29/2026
*/

#include "ProfilerApp.h"

#include "Core/Engine.h"
#include "Core/Utilities/Asserts.h"
#include "Core/Utilities/Loggers.h"
#include "Debug/ImGui/ImGuiManager.h"
#include "Panels/CallTreePanel.h"
#include "Views/GraphViews.h"
#include "Trace/ScopeTraceImporter.h"
#include "Trace/Panels/TraceViewerPanel.h"
#include "Window/WindowManager.h"

#include "ImPlot/implot.h"
#include "imgui.h"

namespace Astral {

    ProfilerApp* ProfilerApp::m_Instance = nullptr;

    ProfilerApp::ProfilerApp()
    {
        PROFILE_SCOPE("ProfilerApp::ProfilerApp");
        ASSERT(m_Instance == nullptr, "ProfilerApp has already been constructed!");
        m_Instance = this;
    }

    ProfilerApp::~ProfilerApp()
    {
    }


    ProfilerApp& ProfilerApp::Get()
    {
        return *m_Instance;
    }


    void ProfilerApp::Init()
    {
        ImGuiManager& imguiManager = Engine::Get().GetImGuiManager();
        imguiManager.EnableViewportDockSpace();
        imguiManager.LoadImGuiConfigFile(std::string(ASTRAL_PROFILER_APP_SOURCE_DIR) + "/imgui-profiler-config.ini");

        Astral::Window& window = Astral::Engine::Get().GetWindowManager().GetWindow();
        window.SetWindowName("Astral Profiler");
        window.SetWindowDimensions(1600, 900);

        m_RenderImGuiListener.StartListening();

        m_TraceViewDataCache.InitializeCache("/Users/sparky/Downloads/20-50-19-Profiler.json");
    }


    void ProfilerApp::Shutdown()
    {
        m_RenderImGuiListener.StopListening();
    }


    void ProfilerApp::LoadMemoryScene(std::string filePath)
    {
        m_SceneDataCache.InitializeCache(filePath);
        ImPlot::SetNextAxesToFit();
    }


    ProfilerSceneDataCache& ProfilerApp::GetSceneDataCache()
    {
        return m_SceneDataCache;
    }


    TraceViewDataCache& ProfilerApp::GetTraceCache()
    {
        return m_TraceViewDataCache;
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
                TraceViewerPanel::Show();
                m_GraphsView.ShowGlobalGraphView(m_SceneDataCache.GetSceneMetricsStorage());
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Allocator"))
            {
                TraceViewerPanel::Show();
                m_GraphsView.ShowAllocatorGraphsView(m_SceneDataCache.GetSceneMetricsStorage());
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Region"))
            {
                TraceViewerPanel::Show();
                m_GraphsView.ShowRegionGraphsView(m_SceneDataCache.GetSceneMetricsStorage());
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Thread"))
            {
                TraceViewerPanel::Show();
                m_GraphsView.ShowThreadGraphsView(m_SceneDataCache.GetSceneMetricsStorage());
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Call Tree"))
            {
                CallTreePanel::Show();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }

}
