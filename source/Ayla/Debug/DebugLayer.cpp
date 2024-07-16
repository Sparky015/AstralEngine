//
// Created by Andrew Fagan on 6/25/24.
//

#include "DebugLayer.h"

#include "Ayla/Core/Application.h"
#include "Ayla/Core/Time/Clock.h"
#include "Ayla/Input/InputState.h"
#include "Ayla/Input/Keycodes.h"
#include "Ayla/Window/Window.h"
#ifdef AYLA_GENERIC_WINDOW
#include "Ayla/Window/Platform/Generic/GenericWindow.h"
#endif

#include "imgui.h"



namespace Ayla::Core::Debug
{
    bool DebugLayer::m_ShowImGuiDemoWindow = false;
    bool DebugLayer::m_ShowDebugMenu = true;

    DebugLayer::DebugLayer()
    {
        AY_TRACE("[Sholas] DebugLayer: Initializing Debug Layer");
        this->AttachLayer();
        m_DebugName = "Debug Layer";
    }


    DebugLayer::~DebugLayer()
    {
        AY_TRACE("[Sholas] DebugLayer: Destroying Debug Layer");
        OnDetach();
        this->DetachLayer();
    }


    void DebugLayer::OnUpdate()
    {
#ifndef TURN_OFF_DEBUG_MENU
        using namespace Ayla::Input;

        static int64 accumulatedTime;
        accumulatedTime += Time::Clock::Get().GetDeltaTime();
        const Input::SInputState& inputState = Input::SInputState::Get();

        if (inputState.IsKeyDown(AY_KEY_R) && !inputState.IsKeyRepeating(AY_KEY_R))
        {
            if (accumulatedTime <= 300){return;}
            accumulatedTime = 0;
            AY_TRACE("Resetting stopwatch.");
            Time::Clock::Get().ResetStopwatch();
        }
        if (inputState.IsKeyDown(AY_KEY_D))
        {
            m_ShowDebugMenu = true;
        }
#endif
    }


    EEventCategory DebugLayer::GetAcceptingEventFlags()
    {
        return None;
    }


