//
// Created by sparky on 4/24/2024.
//
#include "ImGuiLayer.h"
#include "Solas/Core/Application.h"

#include "Solas/Renderer/Platform/OpenGL/imgui_impl_opengl3.h"
#include "Solas/Window/Platform/Generic/imgui_impl_glfw.h"


#include "Solas/Events/EventTypes/ApplicationEvent.h"
#include "Solas/Events/EventTypes/MouseEvent.h"
#include "Solas/Events/EventTypes/KeyEvent.h"

#include "Solas/Input/Keycodes.h"

//TEMPORARY
#include "GLFW/glfw3.h"

using namespace Solas::Input;

namespace Solas::GUI {

    float ImGuiLayer::m_Time = 0.0f;

    ImGuiLayer::ImGuiLayer()
    {
        AY_TRACE("[Solas] ImGuiLayer: Initializing ImGui Layer");
        this->AttachLayer();
        m_DebugName = "ImGui Layer";
    }


    ImGuiLayer::~ImGuiLayer()
    {
        AY_TRACE("[Solas] ImGuiLayer: Destroying ImGui Layer");
        OnDetach();
        this->DetachLayer();
    }


    void ImGuiLayer::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();

        Windows::Window const& appWindow = Core::Application::Get().GetWindow();
        io.DisplaySize = ImVec2((float)appWindow.GetWidth(), (float)appWindow.GetHeight());
        io.DisplayFramebufferScale = ImVec2(appWindow.GetDisplayFramebufferScaleX(),appWindow.GetDisplayFramebufferScaleY());

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

        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(appWindow.GetNativeWindow()), true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }


    void ImGuiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }


    void ImGuiLayer::OnImGuiRender()
    {
        const ImGuiIO& io = ImGui::GetIO();

        static bool showAnotherWindow = false;
        static bool showDemoNumbersWindow = false;
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        if (showDemoNumbersWindow)
        {
            static float sliderFloat = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!", &showDemoNumbersWindow);                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            //ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &showAnotherWindow);

            ImGui::SliderFloat("float", &sliderFloat, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
            {
                counter++;
            }

            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (showAnotherWindow)
        {
            ImGui::Begin("Another Window", &showAnotherWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
            {
                showAnotherWindow = false;
            }
            ImGui::End();
        }
    }


    void ImGuiLayer::Begin()
    {
        ImGuiIO& io = ImGui::GetIO();
        float time = static_cast<float>(glfwGetTime());
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
        m_Time = time;

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
    }


    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupCurrentContext);
        }
    }


    EEventCategory ImGuiLayer::GetAcceptingEventFlags()
    {
        return None;
    }

}