//
// Created by Andrew Fagan on 11/1/24.
//

#include "ImGuiManager.h"

#include "Components/AssetStatsComponents.h"
#include "Components/BuildConfigComponents.h"
#include "Components/ECSComponents.h"
#include "Components/RendererComponents.h"
#include "Components/SystemInfoComponents.h"
#include "Components/WindowComponents.h"
#include "Debug/ImGui/Components/InputStateComponents.h"
#include "Debug/MemoryTracking/Visualization/Components/LoadFileComponent.h"
#include "Components/MemoryComponents.h"
#include "Core/Engine.h"
#include "Debug/ImGui/Components/EngineComponents.h"
#include "ImGuiDependencies/imgui_impl_vulkan.h"
#include "Input/Keycodes.h"
#include "Renderer/SceneRenderer.h"
#include "Window/WindowManager.h"

#include "ImGuiDependencies/imgui_impl_glfw.h"
#include "ImPlot/implot.h"
#include "imgui/imgui.h"
#include "GLFW/glfw3.h"

namespace Astral {

    void ImGuiManager::Init()
    {
        PROFILE_SCOPE("ImGui Manager Initialization");
        TRACE("Initializing Debug Manager!")

        InitImGui();

        LoadImGuiConfigFile(std::string(ASTRAL_RUNTIME_DIR) + "Debug/ImGui/imgui-config.ini");

        m_RenderImGuiListener.StartListening();
        m_KeyPressedListener.StartListening();
    }


    void ImGuiManager::Shutdown()
    {
        PROFILE_SCOPE("ImGui Manager Shutdown");
        TRACE("Shutting down Debug Manager!")
        m_KeyPressedListener.StopListening();
        m_RenderImGuiListener.StopListening();

        ShutdownImGui();
    }


    void ImGuiManager::RenderImGui()
    {
        PROFILE_SCOPE("ImGuiManager::RenderImGui");

        static bool showDemoWindow = false;
        if (showDemoWindow)
        {
            ImGui::ShowDemoWindow(&showDemoWindow);
        }

        if (m_ShowDebugMenu)
        {
            ImGui::Begin("Debug Menu", &m_ShowDebugMenu);

            if (ImGui::TreeNodeEx("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
            {
                FPSComponent();
                FrameTimeComponent();
                DrawCallsPerFrameComponent();
                RendererViewportSizeComponent();

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
                Astral::LoadMemoryProfileButtonComponent();

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Window"))
            {
                WindowDimensionsComponent();
                WindowFramebufferScaleComponent();
                WindowAPIInfoComponent();
                WindowVsyncStateComponent();
                VsyncToggleComponent();
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


    float ImGuiManager::m_Time = 0.0f;
    void ImGuiManager::BeginFrame()
    {
        PROFILE_SCOPE("ImGui Begin Frame");
        ImGuiIO& io = ImGui::GetIO();
        float time = static_cast<float>(glfwGetTime());
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        ImGui_ImplGlfw_NewFrame();

        if (Astral::SceneRenderer::GetRendererAPIBackend() == Astral::API::Vulkan)
        {
            ImGui_ImplVulkan_NewFrame();
        }

        ImGui::NewFrame();

        if (m_ShowViewportDockSpace)
        {
            ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
        }
    }


    void ImGuiManager::EndFrame()
    {
        PROFILE_SCOPE("ImGui End Frame");
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Render();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }


    void ImGuiManager::LoadImGuiConfigFile(const std::string_view& filePath)
    {
        PROFILE_SCOPE("ImGuiManager::LoadImGuiConfigFile")
        ImGui::LoadIniSettingsFromDisk(filePath.data());
    }


    ImGuiManager::ImGuiManager() : m_KeyPressedListener([this](KeyPressedEvent e){ this->OnKeyPress(e);})
    {
        TRACE("Constructing Debug System!")
    }


    ImGuiManager::~ImGuiManager()
    {
        TRACE("Destroying Debug System!")
    }


    void ImGuiManager::InitImGui() const
    {
        PROFILE_SCOPE("Initialize ImGui");

        ImGui::CreateContext();
        ImPlot::CreateContext();

        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();

        Window& window = Astral::Engine::Get().GetWindowManager().GetWindow();
        io.DisplaySize = ImVec2((float)window.GetWidth(), (float)window.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1,1);

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        if (Astral::SceneRenderer::GetRendererAPIBackend() == Astral::API::Vulkan)
        {
            ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)Astral::Engine::Get().GetWindowManager().GetWindow().GetNativeWindow(), true);
        }

    }


    void ImGuiManager::ShutdownImGui() const
    {
        PROFILE_SCOPE("ImGui Manager Shutdown");

        ImGui_ImplGlfw_Shutdown();

        ImPlot::DestroyContext();
        ImGui::DestroyContext();
    }


    void ImGuiManager::OnKeyPress(KeyPressedEvent keyPressedEvent)
    {

    }

}
