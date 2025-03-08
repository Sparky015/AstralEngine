//
// Created by Andrew Fagan on 1/6/25.
//

#include "ECSComponents.h"

#include <imgui.h>

#include "ECS/ECS.h"
#include "ECS/Old/ECSManager.h"

void Debug::EntityCountComponent()
{
    ImGui::Text("Current Number Of Entities: %d", ECS::g_ECSManager.GetECS().GetCurrentEntityCount());
}
