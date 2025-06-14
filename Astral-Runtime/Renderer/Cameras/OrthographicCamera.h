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

    /**@class OrthographicCamera
     * @brief An orthographic camera to use in scenes. */
    class OrthographicCamera
    {
    public:
        OrthographicCamera();
        OrthographicCamera(float aspectRatio, float zoomLevel);
        ~OrthographicCamera();

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

        Mat4 m_ProjectionMatrix; // Contains aspect ratio and fov
        Mat4 m_ViewMatrix; // Kinda like a world transformation that moves the whole scene
        Mat4 m_ProjectionViewMatrix; // Cache for projection matrix

        Vec3 m_Position;
        float m_Rotation = 0.0f;
        float m_ZoomLevel = 1.0f;
        float m_AspectRatio = 1.0f;

        EventListener<ViewportResizedEvent> m_ViewportResizedListener{[this](ViewportResizedEvent e)
        {
            m_AspectRatio = (float)e.Width / (float)e.Height;
            RecreateProjectionMatrix();
            CalculateProjectionViewMatrix();
        }};
    };

}
