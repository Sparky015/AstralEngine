//
// Created by Andrew Fagan on 11/1/24.
//

#include "ImGuiManager.h"

#include "ImGuiDependencies/imgui_impl_vulkan.h"
#include "ImGuiDependencies/imgui_impl_glfw.h"
#include "Renderer/SceneRenderer.h"
#include "Window/WindowManager.h"

#include "ImPlot/implot.h"
#include "imgui/imgui.h"
#include "GLFW/glfw3.h"

namespace Astral {

    void ImGuiManager::Init()
    {
        PROFILE_SCOPE("ImGuiManager::Init");
        AE_TRACE("Initializing Debug Manager!")

        InitImGui();

        for (const std::string& configFilePath : m_LoadedConfigs)
        {
            ImGui::LoadIniSettingsFromDisk(configFilePath.c_str());
        }
    }


    void ImGuiManager::Shutdown()
    {
        PROFILE_SCOPE("ImGui Manager Shutdown");
        AE_TRACE("Shutting down Debug Manager!")

        ShutdownImGui();
    }


    float ImGuiManager::m_Time = 0.0f;
    void ImGuiManager::BeginFrame()
    {
        PROFILE_SCOPE("ImGuiManager::BeginFrame");
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
        PROFILE_SCOPE("ImGuiManager::EndFrame");
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
        m_LoadedConfigs.push_back(filePath.data());
    }


    ImGuiManager::ImGuiManager()
    {
        AE_TRACE("Constructing Debug System!")
    }


    ImGuiManager::~ImGuiManager()
    {
        AE_TRACE("Destroying Debug System!")
    }


    void ImGuiManager::InitImGui() const
    {
        PROFILE_SCOPE("ImGuiManager::InitImGui");

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
        PROFILE_SCOPE("ImGuiManager::ShutdownImGui");

        ImGui_ImplGlfw_Shutdown();

        ImPlot::DestroyContext();
        ImGui::DestroyContext();
    }

}
