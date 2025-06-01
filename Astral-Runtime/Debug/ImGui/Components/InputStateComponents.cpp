//
// Created by Andrew Fagan on 1/6/25.
//

#include "InputStateComponents.h"

#include "imgui.h"
#include "Input/InputState.h"
#include "Core/CoreMacroDefinitions.h"

namespace Astral {

    const static ImGuiTableFlags imGuiTableFlags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;

    void MouseStateComponent()
    {
        ImGui::Text("Mouse Position: (%.3f, %.3f)", Astral::InputState::MousePositionX(), Astral::InputState::MousePositionY());
        ImGui::Text("Left Click: %s", (Astral::InputState::IsKeyDown(KEY_LEFT_CLICK)) ? "Down" : "Up");
        ImGui::Text("Middle Click: %s", (Astral::InputState::IsKeyDown(KEY_MIDDLE_CLICK)) ? "Down" : "Up");
        ImGui::Text("Right Click: %s", (Astral::InputState::IsKeyDown(KEY_RIGHT_CLICK)) ? "Down" : "Up");
    }

    void AToZKeyStatesComponent()
    {

        if (ImGui::BeginTable("A-Z Keys Table", 4, imGuiTableFlags)) {
            for (uint8 keycode = KEY_A; keycode != KEY_Z + 1; keycode++) {
                ImGui::TableNextColumn();
                ImGui::Text("%s : %s", Astral::InputState::GetKeyName(keycode).data(),
                            Astral::InputState::IsKeyDown(keycode) ? "Down" : "Up");
            }
            ImGui::EndTable();
        }
    }

    void ZeroToNineKeyStatesComponent()
    {
        if (ImGui::BeginTable("0-9 Keys Table", 5, imGuiTableFlags))
        {
            for (Astral::uint8 keycode = KEY_0; keycode != KEY_9 + 1; keycode++)
            {
                ImGui::TableNextColumn();
                ImGui::Text("%s : %s", Astral::InputState::GetKeyName(keycode).data(),
                            Astral::InputState::IsKeyDown(keycode) ? "Down" : "Up");
            }
            ImGui::EndTable();
        }
    }

    void SpecialKeyStatesComponent()
    {
        if (ImGui::BeginTable("Special Keys Table", 4, imGuiTableFlags))
        {
            for (uint8 keycode = KEY_SPACE; keycode != KEY_RIGHT_SUPER + 1; keycode++)
            {
                ImGui::TableNextColumn();
                ImGui::Text("%s : %s", Astral::InputState::GetKeyName(keycode).data(),
                            Astral::InputState::IsKeyDown(keycode) ? "Down" : "Up");
            }
            ImGui::EndTable();
        }
    }

    void OtherKeyStatesComponent()
    {
        if (ImGui::TreeNode("Keypad Keys"))
        {
            if (ImGui::BeginTable("Keypad Keys Table", 3, imGuiTableFlags))
            {
                for (uint8 keycode = KEY_KP_0; keycode != KEY_KP_EQUAL + 1; keycode++)
                {
                    ImGui::TableNextColumn();
                    ImGui::Text("%s : %s", Astral::InputState::GetKeyName(keycode).data(),
                                Astral::InputState::IsKeyDown(keycode) ? "Down" : "Up");
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Function Keys"))
        {
            if (ImGui::BeginTable("Function Keys Table", 4, imGuiTableFlags))
            {
                for (uint8 keycode = KEY_F1; keycode != KEY_F24 + 1; keycode++)
                {
                    ImGui::TableNextColumn();
                    ImGui::Text("%s : %s", Astral::InputState::GetKeyName(keycode).data(),
                                Astral::InputState::IsKeyDown(keycode) ? "Down" : "Up");
                }
                ImGui::EndTable();
            }
            ImGui::TreePop();
        }
    }

    void CompleteInputStateComponent()
    {
        if (ImGui::TreeNode("Mouse"))
        {
            MouseStateComponent();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("A-Z"))
        {
            AToZKeyStatesComponent();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("0-9"))
        {
            ZeroToNineKeyStatesComponent();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Special"))
        {
            SpecialKeyStatesComponent();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Other"))
        {
            OtherKeyStatesComponent();
            ImGui::TreePop();
        }
    }


}