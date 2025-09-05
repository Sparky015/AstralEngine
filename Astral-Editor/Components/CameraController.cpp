/**
* @file CameraController.cpp
* @author Andrew Fagan
* @date 9/5/25
*/

#include "CameraController.h"

#include "Core/Engine.h"
#include "ECS/SceneManager.h"
#include "Scenes/Scene.h"
#include "Input/InputState.h"


namespace Astral {

    static Vec3 initialPosition;
    static Vec2 rotationSpeed = Vec2{0};
    static float sensitivity = .5;
    static float rotationDrag = .85;
    static float baseSpeed = 8;


    void CameraControllerComponent()
    {
        Scene& scene = Engine::Get().GetSceneManager().GetActiveScene();
        Camera& primaryCamera = scene.PrimaryCamera;

        // TODO Make a templated iterator that does the below checking on iterations and holds the state of which ID it is at
        static float magnitude = 2;
        static DeltaTime deltaTime;

        deltaTime.UpdateDeltaTime();
        magnitude = baseSpeed * deltaTime.GetSeconds();

        if (InputState::IsKeyDown(KEY_LEFT_CLICK))
        {
            magnitude *= 20;
        }

        static Vec2 lastMousePosition{};
        static bool wasKeyJustPressed = true;
        if (InputState::IsKeyDown(KEY_RIGHT_CLICK))
        {
            if (wasKeyJustPressed)
            {
                lastMousePosition = InputState::MousePosition();
                wasKeyJustPressed = false;
            }
            Vec2 currentMousePos = InputState::MousePosition();

            rotationSpeed.x += (lastMousePosition.x - currentMousePos.x) * sensitivity;
            rotationSpeed.y += (lastMousePosition.y - currentMousePos.y) * sensitivity;

            lastMousePosition = currentMousePos;
        }
        else
        {
            wasKeyJustPressed = true;
        }

        Vec3 currentRotation = primaryCamera.GetRotation(); // Position
        Vec3 newRotation = {currentRotation.x - rotationSpeed.y, currentRotation.y + rotationSpeed.x, currentRotation.z};
        primaryCamera.SetRotation(newRotation);

        rotationSpeed *= rotationDrag;



        initialPosition = primaryCamera.GetPosition(); // For tracking camera velocity

        if (InputState::IsKeyDown(KEY_W))
        {

            Vec3 forwardVec = primaryCamera.GetForwardVector();
            forwardVec *= magnitude;
            const Vec3& position = primaryCamera.GetPosition();
            Vec3 newPosition = position + forwardVec;

            primaryCamera.SetPosition(Vec3(newPosition.x, position.y, newPosition.z));
        }
        if (InputState::IsKeyDown(KEY_S))
        {
            Vec3 forwardVec = primaryCamera.GetForwardVector();
            forwardVec *= magnitude;
            const Vec3& position = primaryCamera.GetPosition();
            Vec3 newPosition = position - forwardVec;

            primaryCamera.SetPosition(Vec3(newPosition.x, position.y, newPosition.z));
        }
        if (InputState::IsKeyDown(KEY_A))
        {
            Vec3 leftVec = primaryCamera.GetLeftVector();
            leftVec *= magnitude;
            const Vec3& position = primaryCamera.GetPosition();
            Vec3 newPosition = position + leftVec;

            primaryCamera.SetPosition(Vec3(newPosition.x, position.y, newPosition.z));
        }
        if (InputState::IsKeyDown(KEY_D))
        {
            Vec3 leftVec = primaryCamera.GetLeftVector();
            leftVec *= magnitude;
            const Vec3& position = primaryCamera.GetPosition();
            Vec3 newPosition = position - leftVec;

            primaryCamera.SetPosition(Vec3(newPosition.x, position.y, newPosition.z));
        }
        if (InputState::IsKeyDown(KEY_LEFT_SHIFT) || InputState::IsKeyDown(KEY_RIGHT_SHIFT))
        {
            const Vec3& position = primaryCamera.GetPosition();
            primaryCamera.SetPosition(Vec3(position.x, position.y - magnitude, position.z));
        }
        if (InputState::IsKeyDown(KEY_SPACE))
        {
            const Vec3& position = primaryCamera.GetPosition();
            primaryCamera.SetPosition(Vec3(position.x, position.y + magnitude, position.z));
        }


        if (InputState::IsKeyDown(KEY_H))
        {
            Vec3 rotation = primaryCamera.GetRotation();
            rotation.z += 1;
            primaryCamera.SetRotation(rotation);
        }

        if (InputState::IsKeyDown(KEY_G))
        {
            Vec3 rotation = primaryCamera.GetRotation();
            rotation.z -= 1;
            primaryCamera.SetRotation(rotation);
        }

        if (InputState::IsKeyDown(KEY_T))
        {
            const float zoom = primaryCamera.GetZoomLevel();
            primaryCamera.SetZoom(zoom + 5);
        }

        if (InputState::IsKeyDown(KEY_Y))
        {
            const float zoom = primaryCamera.GetZoomLevel();
            primaryCamera.SetZoom(zoom - 5);
        }

        CameraPropertiesComponent();
    }

    void CameraPropertiesComponent()
    {
        Scene& scene = Engine::Get().GetSceneManager().GetActiveScene();

        ImGui::Begin("Camera Controller##EditorCameraController");

        Vec3 cameraPosition = scene.PrimaryCamera.GetPosition();
        ImGui::Text("Camera Position: (%.3f, %.3f, %.3f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);
        ImGui::Text("Camera Velocity: %.3f m/s", glm::length(initialPosition - cameraPosition));

        Vec3 cameraRotation = scene.PrimaryCamera.GetRotation();
        ImGui::Text("Camera Rotation: (%.3f, %.3f, %.3f)", cameraRotation.x, cameraRotation.y, cameraRotation.z);
        ImGui::Text("Camera Rotation Speed: (%.3f, %.3f)", rotationSpeed.x, rotationSpeed.y);

        ImGui::Text("Camera Base Speed: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputFloat("##CameraSpeedInput", &baseSpeed);

        ImGui::Text("Mouse Sensitivity: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputFloat("##MouseSensitivityInput", &sensitivity);

        ImGui::Text("Camera Drag Factor: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputFloat("##CameraDragFactorInput", &rotationDrag);


        ImGui::End();
    }

}
