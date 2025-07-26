/**
* @file ExternalSceneBreakdownConfigComponent.h
* @author Andrew Fagan
* @date 7/23/25
*/

#pragma once

#include "Renderer/Common/Material.h"
#include "Renderer/RHI/Resources/Texture.h"

namespace Astral {

    class ExternalSceneBreakdownConfigComponent
    {
    public:

        static void Show();

        static void Cleanup();

    private:

        static void ClearCurrentTextures();

        static bool m_IsNewShow;

        static ShaderModel m_SettingsMaterialShaderModel;
        static TextureConvention m_SettingsMaterialTextureConvention;

        static TextureHandle m_SettingsMaterialBaseColor;
        static TextureHandle m_SettingsMaterialNormals;
        static TextureHandle m_SettingsMaterialRoughness;
        static TextureHandle m_SettingsMaterialMetallic;
        static TextureHandle m_SettingsMaterialEmission;
    };

}
