/**
* @file ExternalSceneBreakdownConfigComponent.cpp
* @author Andrew Fagan
* @date 7/23/25
*/

#include "ExternalSceneBreakdownConfigComponent.h"

#include "MenuBarComponent.h"
#include "Asset/AssetManager.h"
#include "Asset/Loaders/SceneLoader.h"
#include "Asset/Loaders/ShaderLoader.h"
#include "Core/Engine.h"
#include "Core/SmartPointers.h"
#include "imgui/imgui.h"
#include "Renderer/RHI/Resources/Texture.h"
#include "Utilities/AssetFileHelpers.h"

namespace Astral {

    bool ExternalSceneBreakdownConfigComponent::m_IsNewShow = true;

    ShaderModel ExternalSceneBreakdownConfigComponent::m_SettingsMaterialShaderModel = ShaderModel::PBR;
    TextureConvention ExternalSceneBreakdownConfigComponent::m_SettingsMaterialTextureConvention = TextureConvention::ORM_PACKED;

    Vec3 ExternalSceneBreakdownConfigComponent::m_SettingsRotationOffset = Vec3(0.0f);
    std::string_view ExternalSceneBreakdownConfigComponent::m_SettingsCoordinateSystemForwardPreview = "-Z";
    std::string_view ExternalSceneBreakdownConfigComponent::m_SettingsCoordinateSystemUpPreview = "Y";
    std::string_view ExternalSceneBreakdownConfigComponent::m_SettingsCoordinateSystemRightPreview = "X";

    TextureHandle ExternalSceneBreakdownConfigComponent::m_SettingsMaterialBaseColor = nullptr;
    TextureHandle ExternalSceneBreakdownConfigComponent::m_SettingsMaterialNormals = nullptr;
    TextureHandle ExternalSceneBreakdownConfigComponent::m_SettingsMaterialRoughness = nullptr;
    TextureHandle ExternalSceneBreakdownConfigComponent::m_SettingsMaterialMetallic = nullptr;
    TextureHandle ExternalSceneBreakdownConfigComponent::m_SettingsMaterialEmission = nullptr;


