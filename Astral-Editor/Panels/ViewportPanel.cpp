/**
* @file ViewportPanel.cpp
* @author Andrew Fagan
* @date 3/1/2025
*/

#include "ViewportPanel.h"

#include "Renderer/SceneRenderer.h"

#include "imgui.h"
#include "ImGuizmo.h"
#include "SceneHierarchyPanel.h"
#include "ECS/Entity.h"
#include "ECS/SceneManager.h"
#include "ECS/Systems/RenderingSystem.h"
#include "glm/gtc/type_ptr.inl"
#include "Input/InputState.h"


namespace Astral {

    ImVec2 ViewportPanel::m_ContentRegionSize{};

    void ViewportPanel::Show()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
        ImGui::Begin("Viewport##EditorViewport", nullptr);

        // Engine does not update input for client if the viewport is not active, so that the editor can take inputs
        // and the user can type things without moving the camera in the viewport or do other things in the client
        if (ImGui::IsWindowFocused()) { InputState::EnableTrackingInputs(); }
        else { InputState::DisableTrackingInputs(); }

        ViewportPanel::ShowViewportRender();
        ViewportPanel::ShowGizmo();

        ImGui::End();
        ImGui::PopStyleVar();
    }


    void ViewportPanel::ShowViewportRender()
    {

        if (m_ContentRegionSize.x != ImGui::GetContentRegionAvail().x ||
            m_ContentRegionSize.y != ImGui::GetContentRegionAvail().y)
        {
            m_ContentRegionSize = ImGui::GetContentRegionAvail();
            SceneRenderer::ResizeViewport(m_ContentRegionSize.x, m_ContentRegionSize.y);
        }

        DescriptorSetHandle viewportTexture = Astral::SceneRenderer::GetViewportTexture();
        ImGui::Image((ImTextureID)viewportTexture->GetNativeHandle(), m_ContentRegionSize);
    }


    void ViewportPanel::ShowGizmo()
    {
        ECS& ecs = Engine::Get().GetSceneManager().GetECS();
        Entity selectedEntity = SceneHierarchyPanel::GetSelectedEntity();

        if (selectedEntity.GetID() != NULL_ENTITY)
        {
            TransformComponent transformComponent;
            ecs.GetComponent(selectedEntity, transformComponent);

            Mat4 transformMatrix{};
            ImGuizmo::RecomposeMatrixFromComponents(glm::value_ptr(transformComponent.position), glm::value_ptr(transformComponent.rotation), glm::value_ptr(transformComponent.scale), glm::value_ptr(transformMatrix));

            ImGuizmo::BeginFrame();
            ImGuizmo::SetDrawlist();
            ImGuizmo::Enable(true);

            Camera& camera = RenderingSystem::GetCamera();
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImGuizmo::SetRect(windowPos.x, windowPos.y, m_ContentRegionSize.x, m_ContentRegionSize.y);
            Mat4 yUpProjection = camera.GetProjectionMatrix();
            yUpProjection[1][1] *= -1;
            ImGuizmo::Manipulate(glm::value_ptr(camera.GetViewMatrix()), glm::value_ptr(yUpProjection), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::LOCAL, glm::value_ptr(transformMatrix));

            if (ImGuizmo::IsUsing())
            {
                ImGuizmo::DecomposeMatrixToComponents(
                    glm::value_ptr(transformMatrix),
                    glm::value_ptr(transformComponent.position),
                    glm::value_ptr(transformComponent.rotation),
                    glm::value_ptr(transformComponent.scale)
                );

                ecs.UpdateComponent(selectedEntity, transformComponent);
            }
        }
    }

}
