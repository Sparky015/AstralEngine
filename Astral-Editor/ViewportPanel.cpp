/**
* @file ViewportPanel.cpp
* @author Andrew Fagan
* @date 3/1/2025
*/

#include "ViewportPanel.h"

#include "Renderer/SceneRenderer.h"

#include "imgui.h"


namespace Astral {

    void ViewportPanel()
    {
        static ImVec2 contentRegionSize = ImVec2();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
        ImGui::Begin("Viewport##EditorViewport", nullptr);

        DescriptorSetHandle viewportTexture = Astral::SceneRenderer::GetViewportTexture();

        if (contentRegionSize.x != ImGui::GetContentRegionAvail().x ||
            contentRegionSize.y != ImGui::GetContentRegionAvail().y)
        {
            contentRegionSize = ImGui::GetContentRegionAvail();
            SceneRenderer::ResizeViewport(contentRegionSize.x, contentRegionSize.y);
        }

        ImGui::Image((ImTextureID)viewportTexture->GetNativeHandle(), contentRegionSize);

        ImGui::End();
        ImGui::PopStyleVar();
    }

}
