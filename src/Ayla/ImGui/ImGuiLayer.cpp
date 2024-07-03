//
// Created by sparky on 4/24/2024.
//
#include "ImGuiLayer.h"
#include "Ayla/Core/Application.h"

#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"



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
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();

        Windows::Window& appWindow = Core::Application::getApplication().getWindow();
        io.DisplaySize = ImVec2(appWindow.getWidth(), appWindow.getHeight());
        io.DisplayFramebufferScale = ImVec2(appWindow.getDisplayFramebufferScaleX(),appWindow.getDisplayFramebufferScaleY());

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;


        io.KeyMap[ImGuiKey_MouseLeft] = AY_KEY_LEFT_CLICK;
        io.KeyMap[ImGuiKey_MouseMiddle] = AY_KEY_MIDDLE_CLICK;
        io.KeyMap[ImGuiKey_MouseRight] = AY_KEY_RIGHT_CLICK;

        io.KeyMap[ImGuiKey_A] = AY_KEY_A;
        io.KeyMap[ImGuiKey_B] = AY_KEY_B;
        io.KeyMap[ImGuiKey_C] = AY_KEY_C;
        io.KeyMap[ImGuiKey_D] = AY_KEY_D;
        io.KeyMap[ImGuiKey_E] = AY_KEY_E;
        io.KeyMap[ImGuiKey_F] = AY_KEY_F;
        io.KeyMap[ImGuiKey_G] = AY_KEY_G;
        io.KeyMap[ImGuiKey_H] = AY_KEY_H;
        io.KeyMap[ImGuiKey_I] = AY_KEY_I;
        io.KeyMap[ImGuiKey_J] = AY_KEY_J;
        io.KeyMap[ImGuiKey_K] = AY_KEY_K;
        io.KeyMap[ImGuiKey_L] = AY_KEY_L;
        io.KeyMap[ImGuiKey_M] = AY_KEY_M;
        io.KeyMap[ImGuiKey_N] = AY_KEY_N;
        io.KeyMap[ImGuiKey_O] = AY_KEY_O;
        io.KeyMap[ImGuiKey_P] = AY_KEY_P;
        io.KeyMap[ImGuiKey_Q] = AY_KEY_Q;
        io.KeyMap[ImGuiKey_R] = AY_KEY_R;
        io.KeyMap[ImGuiKey_S] = AY_KEY_S;
        io.KeyMap[ImGuiKey_T] = AY_KEY_T;
        io.KeyMap[ImGuiKey_U] = AY_KEY_U;
        io.KeyMap[ImGuiKey_V] = AY_KEY_V;
        io.KeyMap[ImGuiKey_W] = AY_KEY_W;
        io.KeyMap[ImGuiKey_X] = AY_KEY_X;
        io.KeyMap[ImGuiKey_Y] = AY_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = AY_KEY_Z;

        io.KeyMap[ImGuiKey_0] = AY_KEY_0;
        io.KeyMap[ImGuiKey_1] = AY_KEY_1;
        io.KeyMap[ImGuiKey_2] = AY_KEY_2;
        io.KeyMap[ImGuiKey_3] = AY_KEY_3;
        io.KeyMap[ImGuiKey_4] = AY_KEY_4;
        io.KeyMap[ImGuiKey_5] = AY_KEY_5;
        io.KeyMap[ImGuiKey_6] = AY_KEY_6;
        io.KeyMap[ImGuiKey_7] = AY_KEY_7;
        io.KeyMap[ImGuiKey_8] = AY_KEY_8;
        io.KeyMap[ImGuiKey_9] = AY_KEY_9;

        io.KeyMap[ImGuiKey_Tab] = AY_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = AY_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = AY_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = AY_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = AY_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = AY_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = AY_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = AY_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = AY_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = AY_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = AY_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = AY_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = AY_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = AY_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = AY_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_LeftSuper] = AY_KEY_LEFT_SUPER;
        io.KeyMap[ImGuiKey_RightSuper] = AY_KEY_RIGHT_SUPER;
        io.KeyMap[ImGuiKey_LeftCtrl] = AY_KEY_LEFT_CONTROL;
        io.KeyMap[ImGuiKey_RightCtrl] = AY_KEY_RIGHT_CONTROL;
        io.KeyMap[ImGuiKey_LeftAlt] = AY_KEY_LEFT_ALT;
        io.KeyMap[ImGuiKey_RightAlt] = AY_KEY_RIGHT_ALT;
        io.KeyMap[ImGuiKey_LeftShift] = AY_KEY_LEFT_SHIFT;
        io.KeyMap[ImGuiKey_RightShift] = AY_KEY_RIGHT_SHIFT;


        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::onDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::onUpdate() {

        ImGuiIO& io = ImGui::GetIO();

        float time = (float)glfwGetTime();
        io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
        m_time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowIDStackToolWindow();
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::onEvent(Event& event) {
        //AY_LOG("ImGui received an event!");

        ImGuiIO& io = ImGui::GetIO();

        switch(event.getEventType()){
            case MOUSE_CURSOR_MOVED: {
                auto mouseMovedEvent = dynamic_cast<MouseMovedEvent&>(event);
                io.MousePos = ImVec2(mouseMovedEvent.getXPos(), mouseMovedEvent.getYPos());
                break;
            }
            case MOUSE_BUTTON_PRESSED: {
                auto mouseButtonPressEvent = dynamic_cast<MouseButtonPressEvent&>(event);
                io.MouseDown[mouseButtonPressEvent.getButton()] = true;
                break;
            }
            case MOUSE_BUTTON_RELEASED: {
                auto mouseButtonReleaseEvent = dynamic_cast<MouseButtonReleaseEvent&>(event);
                io.MouseDown[mouseButtonReleaseEvent.getButton()] = false;
                break;
            }
            case MOUSE_SCROLLED: {
                auto mouseScrollEvent = dynamic_cast<MouseScrollEvent&>(event);
                io.MouseWheel += mouseScrollEvent.getYOffset();
                break;
            }
            case WINDOW_RESIZE: {
                auto windowResizeEvent = dynamic_cast<WindowResizeEvent&>(event);
                Windows::Window& appWindow = Core::Application::getApplication().getWindow();
                io.DisplaySize = ImVec2(windowResizeEvent.getWidth(), windowResizeEvent.getHeight());
                io.DisplayFramebufferScale = ImVec2(appWindow.getDisplayFramebufferScaleX(),appWindow.getDisplayFramebufferScaleY());
                glViewport(0, 0, windowResizeEvent.getWidth(), windowResizeEvent.getHeight());
                break;
            }
            case KEY_PRESSED: {
                auto keyPressedEvent = dynamic_cast<KeyPressedEvent&>(event);
                io.KeysDown[keyPressedEvent.getKeycode()] = true;

                io.KeyCtrl = io.KeysDown[AY_KEY_LEFT_CONTROL] || io.KeysDown[AY_KEY_RIGHT_CONTROL];
                io.KeyAlt = io.KeysDown[AY_KEY_LEFT_ALT] || io.KeysDown[AY_KEY_RIGHT_ALT];
                io.KeyShift = io.KeysDown[AY_KEY_LEFT_SHIFT] || io.KeysDown[AY_KEY_RIGHT_SHIFT];
                io.KeySuper = io.KeysDown[AY_KEY_LEFT_SUPER] || io.KeysDown[AY_KEY_RIGHT_SUPER];

                if (io.KeysDown[AY_KEY_LEFT_CONTROL] && io.KeysDown[AY_KEY_C]){
                    ImGui::LogToClipboard();
                } else {
                    ImGui::LogFinish();
                }

                break;
            }
            case KEY_RELEASED: {
                auto keyReleasedEvent = dynamic_cast<KeyReleasedEvent&>(event);
                io.KeysDown[keyReleasedEvent.getKeycode()] = false;

                io.KeyCtrl = io.KeysDown[AY_KEY_LEFT_CONTROL] || io.KeysDown[AY_KEY_RIGHT_CONTROL];
                io.KeyAlt = io.KeysDown[AY_KEY_LEFT_ALT] || io.KeysDown[AY_KEY_RIGHT_ALT];
                io.KeyShift = io.KeysDown[AY_KEY_LEFT_SHIFT] || io.KeysDown[AY_KEY_RIGHT_SHIFT];
                io.KeySuper = io.KeysDown[AY_KEY_LEFT_SUPER] || io.KeysDown[AY_KEY_RIGHT_SUPER];

                if (io.KeysDown[AY_KEY_LEFT_CONTROL] && io.KeysDown[AY_KEY_C]){
                    ImGui::LogToClipboard();
                } else {
                    ImGui::LogFinish();
                }
                break;
            }
            case KEY_TYPED: {
                auto keyTypedEvent = dynamic_cast<KeyTypedEvent&>(event);
                unsigned int keycode = keyTypedEvent.getKeycode();
                if (keycode > 0 && keycode < 0x10000){
                    io.AddInputCharacter(keyTypedEvent.getKeycode());
                }
            }
            default: {
                break;
            }
        }


        if (event.isInCategory(ApplicationCategory)) {event.setIsHandled(true);}
    }

    EventCategory ImGuiLayer::getAcceptingEventFlags() {
        return static_cast<EventCategory>(ApplicationCategory | InputCategory);
    }

}