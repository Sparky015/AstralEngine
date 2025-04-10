//
// Created by Andrew Fagan on 1/6/25.
//

#include "ECSComponents.h"

#include <imgui.h>

#include "ECS/ECS.h"
#include "../../../ECS/ECSManager.h"

void Debug::EntityCountComponent()
{
    ImGui::Text("Current Number Of Entities: %d", Astral::g_ECSManager.GetECS().GetNumberOfActiveEntities());
}
