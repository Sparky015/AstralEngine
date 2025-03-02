/**
* @file Editor.cpp
* @author Andrew Fagan
* @date 3/1/2025
*/

#include "Editor.h"

#include "imgui.h"
#include "SceneGraphPanel.h"
#include "ViewportPanel.h"

namespace Astral {

    Editor::Editor()
    {
    }

    Editor::~Editor()
    {
    }


    void Editor::Init()
    {
        m_RenderImGuiListener.StartListening();
    }


    void Editor::Shutdown()
    {
        m_RenderImGuiListener.StopListening();
    }


    void Editor::UpdateEditorUI()
    {
        ViewportPanel();
        SceneGraphPanel();
    }

}
