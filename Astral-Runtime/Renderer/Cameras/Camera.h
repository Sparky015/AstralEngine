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
        Camera(CameraType cameraType, float aspectRatio, float zNear, float zFar);
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
        void SetRotation(const Vec3& rotation)
        {
            m_Rotation = rotation;
            CalculateProjectionViewMatrix();
        }

        /**@brief Gets the rotation of the camera */
        [[nodiscard]] const Vec3& GetRotation() const { return m_Rotation; }

        void SetZoom(float zoom)
        {
            m_ZoomLevel = zoom;
            RecreateProjectionMatrix();
            CalculateProjectionViewMatrix();
        }

        float GetZoomLevel() const { return m_ZoomLevel; }

        void SetNearPlane(float zNear)
        {
            m_ZNear = zNear;
            RecreateProjectionMatrix();
            CalculateProjectionViewMatrix();
        }

        void SetFarPlane(float zFar)
        {
            m_ZFar = zFar;
            RecreateProjectionMatrix();
            CalculateProjectionViewMatrix();
        }

        float GetNearPlane() { return m_ZNear; }
        float GetFarPlane() { return m_ZFar; }
        float GetAspectRatio() { return m_AspectRatio; }

        Vec3 GetForwardVector() const;
        Vec3 GetLeftVector() const;
        Vec3 GetRightVector() const;
        const Mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const Mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

        Camera(const Camera&);
        Camera& operator=(const Camera&);
        Camera(Camera&&) noexcept = delete;
        Camera& operator=(Camera&&) noexcept = delete;

    private:

        void RecreateProjectionMatrix();
        void CalculateProjectionViewMatrix();

        CameraType m_CameraType;

        Mat4 m_ProjectionMatrix; // Contains aspect ratio and zoom
        Mat4 m_ViewMatrix; // Kinda like a world transformation that moves the whole scene
        Mat4 m_ProjectionViewMatrix; // Cache for projection-view matrix

        Vec3 m_Position;
        Vec3 m_Rotation;
        float m_AspectRatio = 1.0f;
        float m_ZNear;
        float m_ZFar;

        union
        {
            float m_ZoomLevel; // Orthographic
            float m_POV; // Perspective
        };

        void ResizeCamera(const ViewportResizedEvent& e)
        {
            m_AspectRatio = (float)e.Width / (float)e.Height;
            RecreateProjectionMatrix();
            CalculateProjectionViewMatrix();
        }

        EventListener<ViewportResizedEvent> m_ViewportResizedListener{[this](ViewportResizedEvent e)
        {
            ResizeCamera(e);
        }};
    };

}
