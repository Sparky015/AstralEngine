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

    ImGuiLayer::ImGuiLayer() {
        AY_TRACE("ImGuiLayer: Initializing ImGui Layer");
        this->attachLayer();
        m_debugName = "ImGui Layer";
    }

    ImGuiLayer::~ImGuiLayer() {
        this->detachLayer();
    }

    void ImGuiLayer::onAttach() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGuiIO& io = ImGui::GetIO();

        Windows::Window& appWindow = Core::Application::getWindow();
        io.DisplaySize = ImVec2(appWindow.getWidth(), appWindow.getHeight());
        io.DisplayFramebufferScale = ImVec2(appWindow.getDisplayFramebufferScaleX(),appWindow.getDisplayFramebufferScaleY());

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

        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Core::Application::getWindow().getNativeWindow()), true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::onDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::onUpdate() {

        ImGuiIO& io = ImGui::GetIO();

        float time = (float)glfwGetTime();
        io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
        m_time = time;

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        static bool showStackTool = true;
        if (showStackTool) ImGui::ShowIDStackToolWindow(&showStackTool);
        if (show) ImGui::ShowDemoWindow(&show);

        static bool show_another_window = true;
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);

        }

    }

    void ImGuiLayer::onEvent(Event& event) {
        ///AY_LOG("ImGui received an event!");

//        ImGuiIO& io = ImGui::GetIO();
//
////        TODO: Put each case into a separate function
//        switch(event.getEventType()){
//            case MOUSE_CURSOR_MOVED: {
//                auto mouseMovedEvent = dynamic_cast<MouseMovedEvent&>(event);
//                io.MousePos = ImVec2((float)mouseMovedEvent.getXPos(), (float)mouseMovedEvent.getYPos());
//                break;
//            }
//            case MOUSE_BUTTON_PRESSED: {
//                auto mouseButtonPressEvent = dynamic_cast<MouseButtonPressEvent&>(event);
//                io.MouseDown[mouseButtonPressEvent.getButton()] = true;
//                break;
//            }
//            case MOUSE_BUTTON_RELEASED: {
//                auto mouseButtonReleaseEvent = dynamic_cast<MouseButtonReleaseEvent&>(event);
//                io.MouseDown[mouseButtonReleaseEvent.getButton()] = false;
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
//                io.DisplaySize = ImVec2((float)windowResizeEvent.getWidth(), (float)windowResizeEvent.getHeight());
//                io.DisplayFramebufferScale = ImVec2(appWindow.getDisplayFramebufferScaleX(),appWindow.getDisplayFramebufferScaleY());
//                glViewport(0, 0, windowResizeEvent.getWidth(), windowResizeEvent.getHeight());
//                break;
//            }
//            case KEY_PRESSED: {
//                auto keyPressedEvent = dynamic_cast<KeyPressedEvent&>(event);
//                io.KeysDown[keyPressedEvent.getKeycode()] = true;
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
//                io.KeysDown[keyReleasedEvent.getKeycode()] = false;
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
//                unsigned int keycode = keyTypedEvent.getKeycode();
//                if (keycode > 0 && keycode < 0x10000){
//                    io.AddInputCharacter(keyTypedEvent.getKeycode());
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
        if (event.isInCategory(ApplicationCategory)) {event.setIsHandled(true);}
    }

    EventCategory ImGuiLayer::getAcceptingEventFlags() {
        return static_cast<EventCategory>(ApplicationCategory | InputCategory);
    }

}