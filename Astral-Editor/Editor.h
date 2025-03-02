/**
* @file Editor.h
* @author Andrew Fagan
* @date 3/1/2025
*/

#pragma once

#include "Core/Events/EventListener.h"
#include "Debug/ImGui/ImGuiEvents.h"

namespace Astral {

    class Editor
    {
    public:
        Editor();
        ~Editor();

        void Init();
        void Shutdown();

    private:

        void UpdateEditorUI();

        Core::EventListener<RenderImGuiEvent> m_RenderImGuiListener{[this](RenderImGuiEvent e){ UpdateEditorUI(); }};
    };

}