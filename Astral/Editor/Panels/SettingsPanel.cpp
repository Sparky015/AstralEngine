/**
* @file SettingsPanel.cpp
* @author Andrew Fagan
* @date 4/13/26
*/

#include "SettingsPanel.h"

#include "Profiler/Instrumentation/ScopeProfiler.h"
#include "Editor/Components/Settings/AssetStatsComponents.h"
#include "Editor/Components/Settings/BuildConfigComponents.h"
#include "Editor/Components/Settings/ECSComponents.h"
#include "Editor/Components/Settings/RendererComponents.h"
#include "Editor/Components/Settings/SystemInfoComponents.h"
#include "Editor/Components/Settings/WindowComponents.h"
#include "Editor/Components/Settings/InputStateComponents.h"
#include "Editor/Components/Settings/MemoryComponents.h"
#include "Editor/Components/Settings/EngineComponents.h"

#include "imgui/imgui.h"

namespace Astral {

    void SettingsPanel::Show()
    {
        PROFILE_SCOPE("SettingsPanel::Show");

        ImGui::Begin("Debug Menu");

        if (ImGui::TreeNodeEx("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
        {
            FPSComponent();
            FrameTimeComponent();
            DrawCallsPerFrameComponent();
            RendererViewportSizeComponent();

            ImGui::Spacing();

            VsyncToggleComponent();
            FrustumCullingToggleComponent();
            RendererTypeSelector();
            RendererDebugViewComponent();

            ImGui::Spacing();

            RendererAPIComponent();
            RendererAPIValidationStatus();


            ImGui::Spacing();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Assets"))
        {
            ShowNumberOfAssetsLoaded();
            ShowTotalNumberOfAssetsLoaded();
            ShowTotalNumberOfAssetsUnloaded();
            ShowNumberOfAssetsLoadedByType();
            ImGui::Spacing();
            ImGui::TreePop();
        }


        if (ImGui::TreeNode("ECS"))
        {
            EntityCountComponent();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Memory"))
        {
            ImGui::Spacing();

            PeakMemoryUsage();
            GlobalMemoryUsage();
            GlobalActiveAllocations();
            GlobalTotalAllocationsMade();
            AllocationsInCurrentFrame();

            ImGui::Spacing();
            ImGui::Spacing();

            if (ImGui::TreeNode("Metrics by Allocator"))
            {
                MemoryUsageByAllocator();
                PeakMemoryUsageByAllocator();
                TotalAllocationsMadeByAllocator();
                ActiveAllocationsByAllocator();
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Metrics by Region"))
            {
                MemoryUsageByRegion();
                PeakMemoryUsageByRegion();
                TotalAllocationsMadeByRegion();
                ActiveAllocationsByRegion();
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Metrics by Thread"))
            {
                MemoryUsageByThread();
                PeakMemoryUsageByThread();
                TotalAllocationsMadeByThread();
                ActiveAllocationsByThread();
                ImGui::TreePop();
            }

            ImGui::Spacing();
            ImGui::Spacing();

            ManageMemoryProfilingScene();
            OpenProfilerComponent();

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Window"))
        {
            WindowDimensionsComponent();
            WindowFramebufferScaleComponent();
            WindowAPIInfoComponent();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Input State"))
        {
            IsTrackingInputsComponent();
            ImGui::Spacing();
            CompleteInputStateComponent();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Build Configuration"))
        {
            BuildProfileComponent();
            CompilerInfoComponents();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("System Info"))
        {
            SystemGPUNameComponent();
            SystemCPUNameComponent();

            ImGui::Spacing();
            CPUCoreInfoComponent();

            ImGui::Spacing();
            if (ImGui::TreeNode("CPU Cache Info"))
            {
                CPUCacheSizeComponent();
                CPUCacheLineComponent();
                ImGui::TreePop();
            }

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Other"))
        {
            EngineTimeComponent();
            ImGui::TreePop();
        }

        ImGui::End();
    }

}