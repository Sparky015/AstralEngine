/**
* @file PropertiesPanel.cpp
* @author Andrew Fagan
* @date 6/23/2025
*/

#include "PropertiesPanel.h"

#include "imgui.h"
#include "SceneHierarchyPanel.h"
#include "ECS/SceneManager.h"

namespace Astral {

    void Astral::PropertiesPanel()
    {
        ImGui::Begin("Properties##EditorPropertiesPanel");

        ECS& ecs = Engine::Get().GetSceneManager().GetECS();
        Entity selectedEntity = SceneHierarchyPanel::GetSelectedEntity();

        if (selectedEntity.GetID() == NULL_ENTITY)
        {
            ImGui::End();
            return;
        }


        // Display Owned Components

        if (ecs.HasComponent<TransformComponent>(selectedEntity))
        {
            ShowTransformComponent(ecs, selectedEntity);
        }

        if (ecs.HasComponent<SpriteComponent>(selectedEntity))
        {
            ShowSpriteComponent(ecs, selectedEntity);
        }

        if (ecs.HasComponent<MeshComponent>(selectedEntity))
        {
            ShowMeshComponent(ecs, selectedEntity);
        }



        // Add Components

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::BeginMenu("Add Component"))
        {
            bool canAddAComponent = false;

            if (!ecs.HasComponent<TransformComponent>(selectedEntity))
            {
                canAddAComponent = true;
                if (ImGui::MenuItem("Transform"))
                {
                    TransformComponent transformComponent{};
                    ecs.AddComponent(selectedEntity, transformComponent);
                }
            }

            if (!ecs.HasComponent<SpriteComponent>(selectedEntity))
            {
                canAddAComponent = true;
                if (ImGui::MenuItem("Sprite"))
                {
                    SpriteComponent spriteComponent{};
                    ecs.AddComponent(selectedEntity, spriteComponent);
                }
            }

            if (!ecs.HasComponent<MeshComponent>(selectedEntity))
            {
                canAddAComponent = true;
                if (ImGui::MenuItem("Mesh"))
                {
                    MeshComponent meshComponent{};
                    ecs.AddComponent(selectedEntity, meshComponent);
                }
            }

            if (!canAddAComponent)
            {
                ImGui::Text("(All Component Types Used Already)");
            }

            ImGui::EndMenu();
        }


        ImGui::End();
    }

}
