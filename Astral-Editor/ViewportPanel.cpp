/**
* @file ViewportPanel.cpp
* @author Andrew Fagan
* @date 3/1/2025
*/

#include "ViewportPanel.h"

#include "Renderer/SceneRenderer.h"

#include "imgui.h"
#include "Input/InputState.h"


namespace Astral {

    void ViewportPanel()
    {
        static ImVec2 contentRegionSize = ImVec2();
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
        ImGui::Begin("Viewport##EditorViewport", nullptr);

        // Engine does not update input for client if the viewport is not active, so that the editor can take inputs
        // and the user can type things without moving the camera in the viewport or do other things in the client
        if (ImGui::IsWindowFocused())
        {
            InputState::EnableTrackingInputs();
        }
        else
        {
            InputState::DisableTrackingInputs();
        }

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
