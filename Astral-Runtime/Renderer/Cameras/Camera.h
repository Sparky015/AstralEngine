/**
* @file OrthographicCamera.h
* @author Andrew Fagan
* @date 1/14/2025
*/

#pragma once

#include "Core/Events/EventListener.h"
#include "Core/Math/Math.h"
#include "Renderer/Common/RendererEvents.h"

namespace Astral {

    enum class CameraType : uint8
    {
        ORTHOGRAPHIC,
        PERSPECTIVE
    };

    /**@class Camera
     * @brief An orthographic camera to use in scenes. */
    class Camera
    {
    public:
        Camera();
        Camera(CameraType cameraType, float aspectRatio, float zoomLevel);
        ~Camera();

        /**@brief Gets the ProjectionView matrix. The matrix is calculated on other calls and cached. */
        const Mat4& GetProjectionViewMatrix() const;

        /**@brief Sets the position of the camera in world space */
        void SetPosition(const Vec3& position)
        {
            m_Position = position;
            CalculateProjectionViewMatrix();
        }

        /**@brief Gets the position of the camera */
        [[nodiscard]] const Vec3& GetPosition() const { return m_Position; }

        /**@brief Sets the rotation of the camera */
        void SetRotation(float rotation)
        {
            m_Rotation = rotation;
            CalculateProjectionViewMatrix();
        }

        /**@brief Gets the rotation of the camera */
        [[nodiscard]] float GetRotation() const { return m_Rotation; }

        void SetZoom(float zoom)
        {
            m_ZoomLevel = zoom;
            RecreateProjectionMatrix();
            CalculateProjectionViewMatrix();
        }

        float GetZoomLevel() const { return m_ZoomLevel; }

    private:

        void RecreateProjectionMatrix();
        void CalculateProjectionViewMatrix();

        CameraType m_CameraType;

        Mat4 m_ProjectionMatrix; // Contains aspect ratio and zoom
        Mat4 m_ViewMatrix; // Kinda like a world transformation that moves the whole scene
        Mat4 m_ProjectionViewMatrix; // Cache for projection-view matrix

        Vec3 m_Position;
        float m_Rotation = 0.0f;
        float m_AspectRatio = 1.0f;

        union
        {
            float m_ZoomLevel; // Orthographic
            float m_POV; // Perspective
        };

        float m_Pitch = 0.0f; // Rotation around X-axis (looking up/down)
        float m_Yaw = 0.0f;   // Rotation around Y-axis (looking left/right)
        float m_Roll = 0.0f; // Optional: Rotation around Z-axis (tilting)

        EventListener<ViewportResizedEvent> m_ViewportResizedListener{[this](ViewportResizedEvent e)
        {
            m_AspectRatio = (float)e.Width / (float)e.Height;
            RecreateProjectionMatrix();
            CalculateProjectionViewMatrix();
        }};
    };

}