    void DebugLayer::OnImGuiRender()
    {
#ifndef TURN_OFF_DEBUG_MENU
        using namespace Input;
        const Input::SInputState& inputState = SInputState::Get();
        const ImGuiIO& io = ImGui::GetIO();
        const static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;

        if (m_ShowImGuiDemoWindow)
        {
            ImGui::ShowDemoWindow(&m_ShowImGuiDemoWindow);
        }

        if (m_ShowDebugMenu)
        {
            //ImGui::SetNextWindowSize(ImVec2(600, 400));
            ImGui::Begin("Debug Menu", &m_ShowDebugMenu);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::Text("Stopwatch time: %.3f seconds", (float)Time::Clock::Get().GetStopwatchTime() * .001);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::SetNextItemOpen(false, ImGuiCond_Once);
            if (ImGui::TreeNode("Input State"))
            {
                if (ImGui::TreeNode("Mouse"))
                {
                    ImGui::Text("Mouse Position: (%.3f, %.3f)", inputState.MousePositionX(), inputState.MousePositionY());
                    ImGui::Text("Left Click: %s", (inputState.IsKeyDown(AY_KEY_LEFT_CLICK)) ? "Down" : "Up");
                    ImGui::Text("Middle Click: %s", (inputState.IsKeyDown(AY_KEY_MIDDLE_CLICK)) ? "Down" : "Up");
                    ImGui::Text("Right Click: %s", (inputState.IsKeyDown(AY_KEY_RIGHT_CLICK)) ? "Down" : "Up");
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("A-Z")) // TODO: maybe make a make/fill table function
                {
                    if (ImGui::BeginTable("A-Z Keys Table", 4, flags))
                    {
                        for (uint8 keycode = AY_KEY_A; keycode != AY_KEY_Z + 1; keycode++)
                        {
                            ImGui::TableNextColumn();
                            ImGui::Text("%s : %s", inputState.GetKeyName(keycode).c_str(),
                                        (inputState.IsKeyDown(keycode) == true) ? "Down" : "Up");
                        }
                        ImGui::EndTable();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("0-9"))
                {
                    if (ImGui::BeginTable("0-9 Keys Table", 5, flags))
                    {
                        for (uint8 keycode = AY_KEY_0; keycode != AY_KEY_9 + 1; keycode++)
                        {
                            ImGui::TableNextColumn();
                            ImGui::Text("%s : %s", inputState.GetKeyName(keycode).c_str(),
                                        (inputState.IsKeyDown(keycode) == true) ? "Down" : "Up");
                        }
                        ImGui::EndTable();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Special Keys"))
                {
                    if (ImGui::BeginTable("Special Keys Table", 4, flags))
                    {
                        for (uint8 keycode = AY_KEY_SPACE; keycode != AY_KEY_RIGHT_SUPER + 1; keycode++)
                        {
                            ImGui::TableNextColumn();
                            ImGui::Text("%s : %s", inputState.GetKeyName(keycode).c_str(),
                                        (inputState.IsKeyDown(keycode) == true) ? "Down" : "Up");
                        }
                        ImGui::EndTable();
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Other"))
                {
                    if (ImGui::TreeNode("Keypad Keys"))
                    {
                        if (ImGui::BeginTable("Keypad Keys Table", 3, flags))
                        {
                            for (uint8 keycode = AY_KEY_KP_0; keycode != AY_KEY_KP_EQUAL + 1; keycode++)
                            {
                                ImGui::TableNextColumn();
                                ImGui::Text("%s : %s", inputState.GetKeyName(keycode).c_str(),
                                            (inputState.IsKeyDown(keycode) == true) ? "Down" : "Up");
                            }
                            ImGui::EndTable();
                        }
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode("Function Keys"))
                    {
                        if (ImGui::BeginTable("Function Keys Table", 4, flags))
                        {
                            for (uint8 keycode = AY_KEY_F1; keycode != AY_KEY_F24 + 1; keycode++)
                            {
                                ImGui::TableNextColumn();
                                ImGui::Text("%s : %s", inputState.GetKeyName(keycode).c_str(),
                                            (inputState.IsKeyDown(keycode) == true) ? "Down" : "Up");
                            }
                            ImGui::EndTable();
                        }
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }

            ImGui::SetNextItemOpen(false, ImGuiCond_Once);
            if (ImGui::TreeNode("Window Properties"))
            {
                Windows::Window& window = Application::GetWindow();

                ImGui::Text("Framebuffer: (%.2f, %.2f)", window.GetDisplayFramebufferScaleX(), window.GetDisplayFramebufferScaleY());
                ImGui::Text("Window Dimension: %d x %d", window.GetWidth(), window.GetHeight());

                static bool isVsyncEnabled = true;
                if (ImGui::Checkbox("VSync", &isVsyncEnabled))
                {
                    if (isVsyncEnabled)
                    {
                        AY_LOG("Enabled VSync!");
                        window.EnableVSync();
                    }
                    else
                    {
                        AY_LOG("Disabled VSync!");
                        window.DisableVSync();
                    }

                };

                ImGui::TreePop();
            }

            ImGui::SetNextItemOpen(false, ImGuiCond_Once);
            if (ImGui::TreeNode("Action Keys"))
            {
                ImGui::Text("T: Toggle Mouse Position");
                ImGui::Text("R: Reset Stopwatch");
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Other"))
            {
                ImGui::Checkbox("ImGui Demo Window", &m_ShowImGuiDemoWindow);
                static bool doCauseError = false;
                if (ImGui::Checkbox("Force Error (AY_ERROR)", &doCauseError))
                {
                    AY_ERROR("Test error called from the debug menu!");
                }
                ImGui::TreePop();
            }

            ImGui::End();
        }

#endif
    }

} // namespace Ayla::Core::Debug




