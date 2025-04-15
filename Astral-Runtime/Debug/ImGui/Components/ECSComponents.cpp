//
// Created by Andrew Fagan on 1/6/25.
//

#include "ECSComponents.h"

#include <imgui.h>

#include "ECS/ECSManager.h"
#include "Core/Engine.h"

void Debug::EntityCountComponent()
{
    ImGui::Text("Current Number Of Entities: %d", Engine::Get().GetECSManager().GetECS().GetNumberOfActiveEntities());
}
