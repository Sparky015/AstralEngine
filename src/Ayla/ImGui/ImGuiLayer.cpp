//
// Created by sparky on 4/24/2024.
//
#include "ImGuiLayer.h"
#include "Ayla/Core/Application.h"

#include "Ayla/Renderer/Platform/OpenGL/imgui_impl_opengl3.h"
#include "Ayla/Window/Platform/Generic/imgui_impl_glfw.h"


#include "Ayla/Events/ApplicationEvent.h"
#include "Ayla/Events/MouseEvent.h"
#include "Ayla/Events/KeyEvent.h"

#include "Ayla/Input/Keycodes.h"

//TEMPORARY
#include "GLFW/glfw3.h"

using namespace Ayla::Input;

namespace Ayla::GUI {

    float ImGuiLayer::m_Time = 0.0f;

    ImGuiLayer::ImGuiLayer()
    {
        AY_TRACE("[Sholas] ImGuiLayer: Initializing ImGui Layer");
        this->AttachLayer();
        m_DebugName = "ImGui Layer";
    }


    ImGuiLayer::~ImGuiLayer()
    {
        AY_TRACE("[Sholas] ImGuiLayer: Destroying ImGui Layer");
        this->DetachLayer();
    }


    void ImGuiLayer::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();

        Windows::Window& appWindow = Core::Application::GetWindow();
        io.DisplaySize = ImVec2(appWindow.GetWidth(), appWindow.GetHeight());
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

        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Core::Application::GetWindow().GetNativeWindow()), true);
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
        ImGuiIO& io = ImGui::GetIO();
        static bool showDemoWindow = true;
        static bool showStackTool = true;
        if (showStackTool)
        {
            ImGui::ShowIDStackToolWindow(&showStackTool);
        }
        if (showDemoWindow)
        {
            ImGui::ShowDemoWindow(&showDemoWindow);
        }

        static bool showAnotherWindow = true;
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float sliderFloat = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state
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
        return static_cast<EEventCategory>(APPLICATION_CATEGORY | INPUT_CATEGORY);
    }


    void ImGuiLayer::OnEvent(IEvent& event)
    {
        ///AY_LOG("ImGui received an event!");

//        ImGuiIO& io = ImGui::GetIO();
//
////        TODO: Put each case into a separate function
//        switch(event.GetEventType()){
//            case MOUSE_CURSOR_MOVED: {
//                auto mouseMovedEvent = dynamic_cast<MouseMovedEvent&>(event);
//                io.MousePos = ImVec2((float)mouseMovedEvent.GetXPos(), (float)mouseMovedEvent.GetYPos());
//                break;
//            }
//            case MOUSE_BUTTON_PRESSED: {
//                auto mouseButtonPressEvent = dynamic_cast<MouseButtonPressEvent&>(event);
//                io.MouseDown[mouseButtonPressEvent.GetButton()] = true;
//                break;
//            }
//            case MOUSE_BUTTON_RELEASED: {
//                auto mouseButtonReleaseEvent = dynamic_cast<MouseButtonReleaseEvent&>(event);
//                io.MouseDown[mouseButtonReleaseEvent.GetButton()] = false;
//                break;
//            }
//            case MOUSE_SCROLLED: {
//                auto mouseScrollEvent = dynamic_cast<MouseScrollEvent&>(event);
//                io.MouseWheel += (float)mouseScrollEvent.getYOffset();
//                break;
//            }
//            case WINDOW_RESIZE: {
//                auto windowResizeEvent = dynamic_cast<WindowResizeEvent&>(event);
//                Windows::Window& appWindow = Core::Application::getWindow();
//                io.DisplaySize = ImVec2((float)windowResizeEvent.GetWidth(), (float)windowResizeEvent.GetHeight());
//                io.DisplayFramebufferScale = ImVec2(appWindow.GetDisplayFramebufferScaleX(),appWindow.GetDisplayFramebufferScaleY());
//                glViewport(0, 0, windowResizeEvent.GetWidth(), windowResizeEvent.GetHeight());
//                break;
//            }
//            case KEY_PRESSED: {
//                auto keyPressedEvent = dynamic_cast<KeyPressedEvent&>(event);
//                io.KeysDown[keyPressedEvent.GetKeycode()] = true;
//
//                io.KeyCtrl = io.KeysDown[AY_KEY_LEFT_CONTROL] || io.KeysDown[AY_KEY_RIGHT_CONTROL];
//                io.KeyAlt = io.KeysDown[AY_KEY_LEFT_ALT] || io.KeysDown[AY_KEY_RIGHT_ALT];
//                io.KeyShift = io.KeysDown[AY_KEY_LEFT_SHIFT] || io.KeysDown[AY_KEY_RIGHT_SHIFT];
//                io.KeySuper = io.KeysDown[AY_KEY_LEFT_SUPER] || io.KeysDown[AY_KEY_RIGHT_SUPER];
//
//                if (io.KeysDown[AY_KEY_LEFT_CONTROL] && io.KeysDown[AY_KEY_C]){
//                    ImGui::LogToClipboard();
//                } else {
//                    ImGui::LogFinish();
//                }
//
//                break;
//            }
//            case KEY_RELEASED: {
//                auto keyReleasedEvent = dynamic_cast<KeyReleasedEvent&>(event);
//                io.KeysDown[keyReleasedEvent.GetKeycode()] = false;
//
//                io.KeyCtrl = io.KeysDown[AY_KEY_LEFT_CONTROL] || io.KeysDown[AY_KEY_RIGHT_CONTROL];
//                io.KeyAlt = io.KeysDown[AY_KEY_LEFT_ALT] || io.KeysDown[AY_KEY_RIGHT_ALT];
//                io.KeyShift = io.KeysDown[AY_KEY_LEFT_SHIFT] || io.KeysDown[AY_KEY_RIGHT_SHIFT];
//                io.KeySuper = io.KeysDown[AY_KEY_LEFT_SUPER] || io.KeysDown[AY_KEY_RIGHT_SUPER];
//
//                if (io.KeysDown[AY_KEY_LEFT_CONTROL] && io.KeysDown[AY_KEY_C]){
//                    ImGui::LogToClipboard();
//                } else {
//                    ImGui::LogFinish();
//                }
//                break;
//            }
//            case KEY_TYPED: {
//                auto keyTypedEvent = dynamic_cast<KeyTypedEvent&>(event);
//                unsigned int keycode = keyTypedEvent.GetKeycode();
//                if (keycode > 0 && keycode < 0x10000){
//                    io.AddInputCharacter(keyTypedEvent.GetKeycode());
//                }
//                break;
//            }
//            case WINDOW_GAINED_FOCUS: {
//                //io.AppFocusLost = false;
//                break;
//            }
//            case WINDOW_LOST_FOCUS: {
//                //io.AppFocusLost = true;
//                break;
//            }
//            default: {
//                break;
//            }
//        }
//
//
        if (event.IsInCategory(APPLICATION_CATEGORY))
        {
            event.SetIsHandled(true);
        }
    }


}