    void ExternalSceneBreakdownConfigComponent::Show()
    {
        if (ImGui::BeginPopupModal("3D Object Import##ExternalSceneBreakdownConfigComponent"))
        {
            if (m_IsNewShow)
            {
                // Get current material defaults from scene loader

                m_SettingsMaterialShaderModel = SceneLoader::GetDefaultMaterialShaderModel();
                m_SettingsMaterialTextureConvention = SceneLoader::GetDefaultMaterialTextureConvention();

                m_SettingsMaterialBaseColor = SceneLoader::GetDefaultMaterialBaseColor();
                m_SettingsMaterialNormals = SceneLoader::GetDefaultMaterialNormals();
                m_SettingsMaterialRoughness = SceneLoader::GetDefaultMaterialRoughness();
                m_SettingsMaterialMetallic = SceneLoader::GetDefaultMaterialMetallic();
                m_SettingsMaterialEmission = SceneLoader::GetDefaultMaterialEmission();

                m_IsNewShow = false;
            }


            bool shouldCreateNewSourceFiles = true;
            ImGui::Checkbox("Create New Mesh Source Files", &shouldCreateNewSourceFiles);


            ImGui::Text("Coordinate System");
            ImGui::Spacing();

            ImGui::Text("Forward Axis:");
            ImGui::SameLine();
            if (ImGui::BeginCombo("##ExternalSceneBreakdownConfigComponentCoordinateSystemX", m_SettingsCoordinateSystemForwardPreview.data()))
            {
                if (ImGui::Selectable("Z")) { m_SettingsRotationOffset.y = 180.0f; m_SettingsCoordinateSystemForwardPreview = "Z"; }
                if (ImGui::Selectable("-Z")) { m_SettingsRotationOffset.y = 0.0f; m_SettingsCoordinateSystemForwardPreview = "-Z";}
                if (ImGui::Selectable("X")) { m_SettingsRotationOffset.y = 90.0f; m_SettingsCoordinateSystemForwardPreview = "X";}
                if (ImGui::Selectable("-X")) { m_SettingsRotationOffset.y = -90.0f; m_SettingsCoordinateSystemForwardPreview = "-X";}
                if (ImGui::Selectable("Y")) { m_SettingsRotationOffset.x = -90.0f; m_SettingsCoordinateSystemForwardPreview = "Y";}
                if (ImGui::Selectable("-Y")) { m_SettingsRotationOffset.x = 90.0f;m_SettingsCoordinateSystemForwardPreview = "-Y"; }
                ImGui::EndCombo();
            }
            ImGui::Text("Up Axis:");
            ImGui::SameLine();
            if (ImGui::BeginCombo("##ExternalSceneBreakdownConfigComponentCoordinateSystemY", m_SettingsCoordinateSystemUpPreview.data()))
            {
                if (ImGui::Selectable("Z")) { m_SettingsRotationOffset.x = -90.0f; m_SettingsCoordinateSystemUpPreview = "Z"; }
                if (ImGui::Selectable("-Z")) { m_SettingsRotationOffset.x = 90.0f; m_SettingsCoordinateSystemUpPreview = "-Z";}
                if (ImGui::Selectable("X")) { m_SettingsRotationOffset.z = 90.0f; m_SettingsCoordinateSystemUpPreview = "X";}
                if (ImGui::Selectable("-X")) { m_SettingsRotationOffset.z = -90.0f; m_SettingsCoordinateSystemUpPreview = "-X";}
                if (ImGui::Selectable("Y")) { m_SettingsRotationOffset.x = 0.0f; m_SettingsCoordinateSystemUpPreview = "Y";}
                if (ImGui::Selectable("-Y")) { m_SettingsRotationOffset.x = 180.0f;m_SettingsCoordinateSystemUpPreview = "-Y"; }
                ImGui::EndCombo();
            }
            ImGui::Text("Right Axis:");
            ImGui::SameLine();
            if (ImGui::BeginCombo("##ExternalSceneBreakdownConfigComponentCoordinateSystemZ", m_SettingsCoordinateSystemRightPreview.data()))
            {
                if (ImGui::Selectable("Z")) { m_SettingsRotationOffset.y = 90.0f; m_SettingsCoordinateSystemRightPreview = "Z"; }
                if (ImGui::Selectable("-Z")) { m_SettingsRotationOffset.y = -90.0f; m_SettingsCoordinateSystemRightPreview = "-Z";}
                if (ImGui::Selectable("X")) { m_SettingsRotationOffset.y = 0.0f; m_SettingsCoordinateSystemRightPreview = "X";}
                if (ImGui::Selectable("-X")) { m_SettingsRotationOffset.y = 180.0f; m_SettingsCoordinateSystemRightPreview = "-X";}
                if (ImGui::Selectable("Y")) { m_SettingsRotationOffset.z = 90.0f; m_SettingsCoordinateSystemRightPreview = "Y";}
                if (ImGui::Selectable("-Y")) { m_SettingsRotationOffset.z = -90.0f;m_SettingsCoordinateSystemRightPreview = "-Y"; }
                ImGui::EndCombo();
            }


            ImGui::Text("Material Defaults");
            ImGui::Spacing();


            ImGui::Text("Shader Model:");
            ImGui::SameLine();
            if (ImGui::BeginCombo("##ExternalSceneBreakdownConfigComponentShaderModelSelect", ShaderModelToString(m_SettingsMaterialShaderModel).data()))
            {
                if (ImGui::Selectable("PBR")) { m_SettingsMaterialShaderModel = ShaderModel::PBR; }
                if (ImGui::Selectable("Unlit")) { m_SettingsMaterialShaderModel = ShaderModel::UNLIT; }
                ImGui::EndCombo();
            }

            ImGui::Text("Texture Convention:");
            ImGui::SameLine();
            if (ImGui::BeginCombo("##ExternalSceneBreakdownConfigComponentTextureConventionSelect", TextureConventionToString(m_SettingsMaterialTextureConvention).data()))
            {
                if (ImGui::Selectable("Unpacked")) { m_SettingsMaterialTextureConvention = TextureConvention::UNPACKED; }
                if (ImGui::Selectable("AO-Roughness-Metallic Packed")) { m_SettingsMaterialTextureConvention = TextureConvention::ORM_PACKED; }
                ImGui::EndCombo();
            }


            if (m_SettingsMaterialTextureConvention == TextureConvention::UNPACKED)
            {
                AssetSelectionComponent("Base Color: ", m_SettingsMaterialBaseColor, "Textures", "jpeg,jpg,png,tiff,bmp,tga,hdr");
                AssetSelectionComponent("Normals: ", m_SettingsMaterialNormals, "Textures", "jpeg,jpg,png,tiff,bmp,tga,hdr");
                AssetSelectionComponent("Roughness: ", m_SettingsMaterialRoughness, "Textures", "jpeg,jpg,png,tiff,bmp,tga,hdr");
                AssetSelectionComponent("Metallic: ", m_SettingsMaterialMetallic, "Textures", "jpeg,jpg,png,tiff,bmp,tga,hdr");
                AssetSelectionComponent("Emission: ", m_SettingsMaterialEmission, "Textures", "jpeg,jpg,png,tiff,bmp,tga,hdr");
            }
            else if (m_SettingsMaterialTextureConvention == TextureConvention::ORM_PACKED)
            {
                AssetSelectionComponent("Base Color: ", m_SettingsMaterialBaseColor, "Textures", "jpeg,jpg,png,tiff,bmp,tga,hdr");
                AssetSelectionComponent("Normals: ", m_SettingsMaterialNormals, "Textures", "jpeg,jpg,png,tiff,bmp,tga,hdr");
                AssetSelectionComponent("AO-Roughness-Metallic: ", m_SettingsMaterialRoughness, "Textures", "jpeg,jpg,png,tiff,bmp,tga,hdr");
                AssetSelectionComponent("Emission: ", m_SettingsMaterialEmission, "Textures", "jpeg,jpg,png,tiff,bmp,tga,hdr");
            }



            if (ImGui::Button("Cancel"))
            {
                ClearCurrentTextures();
                ImGui::CloseCurrentPopup();
                m_IsNewShow = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Continue"))
            {
                SceneLoader::SetDefaultMaterialShaderModel(m_SettingsMaterialShaderModel);
                SceneLoader::SetDefaultMaterialTextureConvention(m_SettingsMaterialTextureConvention);
                SceneLoader::SetDefaultCoordinateSystemOffset(m_SettingsRotationOffset);

                if (m_SettingsMaterialBaseColor) { SceneLoader::SetDefaultMaterialBaseColor(m_SettingsMaterialBaseColor); }
                if (m_SettingsMaterialNormals) { SceneLoader::SetDefaultMaterialNormals(m_SettingsMaterialNormals); }
                if (m_SettingsMaterialRoughness) { SceneLoader::SetDefaultMaterialRoughness(m_SettingsMaterialRoughness); }
                if (m_SettingsMaterialMetallic) { SceneLoader::SetDefaultMaterialMetallic(m_SettingsMaterialMetallic); }
                if (m_SettingsMaterialEmission) { SceneLoader::SetDefaultMaterialEmission(m_SettingsMaterialEmission); }

                MenuBarComponent::LoadExternalScene();

                ClearCurrentTextures();

                ImGui::CloseCurrentPopup();
                m_IsNewShow = true;
            }

            ImGui::EndPopup();
        }
    }


    void ExternalSceneBreakdownConfigComponent::Cleanup()
    {
        ClearCurrentTextures();
    }


    void ExternalSceneBreakdownConfigComponent::ClearCurrentTextures()
    {
        m_SettingsMaterialBaseColor = nullptr;
        m_SettingsMaterialNormals = nullptr;
        m_SettingsMaterialRoughness = nullptr;
        m_SettingsMaterialMetallic = nullptr;
        m_SettingsMaterialEmission = nullptr;
    }

}
