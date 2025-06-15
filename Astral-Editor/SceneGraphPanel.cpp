/**
* @file SceneGraphPanel.cpp
* @author Andrew Fagan
* @date 3/1/2025
*/

#include "SceneGraphPanel.h"

#include "Core/Engine.h"
#include "ECS/ECSManager.h"

#include "imgui.h"

namespace Astral {

    void SceneGraphPanel()
    {
        ECS& ecs = Engine::Get().GetECSManager().GetECS();

        ImGui::Begin("SceneGraph##EditorSceneGraph", nullptr);

        // ecs;

        ImGui::End();
    }

}
