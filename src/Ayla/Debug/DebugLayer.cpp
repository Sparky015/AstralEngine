//
// Created by Andrew Fagan on 6/25/24.
//

#include "DebugLayer.h"

#include "Ayla/Core/Time/Clock.h"
#include "Ayla/Input/InputState.h"
#include "Ayla/Input/Keycodes.h"

#include "imgui.h"


namespace Ayla::Core::Debug
{
    bool DebugLayer::m_ShowDemoWindow = false;
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
        using namespace Ayla::Input::Keycodes;

        static int64 accumulatedTime;
        accumulatedTime += Time::Clock::Get().GetDeltaTime();

        const Input::SInputState& inputState = Input::SInputState::Get();
        if (inputState.IsKeyDown(AY_KEY_T) && !inputState.IsKeyRepeating(AY_KEY_T))
        {
            if (accumulatedTime <= 75){return;}
            accumulatedTime = 0;
            AY_TRACE("T is pressed down.");
            AY_LOG_SS(inputState.MousePositionX() << " by " << inputState.MousePositionY());
        }
        if (inputState.IsKeyDown(AY_KEY_R) && !inputState.IsKeyRepeating(AY_KEY_R))
        {
            if (accumulatedTime <= 300){return;}
            accumulatedTime = 0;
            AY_TRACE("Resetting stopwatch.");
            Ayla::Core::Time::Clock::Get().ResetStopwatch();
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
        using namespace Input::Keycodes;
        const Input::SInputState& inputState = Input::SInputState::Get();
        const ImGuiIO& io = ImGui::GetIO();

        if (m_ShowDemoWindow)
        {
            ImGui::ShowDemoWindow(&m_ShowDemoWindow);
        }

        if (m_ShowDebugMenu)
        {
            ImGui::SetNextWindowSize(ImVec2(600, 400));
            ImGui::Begin("Debug Menu", &m_ShowDebugMenu);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::Text("Stopwatch time: %.3f seconds", Time::Clock::Get().GetStopwatchTime() * .001);

            static bool isInputStateNodeOpen;
            ImGui::SetNextItemOpen(true, ImGuiCond_Once);
            isInputStateNodeOpen = ImGui::TreeNode("Input State");
            if (isInputStateNodeOpen)
            {
                ImGui::Text("%s : %s", inputState.GetKeyName(AY_KEY_A).c_str(), (inputState.IsKeyDown(AY_KEY_A) == true) ? "Down" : "Up");
                ImGui::TreePop();
            }

            static bool isActionKeysNodeOpen;
            ImGui::SetNextItemOpen(false, ImGuiCond_Once);
            isActionKeysNodeOpen = ImGui::TreeNode("Action Keys");
            if (isActionKeysNodeOpen)
            {
                ImGui::Text("Keys Pressed:");
                ImGui::Text("T: Toggle Mouse Position");
                ImGui::Text("R: Reset Stopwatch");
                ImGui::TreePop();
            }


            if (ImGui::TreeNode("Resizable, stretch"))
            {
                // By default, if we don't enable ScrollX the sizing policy for each column is "Stretch"
                // All columns maintain a sizing weight, and they will occupy all available width.
                const static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;

                if (ImGui::BeginTable("Input States", 3, flags))
                {

                    for (int keycode = 0; keycode < Input::SInputState::NUMBER_OF_KEYS; keycode++)
                    {
                        ImGui::Text("%s : %s", inputState.GetKeyName(keycode).c_str(),
                                    (inputState.IsKeyDown(keycode) == true) ? "Down" : "Up");
                        ImGui::TableNextColumn();
                    }

                    ImGui::EndTable();
                }
                ImGui::TreePop();
            }


            ImGui::Checkbox("Demo Window", &m_ShowDemoWindow);

            ImGui::End();
        }

#endif
    }

}




