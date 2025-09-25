/**
* @file CameraController.cpp
* @author Andrew Fagan
* @date 9/5/25
*/

#include "CameraController.h"

#include "Core/Engine.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Scene.h"
#include "Input/InputState.h"


namespace Astral {

    static Vec3 initialPosition;
    static Vec2 rotationSpeed = Vec2{0};
    static float sensitivity = .15;
    static float rotationConservationFactor = .85;
    static float translationConservationFactor = .75;
    static float baseAcceleration = 2;
    static DeltaTime deltaTime;




    void CameraControllerComponent()
    {
        Scene& scene = Engine::Get().GetSceneManager().GetActiveScene();
        Camera& primaryCamera = scene.PrimaryCamera;

        static float translationAcceleration = 0;
        static Vec3 translationVelocity = Vec3(0);

        deltaTime.UpdateDeltaTime();
        translationAcceleration = baseAcceleration * deltaTime.GetSeconds();

        if (InputState::IsKeyDown(KEY_LEFT_CLICK))
        {
            translationAcceleration *= 20;
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

        rotationSpeed *= rotationConservationFactor;

        initialPosition = primaryCamera.GetPosition(); // For tracking camera velocity
        Vec3 movingDirection = Vec3(0.0f);

        if (InputState::IsKeyDown(KEY_W))
        {
            Vec3 forwardVec = primaryCamera.GetForwardVector();
            movingDirection += forwardVec;
        }
        if (InputState::IsKeyDown(KEY_S))
        {
            Vec3 forwardVec = primaryCamera.GetForwardVector();
            movingDirection -= forwardVec;
        }
        if (InputState::IsKeyDown(KEY_A))
        {
            Vec3 leftVec = primaryCamera.GetLeftVector();
            movingDirection += leftVec;
        }
        if (InputState::IsKeyDown(KEY_D))
        {
            Vec3 leftVec = primaryCamera.GetLeftVector();
            movingDirection -= leftVec;
        }
        if (InputState::IsKeyDown(KEY_LEFT_SHIFT) || InputState::IsKeyDown(KEY_RIGHT_SHIFT))
        {
            movingDirection.y -= 1.0f;
        }
        if (InputState::IsKeyDown(KEY_SPACE))
        {
            movingDirection.y += 1.0f;
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

        if (movingDirection != Vec3(0.0f))
        {
            translationVelocity += glm::normalize(movingDirection) * translationAcceleration;
        }

        primaryCamera.SetPosition(initialPosition + translationVelocity);

        translationVelocity *= translationConservationFactor;

        CameraPropertiesComponent();
    }

    void CameraPropertiesComponent()
    {
        Scene& scene = Engine::Get().GetSceneManager().GetActiveScene();

        ImGui::Begin("Camera Controller##EditorCameraController");

        Vec3 cameraPosition = scene.PrimaryCamera.GetPosition();
        ImGui::Text("Camera Position: (%.3f, %.3f, %.3f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);

        float framesPerSecond = 1 / deltaTime.GetSeconds();
        Vec3 positionDelta = cameraPosition - initialPosition;
        Vec3 cameraVelocity = positionDelta * framesPerSecond;
        ImGui::Text("Camera Velocity: (%.3f, %.3f, %.3f)", cameraVelocity.x, cameraVelocity.y, cameraVelocity.z);

        Vec3 cameraRotation = scene.PrimaryCamera.GetRotation();
        ImGui::Text("Camera Rotation: (%.3f, %.3f, %.3f)", cameraRotation.x, cameraRotation.y, cameraRotation.z);
        ImGui::Text("Camera Rotation Speed: (%.3f, %.3f)", rotationSpeed.x, rotationSpeed.y);

        float zNear = scene.PrimaryCamera.GetNearPlane();
        ImGui::Text("zNear: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputFloat("##zNearInput", &zNear);
        if (scene.PrimaryCamera.GetNearPlane() != zNear) { scene.PrimaryCamera.SetNearPlane(zNear); }

        float zFar = scene.PrimaryCamera.GetFarPlane();
        ImGui::Text("zFar: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputFloat("##zFarInput", &zFar);
        if (scene.PrimaryCamera.GetFarPlane() != zFar) { scene.PrimaryCamera.SetFarPlane(zFar); }

        ImGui::Text("Base Acceleration: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputFloat("##CameraAccelerationInput", &baseAcceleration);

        ImGui::Text("Translation Conservation Factor: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputFloat("##CameraTranslationConservationFactorInput", &translationConservationFactor);

        ImGui::Text("Mouse Sensitivity: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputFloat("##MouseSensitivityInput", &sensitivity);

        ImGui::Text("Rotation Conservation Factor: ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(-1);
        ImGui::InputFloat("##CameraRotationConservationFactorInput", &rotationConservationFactor);


        ImGui::End();
    }

}
