/**
* @file ViewportPanel.h
* @author Andrew Fagan
* @date 3/1/2025
*/

#pragma once

#include "imgui.h"

namespace Astral {

    class ViewportPanel
    {
    public:

        static void Show();

    private:

        static void ShowViewportRender();
        static void ShowGizmo();

        static ImVec2 m_ContentRegionSize;
    };

}
