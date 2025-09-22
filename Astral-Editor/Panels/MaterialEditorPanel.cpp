/**
* @file MaterialEditorPanel.cpp
* @author Andrew Fagan
* @date 9/1/25
*/

#include "MaterialEditorPanel.h"

#include "ECS/ECS.h"
#include "Scenes/SceneManager.h"

#include "imgui.h"
#include "nfd.h"
#include "SceneHierarchyPanel.h"
#include "Asset/Loaders/MaterialLoader.h"
#include "Renderer/RHI/RendererAPI.h"
#include "Utilities/AssetFileHelpers.h"

namespace Astral {

    Material MaterialEditorPanel::m_MaterialCopy = Material();
    AssetID MaterialEditorPanel::m_SelectedEntityMaterialID = NullAssetID;
    Entity MaterialEditorPanel::m_SelectedEntity = Entity();


    void MaterialEditorPanel::Show()
    {
        ECS& ecs = Engine::Get().GetSceneManager().GetECS();
        AssetRegistry& registry = Engine::Get().GetAssetManager().GetRegistry();

        ImGui::Begin("Material Editor##MaterialEditorPanel", nullptr);

        Entity activeEntity = SceneHierarchyPanel::GetSelectedEntity();

        if (!ecs.HasComponent<MeshComponent>(activeEntity))
        {
            ImGui::Text("No entity with material component selected!");
            ImGui::End();
            return;
        }

        MeshComponent meshComponent;
        ecs.GetComponent(activeEntity, meshComponent);

        if (!meshComponent.Material) // m_MaterialCopy.Textures.size() == 0 if true means it has not been used yet
        {
            ImGui::Text("Empty material!");

            if (ImGui::Button("Create New Material"))
            {
                meshComponent.Material = CreateRef<Material>();
                m_MaterialCopy = Material();
                m_SelectedEntity = activeEntity;
                ecs.UpdateComponent(activeEntity, meshComponent);
            }
            else
            {
                ImGui::End();
                return;
            }
        }

        if (meshComponent.Material)
        {
            Material& selectedMaterial = *meshComponent.Material;

            if (m_SelectedEntityMaterialID != selectedMaterial.GetAssetID() || m_SelectedEntity != activeEntity)
            {
                // Material instance changed so update the material copy to that material.
                // We use a material copy so that we can make changes without actually changing the material until the user wants to.
                m_MaterialCopy = selectedMaterial;
            }

            m_SelectedEntity = activeEntity;
            m_SelectedEntityMaterialID = selectedMaterial.GetAssetID();
        }



        ImGui::Text("Shader Model:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##MaterialEditorPanelShaderModelSelect", ShaderModelToString(m_MaterialCopy.ShaderModel).data()))
        {
            if (ImGui::Selectable("PBR")) { m_MaterialCopy.ShaderModel = ShaderModel::PBR; }
            if (ImGui::Selectable("Unlit")) { m_MaterialCopy.ShaderModel = ShaderModel::UNLIT; }
            ImGui::EndCombo();
        }

