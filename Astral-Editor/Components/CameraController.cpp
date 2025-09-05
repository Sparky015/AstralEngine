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

    void CameraControllerComponent()
    {
        Scene& scene = Engine::Get().GetSceneManager().GetActiveScene();
        Camera& primaryCamera = scene.PrimaryCamera;

        // TODO Make a templated iterator that does the below checking on iterations and holds the state of which ID it is at
        static float magnitude = 2;
        static Vec2 initialMousePos{};
        static Vec3 initialRotation{};
        static bool isReadyToTrack = true;
        static DeltaTime deltaTime;

        deltaTime.UpdateDeltaTime();
        magnitude = 8 * deltaTime.GetSeconds();

        if (InputState::IsKeyDown(KEY_LEFT_CLICK))
        {
            magnitude *= 20;
        }


        float rotationSpeed = 1;
        float rotationSpeedX = 0;
        float rotationSpeedY = 0;
        if (InputState::IsKeyDown(KEY_RIGHT_CLICK))
        {
            Vec2 lastMousePosition = InputState::MousePosition();
            Vec2 currentMousePos = InputState::MousePosition();

            rotationSpeedX = (lastMousePosition.x - currentMousePos.x) * 500;
            rotationSpeedY = (lastMousePosition.y - currentMousePos.y) * 500;
        }

        if (rotationSpeed > 0)
        {
            Vec3 currentRotation = primaryCamera.GetRotation(); // Position
            Vec3 newRotation = {currentRotation.x - rotationSpeedY, currentRotation.y + rotationSpeedX, currentRotation.z};
            primaryCamera.SetRotation(newRotation);
        }


        // if (InputState::IsKeyDown(KEY_RIGHT_CLICK))
        // {
        //     if (isReadyToTrack)
        //     {
        //         initialMousePos = InputState::MousePosition();
        //         initialRotation = camera.GetRotation();
        //         isReadyToTrack = false;
        //     }
        //
        //     Vec2 currentMousePos = InputState::MousePosition();
        //     xDelta = (initialMousePos.x - currentMousePos.x) * .5;
        //     yDelta = (initialMousePos.y - currentMousePos.y) * .5;
        //
        //     Vec3 newRotation = {initialRotation.x - yDelta, initialRotation.y + xDelta, initialRotation.z};
        //     camera.SetRotation(newRotation);
        // }
        // else
        // {
        //     isReadyToTrack = true;
        // }

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
    }

}
