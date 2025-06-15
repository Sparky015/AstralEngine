/**
* @file SceneGraphPanel.cpp
* @author Andrew Fagan
* @date 3/1/2025
*/

#include "SceneGraphPanel.h"

#include "Core/Engine.h"
#include "ECS/ECSManager.h"

#include "imgui.h"
#include "Asset/AssetManager.h"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer/Common/Material.h"

namespace Astral {

    void SceneGraphPanel()
    {
        ECS& ecs = Engine::Get().GetECSManager().GetECS();

        ImGui::Begin("SceneGraph##EditorSceneGraph", nullptr);

        if (ImGui::TreeNode("Entities"))
        {
            for (Entity entity : ecs)
            {
                ImGui::PushID(entity.GetID());

                if (ImGui::TreeNode(entity.GetDebugName().data()))
                {
                    if (ImGui::TreeNodeEx("Transform##TransformComponentSceneGraph", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        if (ecs.HasComponent<TransformComponent>(entity))
                        {
                            TransformComponent transform;
                            ECS_Result result = ecs.GetComponent(entity, transform);
                            ASSERT(result == ECS_Result::ECS_SUCCESS, "SceneGraphPanel failed to get transform component")

                            ImGui::Text("Position: ");
                            ImGui::SameLine();
                            ImGui::InputFloat3("##PositionInput", glm::value_ptr(transform.position));

                            ImGui::Text("Scale: ");
                            ImGui::SameLine();
                            ImGui::InputFloat3("##ScaleInput", glm::value_ptr(transform.scale));

                            ecs.AddComponent(entity, transform);
                        }

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNodeEx("Sprite##SpriteComponentSceneGraph", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        if (ecs.HasComponent<SpriteComponent>(entity))
                        {
                            SpriteComponent sprite;
                            ECS_Result result = ecs.GetComponent(entity, sprite);
                            ASSERT(result == ECS_Result::ECS_SUCCESS, "SceneGraphPanel failed to get sprite component")

                            AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();
                            Ref<Material> material = registry.GetAsset<Material>(sprite.materialAssetID);

                            std::string filePath = registry.GetFilePathFromAssetID(sprite.materialAssetID).string();

                            static char inputBuffer[200];

                            strncpy(inputBuffer, filePath.c_str(), sizeof(inputBuffer) - 1);
                            inputBuffer[sizeof(inputBuffer) - 1] = '\0'; // Ensure null-termination


                            if (ImGui::InputText("##MaterialFilePath", inputBuffer, sizeof(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
                            {
                                AssetID newAssetID = registry.GetAssetIDFromFilePath(inputBuffer);

                                if (newAssetID != NullAssetID)
                                {
                                    sprite.materialAssetID = newAssetID;
                                }
                            }

                            ecs.AddComponent(entity, sprite);
                        }

                        ImGui::TreePop();
                    }

                    ImGui::TreePop();
                }

                ImGui::PopID();
            }

            ImGui::TreePop();
        }

        ImGui::End();
    }

}