        ImGui::Text("Texture Convention:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##MaterialEditorPanelTextureConventionSelect", TextureConventionToString(m_MaterialCopy.TextureConvention).data()))
        {
            if (ImGui::Selectable("Unpacked")) { m_MaterialCopy.TextureConvention = TextureConvention::UNPACKED; }
            if (ImGui::Selectable("AO-Roughness-Metallic Packed")) { m_MaterialCopy.TextureConvention = TextureConvention::ORM_PACKED; }
            ImGui::EndCombo();
        }

        ImGui::Text("Normals Convention:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##MaterialEditorPanelNormalsConventionSelect", m_MaterialCopy.HasDirectXNormals ? "DirectX" : "OpenGL/Vulkan"))
        {
            if (ImGui::Selectable("OpenGL/Vulkan")) { m_MaterialCopy.HasDirectXNormals = false; }
            if (ImGui::Selectable("DirectX")) { m_MaterialCopy.HasDirectXNormals = true; }
            ImGui::EndCombo();
        }

        ImGui::Text("Is Alpha Blended:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##MaterialEditorPanelIsAlphaBlendedSelect", m_MaterialCopy.IsAlphaBlended ? "Yes" : "No"))
        {
            if (ImGui::Selectable("No")) { m_MaterialCopy.IsAlphaBlended = false; }
            if (ImGui::Selectable("Yes")) { m_MaterialCopy.IsAlphaBlended = true; }
            ImGui::EndCombo();
        }

        ImGui::Text("Use Normal Map:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##MaterialEditorPanelUseNormalMapSelect", m_MaterialCopy.HasNormalMap ? "Yes" : "No"))
        {
            if (ImGui::Selectable("No")) { m_MaterialCopy.HasNormalMap = false; }
            if (ImGui::Selectable("Yes")) { m_MaterialCopy.HasNormalMap = true; }
            ImGui::EndCombo();
        }



        if (m_MaterialCopy.TextureConvention == TextureConvention::UNPACKED)
        {
            if (m_MaterialCopy.Textures.size() < 5) { m_MaterialCopy.Textures.resize(5); }
            static constexpr std::array<std::string_view, 5> textureNames = {"Base Color: ", "Metallic: ", "Roughness: ", "Emission: ", "Normals: "};

            for (size_t i = 0; i < textureNames.size(); i++)
            {
                AssetSelectionComponent(textureNames[i], m_MaterialCopy.Textures[i], "Textures", "jpeg,jpg,png,tiff,bmp,tga,hdr");
            }
        }
        else if (m_MaterialCopy.TextureConvention == TextureConvention::ORM_PACKED)
        {
            if (m_MaterialCopy.Textures.size() < 4) { m_MaterialCopy.Textures.resize(4); }
            static constexpr std::array<std::string_view, 4> textureNames = {"Base Color", "AO-Roughness-Metallic", "Emission", "Normals"};

            for (size_t i = 0; i < textureNames.size(); i++)
            {
                AssetSelectionComponent(textureNames[i], m_MaterialCopy.Textures[i], "Textures", "jpeg,jpg,png,tiff,bmp,tga,hdr");
            }
        }

        if (ImGui::Button("Save to Entity"))
        {
            Ref<Material> materialRef = CreateRef<Material>(m_MaterialCopy);
            meshComponent.Material = materialRef;

            meshComponent.Material->DescriptorSet = RendererAPI::GetDevice().CreateDescriptorSet();
            meshComponent.Material->DescriptorSet->BeginBuildingSet();
            for (const TextureHandle& textureMap : meshComponent.Material->Textures)
            {
                meshComponent.Material->DescriptorSet->AddDescriptorImageSampler(textureMap, ShaderStage::FRAGMENT);
            }
            meshComponent.Material->DescriptorSet->EndBuildingSet();

            ecs.UpdateComponent(activeEntity, meshComponent);
        }

        if (ImGui::Button("Save to Disk"))
        {
            nfdu8char_t* outPath;
            nfdu8filteritem_t filters[1] = { { "Astral Material", "astmat" }};
            nfdsavedialogu8args_t args = {0};
            args.filterList = filters;
            args.filterCount = 1;

            nfdresult_t result = NFD_SaveDialogU8_With(&outPath, &args);

            if (result == NFD_OKAY)
            {
                std::filesystem::path outFilePath = std::filesystem::path(outPath);
                NFD_FreePathU8(outPath);
                Ref<Material> materialRef = CreateRef<Material>(m_MaterialCopy);
                MaterialLoader::SerializeMaterial(materialRef, outFilePath);
            }
            else if (result == NFD_CANCEL)
            {
                AE_LOG("Save File Dialog Canceled");
            }
            else if (result == NFD_ERROR)
            {
                AE_WARN("NFD Error: " << NFD_GetError())
            }
        }

        ImGui::End();
    }

}
