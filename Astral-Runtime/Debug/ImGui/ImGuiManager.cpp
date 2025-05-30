//
// Created by Andrew Fagan on 11/1/24.
//

#include "ImGuiManager.h"

#include "Components/BuildConfigComponents.h"
#include "Components/ECSComponents.h"
#include "Components/RendererComponents.h"
#include "Components/SystemInfoComponents.h"
#include "Components/WindowComponents.h"
#include "Debug/ImGui/Components/InputStateComponents.h"
#include "Debug/MemoryTracking/Visualization/Components/LoadFileComponent.h"
#include "ImPlot/implot.h"
#include "imgui/imgui.h"


#ifdef ASTRAL_OPENGL_AVAILABLE
#include "ImGuiDependencies/imgui_impl_opengl3.h"
#endif

#include "ImGuiDependencies/imgui_impl_glfw.h"




#include "Components/MemoryComponents.h"
#include "Debug/ImGui/Components/EngineComponents.h"
#include "ImGuiDependencies/imgui_impl_vulkan.h"
#include "Input/Keycodes.h"
#include "Renderer/SceneRenderer.h"
#include "Window/Platform/Generic/GenericWindow.h"
#include "Window/WindowManager.h"


namespace Debug {

    ImGuiManager& g_ImGuiManager = ImGuiManager::Get();

    ImGuiManager& ImGuiManager::Get()
    {
        static ImGuiManager m_Instance = ImGuiManager();
        return m_Instance;
    }


    void ImGuiManager::Init()
    {
        PROFILE_SCOPE("ImGui Manager Initialization");
        TRACE("Initializing Debug Manager!")

        InitImGui();

        ImGui::GetIO().IniFilename = "imgui-config.ini";
        ImGui::LoadIniSettingsFromDisk("imgui-config.ini");



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
        PROFILE_SCOPE("Render Debug Menu ImGui");

        static bool showDemoWindow = true;
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

                ImGui::Spacing();
                RendererAPIComponent();

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
                Core::LoadMemoryProfileButtonComponent();

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

        if (Astral::SceneRenderer::GetRendererAPIBackend() == Astral::API::OpenGL)
        {
            ImGui_ImplOpenGL3_NewFrame();
        }
        else if (Astral::SceneRenderer::GetRendererAPIBackend() == Astral::API::Vulkan)
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

        Window::WindowManager& windowManager = Window::g_WindowManager;
        io.DisplaySize = ImVec2((float)windowManager.GetWidth(), (float)windowManager.GetHeight());
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
            ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)Window::g_WindowManager.GetWindow().GetNativeWindow(), true);
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
        if (keyPressedEvent.keycode == KEY_D)
        {
            m_ShowDebugMenu = !m_ShowDebugMenu;
        }
    }

}
