/**
* @file MenuBarComponent.h
* @author Andrew Fagan
* @date 6/21/2025
*/

#pragma once

#include <string>

namespace Astral {

    class MenuBarComponent
    {
    public:
        static void Show();

        static void DisableModal() { m_ShowModal = false; }
        static void LoadExternalScene() { m_LoadExternalScene = true; }

    private:

        static bool m_ShowModal;
        static bool m_LoadExternalScene;
        static bool m_ShouldSerializeObjects;
        static std::string m_ExternalSceneFilePath;
    };

}