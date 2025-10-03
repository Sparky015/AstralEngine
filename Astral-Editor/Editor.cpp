/**
* @file Editor.cpp
* @author Andrew Fagan
* @date 3/1/2025
*/

#include "Editor.h"

#include "Components/CameraController.h"
#include "Components/ExternalSceneBreakdownConfigComponent.h"
#include "Debug/ImGui/ImGuiManager.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ViewportPanel.h"
#include "Components/MenuBarComponent.h"
#include "Core/Engine.h"
#include "Panels/EnvironmentPanel.h"
#include "Panels/MaterialEditorPanel.h"
#include "Panels/PropertiesPanel.h"

namespace Astral {

    Editor::Editor()
    {
    }

    Editor::~Editor()
    {
    }


    void Editor::Init()
    {
        Engine::Get().GetImGuiManager().EnableViewportDockSpace();
        m_RenderImGuiListener.StartListening();
    }


    void Editor::Shutdown()
    {
        m_RenderImGuiListener.StopListening();

        ExternalSceneBreakdownConfigComponent::Cleanup();
    }


    void Editor::UpdateEditorUI()
    {
        PROFILE_SCOPE("Editor::UpdateEditorUI")

        SceneHierarchyPanel::Show();
        ViewportPanel::Show();
        MenuBarComponent::Show();
        PropertiesPanel();
        EnvironmentPanel();
        MaterialEditorPanel::Show();
        CameraControllerComponent();
    }

}
