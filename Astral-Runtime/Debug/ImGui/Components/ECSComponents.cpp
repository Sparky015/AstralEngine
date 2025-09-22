//
// Created by Andrew Fagan on 1/6/25.
//

#include "ECSComponents.h"

#include <imgui.h>

#include "../../../Scenes/SceneManager.h"
#include "Core/Engine.h"

void Astral::EntityCountComponent()
{
    ImGui::Text("Current Number Of Entities: %d", Astral::Engine::Get().GetSceneManager().GetECS().GetNumberOfActiveEntities());
}
