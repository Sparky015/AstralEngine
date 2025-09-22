/**
* @file SceneHierarchyPanel.cpp
* @author Andrew Fagan
* @date 3/1/2025
*/

#include "SceneHierarchyPanel.h"

#include "Core/Engine.h"
#include "Scenes/SceneManager.h"
#include "Utilities/AssetFileHelpers.h"

#include "imgui.h"
#include "Asset/AssetManager.h"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer/Common/Material.h"

namespace Astral {

    Entity SceneHierarchyPanel::selectedEntity = Entity(NULL_ENTITY);

    void SceneHierarchyPanel::Show()
    {
        ECS& ecs = Engine::Get().GetSceneManager().GetECS();
        if (ecs.GetNumberOfActiveEntities() == 0) { selectedEntity = Entity(NULL_ENTITY); }

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

            if (ImGui::MenuItem("Rename Entity"))
            {
                entityToBeRenamed = selectedEntity;
                openEntityRenamePopup = true;
            }

            if (ImGui::MenuItem("Delete Entity") || ImGui::IsKeyDown(ImGuiKey_Delete))
            {
                ecs.DeleteEntity(selectedEntity);
            }

            ImGui::EndPopup();
        }



        for (Entity entity : ecs)
        {
            ImGui::PushID(entity.GetID());

            ImGui::Spacing();

            if (ImGui::Selectable(entity.GetDebugName().data(), selectedEntity == entity))
            {
                selectedEntity = entity;

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

            }

            ImGui::PopID();
        }


        ImGui::End();

        static char entityNameBuffer[128];
        if (openEntityRenamePopup)
        {
            ImGui::OpenPopup("Rename Entity##SceneHierarchyPanel");
            openEntityRenamePopup = false;
            strncpy(entityNameBuffer, entityToBeRenamed.GetDebugName().data(), sizeof(entityNameBuffer));
        }

        if (ImGui::BeginPopupModal("Rename Entity##SceneHierarchyPanel", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
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
            ecs.GetComponent(entity, transform);

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
            ecs.GetComponent(entity, sprite);

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
                    Scene& scene = Engine::Get().GetSceneManager().GetActiveScene();
                    if (sprite.Material) { scene.DecrementResourceRef(registry.GetFilePathFromAssetID(sprite.Material->GetAssetID())); }
                    scene.IncrementResourceRef(registry.GetFilePathFromAssetID(newMaterial->GetAssetID()));
                    sprite.Material = newMaterial;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("...##SelectMaterialFilePath"))
            {
                std::filesystem::path selectedFilePath;
                SelectFileFromDialog(selectedFilePath, "Astral Material", "astmat");
                registry.GetRelativePath(selectedFilePath);
                std::string relativePath = selectedFilePath.string();
                Ref<Material> newMaterial = registry.CreateAsset<Material>(relativePath);

                if (newMaterial != nullptr)
                {
                    Scene& scene = Engine::Get().GetSceneManager().GetActiveScene();
                    if (sprite.Material) { scene.DecrementResourceRef(registry.GetFilePathFromAssetID(sprite.Material->GetAssetID())); }
                    scene.IncrementResourceRef(registry.GetFilePathFromAssetID(newMaterial->GetAssetID()));
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
            ecs.GetComponent(entity, meshComponent);

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
                Ref<Mesh> newMesh = registry.CreateAsset<Mesh>(meshInputBuffer);

                if (newMesh != nullptr)
                {
                    Scene& scene = Engine::Get().GetSceneManager().GetActiveScene();
                    if (meshComponent.MeshData) { scene.DecrementResourceRef(registry.GetFilePathFromAssetID(meshComponent.MeshData->GetAssetID())); }
                    scene.IncrementResourceRef(registry.GetFilePathFromAssetID(newMesh->GetAssetID()));
                    meshComponent.MeshData = newMesh;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("...##SelectMeshFilePath"))
            {
                std::filesystem::path selectedFilePath;
                SelectFileFromDialog(selectedFilePath, "3D Object", "fbx,gltf,obj");
                registry.GetRelativePath(selectedFilePath);
                std::string relativePath = selectedFilePath.string();
                Ref<Mesh> newMesh = registry.CreateAsset<Mesh>(relativePath);

                if (newMesh != nullptr)
                {
                    Scene& scene = Engine::Get().GetSceneManager().GetActiveScene();
                    if (meshComponent.MeshData) { scene.DecrementResourceRef(registry.GetFilePathFromAssetID(meshComponent.MeshData->GetAssetID())); }
                    scene.IncrementResourceRef(registry.GetFilePathFromAssetID(newMesh->GetAssetID()));
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
                Ref<Material> newMaterial = registry.CreateAsset<Material>(materialInputBuffer);

                if (newMaterial != nullptr)
                {
                    Scene& scene = Engine::Get().GetSceneManager().GetActiveScene();
                    if (meshComponent.Material) { scene.DecrementResourceRef(registry.GetFilePathFromAssetID(meshComponent.Material->GetAssetID())); }
                    scene.IncrementResourceRef(registry.GetFilePathFromAssetID(newMaterial->GetAssetID()));
                    meshComponent.Material = newMaterial;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("...##SelectMaterialFilePath"))
            {
                std::filesystem::path selectedFilePath;
                SelectFileFromDialog(selectedFilePath, "Astral Material", "astmat");
                registry.GetRelativePath(selectedFilePath);
                std::string relativePath = selectedFilePath.string();
                Ref<Material> newMaterial = registry.CreateAsset<Material>(relativePath);

                if (newMaterial != nullptr)
                {
                    Scene& scene = Engine::Get().GetSceneManager().GetActiveScene();
                    if (meshComponent.Material) { scene.DecrementResourceRef(registry.GetFilePathFromAssetID(meshComponent.Material->GetAssetID())); }
                    scene.IncrementResourceRef(registry.GetFilePathFromAssetID(newMaterial->GetAssetID()));
                    meshComponent.Material = newMaterial;
                }
            }

            ecs.AddComponent(entity, meshComponent);

            ImGui::TreePop();
        }
    }




}
