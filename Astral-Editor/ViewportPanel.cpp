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
        ImGui::Begin("Viewport##EditorViewport", nullptr);

        TextureHandle viewportTexture = Astral::SceneRenderer::GetViewportTexture();
        UVec2 textureDimensions = viewportTexture->GetDimensions();
        ImVec2 imguiDimensions = ImVec2(textureDimensions.x, textureDimensions.y);
        ImGui::Image(viewportTexture->GetImGuiTextureID(), imguiDimensions);

        ImGui::End();
    }

}
