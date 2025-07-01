/**
* @file PropertiesPanel.cpp
* @author Andrew Fagan
* @date 6/23/2025
*/

#include "PropertiesPanel.h"

#include "imgui.h"
#include "SceneHierarchyPanel.h"
#include "ECS/SceneManager.h"
#include "glm/gtc/type_ptr.hpp"

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

        if (ecs.HasComponent<PointLightComponent>(selectedEntity))
        {
            ShowPointLightComponent(ecs, selectedEntity);
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

            if (!ecs.HasComponent<PointLightComponent>(selectedEntity))
            {
                canAddAComponent = true;
                if (ImGui::MenuItem("Point Light"))
                {
                    PointLightComponent pointLightComponent{};
                    ecs.AddComponent(selectedEntity, pointLightComponent);
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

    void ShowPointLightComponent(ECS& ecs, Entity& entity)
    {
        if (ImGui::TreeNodeEx("Point Light##PointLightComponentSceneHierarchy", ImGuiTreeNodeFlags_DefaultOpen))
        {

            PointLightComponent pointLight;
            ECS_Result result = ecs.GetComponent(entity, pointLight);
            ASSERT(result == ECS_Result::ECS_SUCCESS, "SceneHierarchyPanel failed to get transform component")

            ImGui::Text("Light Color: ");
            ImGui::SameLine();
            ImGui::InputFloat3("##LightColorInput", glm::value_ptr(pointLight.LightColor));

            ImGui::Text("Intensity: ");
            ImGui::SameLine();
            ImGui::InputFloat("##IntensityInput", &pointLight.Intensity);

            ecs.AddComponent(entity, pointLight);

            ImGui::TreePop();
        }
    }

}
