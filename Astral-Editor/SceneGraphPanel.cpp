/**
* @file SceneGraphPanel.cpp
* @author Andrew Fagan
* @date 3/1/2025
*/

#include "SceneGraphPanel.h"

#include "Core/Engine.h"
#include "ECS/SceneManager.h"

#include "imgui.h"
#include "Asset/AssetManager.h"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer/Common/Material.h"

namespace Astral {

    void SceneGraphPanel()
    {
        ECS& ecs = Engine::Get().GetSceneManager().GetECS();

        static Entity entityToBeRenamed{};
        static bool openEntityRenamePopup = false;

        ImGui::Begin("SceneGraph##EditorSceneGraph", nullptr);

        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Add Entity"))
            {
                Entity entity = ecs.CreateEntity("Untitled");

                // Add transform component by default
                TransformComponent transformComponent{};
                ecs.AddComponent(entity, transformComponent);
            }

            ImGui::EndPopup();
        }


        if (ImGui::TreeNodeEx("Entities", ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (Entity entity : ecs)
            {
                ImGui::PushID(entity.GetID());

                if (ImGui::TreeNode(entity.GetDebugName().data()))
                {

                    if (ImGui::BeginPopupContextItem("##EntityAddComponentPopUp"))
                    {
                        if (ImGui::MenuItem("Rename Entity"))
                        {
                            entityToBeRenamed = entity;
                            openEntityRenamePopup = true;
                        }

                        if (ImGui::BeginMenu("Add Component"))
                        {
                            bool canAddAComponent = false;

                            if (!ecs.HasComponent<TransformComponent>(entity))
                            {
                                canAddAComponent = true;
                                if (ImGui::MenuItem("Transform"))
                                {
                                    TransformComponent transformComponent{};
                                    ecs.AddComponent(entity, transformComponent);
                                }
                            }

                            if (!ecs.HasComponent<SpriteComponent>(entity))
                            {
                                canAddAComponent = true;
                                if (ImGui::MenuItem("Sprite"))
                                {
                                    SpriteComponent spriteComponent{};
                                    ecs.AddComponent(entity, spriteComponent);
                                    // TODO: Add defaultable meshes, textures, and materials
                                }
                            }

                            if (!ecs.HasComponent<MeshComponent>(entity))
                            {
                                canAddAComponent = true;
                                if (ImGui::MenuItem("Mesh"))
                                {
                                    MeshComponent meshComponent{};
                                    ecs.AddComponent(entity, meshComponent);

                                }
                            }

                            if (!canAddAComponent)
                            {
                                ImGui::Text("(All Component Types Used Already)");
                            }

                            ImGui::EndMenu();
                        }

                        ImGui::EndPopup();
                    }

                    if (ecs.HasComponent<TransformComponent>(entity))
                    {
                        ShowTransformComponent(ecs, entity);
                    }

                    if (ecs.HasComponent<SpriteComponent>(entity))
                    {
                        ShowSpriteComponent(ecs, entity);
                    }

                    if (ecs.HasComponent<MeshComponent>(entity))
                    {
                        ShowMeshComponent(ecs, entity);
                    }

                    ImGui::TreePop();
                }

                ImGui::PopID();
            }

            ImGui::TreePop();
        }

        ImGui::End();

        static char entityNameBuffer[128];
        if (openEntityRenamePopup)
        {
            ImGui::OpenPopup("Rename Entity##SceneGraphPanel");
            openEntityRenamePopup = false;
            strncpy(entityNameBuffer, entityToBeRenamed.GetDebugName().data(), sizeof(entityNameBuffer));
        }

        if (ImGui::BeginPopupModal("Rename Entity##SceneGraphPanel", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (ImGui::InputText("##SceneGraphRenameEntityInput", entityNameBuffer, sizeof(entityNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                ecs.RenameEntity(entityToBeRenamed, entityNameBuffer);
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("OK"))
            {
                ecs.RenameEntity(entityToBeRenamed, entityNameBuffer);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }


    void ShowTransformComponent(ECS& ecs, const Entity& entity)
    {
        if (ImGui::TreeNodeEx("Transform##TransformComponentSceneGraph", ImGuiTreeNodeFlags_DefaultOpen))
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

            ImGui::Text("Rotation: ");
            ImGui::SameLine();
            ImGui::InputFloat3("##RotationInput", glm::value_ptr(transform.rotation));

            ecs.AddComponent(entity, transform);

            ImGui::TreePop();
        }
    }


    void ShowSpriteComponent(ECS& ecs, const Entity& entity)
    {
        if (ImGui::TreeNodeEx("Sprite##SpriteComponentSceneGraph", ImGuiTreeNodeFlags_DefaultOpen))
        {

            SpriteComponent sprite;
            ECS_Result result = ecs.GetComponent(entity, sprite);
            ASSERT(result == ECS_Result::ECS_SUCCESS, "SceneGraphPanel failed to get sprite component")

            AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

            AssetID materialAssetID = NullAssetID;
            if (sprite.Material)
            {
                materialAssetID = sprite.Material->GetAssetID();
            }

            // Show Material asset file path
            static char materialInputBuffer[200];
            GetAssetFilePath(registry, materialAssetID, materialInputBuffer, sizeof(materialInputBuffer));

            ImGui::Text("Material: ");
            ImGui::SameLine();
            if (ImGui::InputText("##MaterialFilePath", materialInputBuffer, sizeof(materialInputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                Ref<Material> newMaterial = registry.GetAsset<Material>(materialInputBuffer);

                if (newMaterial != nullptr)
                {
                    sprite.Material = newMaterial;
                }
            }

            ecs.AddComponent(entity, sprite);

            ImGui::TreePop();
        }
    }


    void ShowMeshComponent(ECS& ecs, const Entity& entity)
    {
        if (ImGui::TreeNodeEx("Mesh##MeshComponentSceneGraph", ImGuiTreeNodeFlags_DefaultOpen))
        {

            MeshComponent meshComponent;
            ECS_Result result = ecs.GetComponent(entity, meshComponent);
            ASSERT(result == ECS_Result::ECS_SUCCESS, "SceneGraphPanel failed to get transform component")

            AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

            AssetID meshAssetID = NullAssetID;
            if (meshComponent.MeshData)
            {
                meshAssetID = meshComponent.MeshData->GetAssetID();
            }
            static char meshInputBuffer[200];
            GetAssetFilePath(registry, meshAssetID, meshInputBuffer, sizeof(meshInputBuffer));


            ImGui::Text("Mesh: ");
            ImGui::SameLine();
            if (ImGui::InputText("##MeshFilePath", meshInputBuffer, sizeof(meshInputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                Ref<Mesh> newMesh = registry.GetAsset<Mesh>(meshInputBuffer);

                if (newMesh != nullptr)
                {
                    meshComponent.MeshData = newMesh;
                }
            }


            AssetID materialAssetID = NullAssetID;
            if (meshComponent.Material)
            {
                materialAssetID = meshComponent.Material->GetAssetID();
            }
            static char materialInputBuffer[200];
            GetAssetFilePath(registry, materialAssetID, materialInputBuffer, sizeof(materialInputBuffer));


            ImGui::Text("Material: ");
            ImGui::SameLine();
            if (ImGui::InputText("##MaterialFilePath", materialInputBuffer, sizeof(materialInputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
            {
                Ref<Material> newMaterial = registry.GetAsset<Material>(materialInputBuffer);

                if (newMaterial != nullptr)
                {
                    meshComponent.Material = newMaterial;
                }
            }

            ecs.AddComponent(entity, meshComponent);

            ImGui::TreePop();
        }
    }


    void GetAssetFilePath(AssetRegistry& registry, AssetID materialAssetID, char* inputBuffer, int inputBufferSize)
    {
        std::string filePath;
        if (materialAssetID != NullAssetID)
        {
            filePath = registry.GetFilePathFromAssetID(materialAssetID).string();
        }
        else
        {
            filePath = "";
        }
        strncpy(inputBuffer, filePath.c_str(), inputBufferSize - 1);
        inputBuffer[inputBufferSize - 1] = '\0'; // Ensure null-termination
    }

}
