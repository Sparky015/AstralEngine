/**
* @file Camera.h
* @author Andrew Fagan
* @date 1/14/2025
*/

#pragma once

#include "Core/Events/EventListener.h"
#include "Core/Math/Math.h"
#include "Renderer/Common/RendererEvents.h"

namespace Astral {

    /**
     * @enum CameraType Type of camera projections
     * @brief The type of camera projection
     */
    enum class CameraType : uint8
    {
        ORTHOGRAPHIC,
        PERSPECTIVE
    };

    /**
     * @class Camera
     * @brief A camera to use in scenes.
     */
    class Camera
    {
    public:

        /**
         * @brief Default constructor for camera
         * @note Camera type defaults to Orthographic
         */
        Camera();

        /**
         * @brief Constructor to zNear and zFar for Perspective camera type
         * @param cameraType The camera type to make the camera
         * @param aspectRatio The aspect ratio of the camera
         * @param zNear The Z near to set
         * @param zFar The Z far to set
         */
        Camera(CameraType cameraType, float aspectRatio, float zNear, float zFar);

        /**
         * @brief Constructor to specify zoom for Orthographic camera type
         * @param cameraType The camera type to make the camera
         * @param aspectRatio The aspect ratio of the camera
         * @param zoomLevel The zoom level to set
         */
        Camera(CameraType cameraType, float aspectRatio, float zoomLevel);

        /**
         * @brief Destructor for camera
         */
        ~Camera();

        /**
         * @brief Gets the ViewProjection matrix
         * @return The ViewProjection matrix
         */
        const Mat4& GetProjectionViewMatrix() const;

        /**
         * @brief Sets the position of the camera in world space
         * @param position The position to set the camera to
         */
        void SetPosition(const Vec3& position);

        /**
         * @brief Gets the position of the camera
         * @return The position of the camera
         */
        [[nodiscard]] const Vec3& GetPosition() const { return m_Position; }

        /**
         * @brief Sets the rotation of the camera
         * @param rotation The rotation to set the camera to
         */
        void SetRotation(const Vec3& rotation);

        /**
         * @brief Gets the rotation of the camera
         * @return The rotation of the camera
         */
        [[nodiscard]] const Vec3& GetRotation() const { return m_Rotation; }

        /**
         * @brief Sets the zoom of an orthographic camera
         * @param zoom The zoom level to set the camera to
         * @note Does nothing if camera type is not orthographic
         */
        void SetZoom(float zoom);

        /**
         * @brief Gets the zoom level of an orthographic camera
         * @return The zoom level
         * @warning Returns -1 if the camera type is not orthographic
         */
        [[nodiscard]] float GetZoomLevel() const;

        /**
         * @brief Sets the z near value for the camera
         * @param zNear The z near value for the camera to use
         */
        void SetNearPlane(float zNear);

        /**
         * @brief Sets the z far value for the camera
         * @param zFar The z far value for the camera to use
         */
        void SetFarPlane(float zFar);

        /**
         * @brief Gets the z near of the camera
         * @return The z near value
         */
        [[nodiscard]] float GetNearPlane() const { return m_ZNear; }

        /**
         * @brief Gets the z far of the camera
         * @return The z far value
         */
        [[nodiscard]] float GetFarPlane() const { return m_ZFar; }

        /**
         * @brief Gets the aspect ratio of the camera
         * @return The aspect ratio of the camera
         */
        [[nodiscard]] float GetAspectRatio() const { return m_AspectRatio; }

        /**
         * @brief Gets the vector in the direction that the camera is looking in
         * @return A vector in the direction of where the camera is looking in
         */
        [[nodiscard]] Vec3 GetForwardVector() const;

        /**
         * @brief Gets the vector in the direction that the camera is looking in but rotated 90 degrees to the left
         * @return A vector in the direction of where the camera is looking in but rotated 90 degrees to the left
         */
        [[nodiscard]] Vec3 GetLeftVector() const;

        /**
         * @brief Gets the vector in the direction that the camera is looking in but rotated 90 degrees to the right
         * @return A vector in the direction of where the camera is looking in but rotated 90 degrees to the right
         */
        [[nodiscard]] Vec3 GetRightVector() const;

        /**
         * @brief Gets the vector in the direction that the camera is looking in
         * @return A vector in the direction of where the camera is looking in
         */
        [[nodiscard]] const Mat4& GetViewMatrix() const { return m_ViewMatrix; }

        /**
         * @brief Gets the projection matrix of the camera
         * @return The projection matrix of the camera
         */
        [[nodiscard]] const Mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

        /**
         * @brief Copy constructor for Camera
         * @param other The camera to copy when constructing an instance
         */
        Camera(const Camera& other);

        /**
         * @brief Copy assignment operator for Camera
         * @param other The camera to copy to this instance
         */
        Camera& operator=(const Camera& other);

        Camera(Camera&&) noexcept = delete;
        Camera& operator=(Camera&&) noexcept = delete;

    private:

        /**
         * @brief Recreates the projection matrix
         */
        void RecreateProjectionMatrix();

        /**
         * @brief Calculates the view projection matrix and caches it
         */
        void CalculateViewProjectionMatrix();

        /**
         * @brief Resizes the camera when the rendering viewport is resized
         * @param e The viewport resized event data
         */
        void ResizeCamera(const ViewportResizedEvent& e);

        CameraType m_CameraType;

        Mat4 m_ProjectionMatrix; // Contains aspect ratio and zoom
        Mat4 m_ViewMatrix; // Kinda like a world transformation that moves the whole scene
        Mat4 m_ViewProjection; // Cache for projection-view matrix

        Vec3 m_Position;
        Vec3 m_Rotation;
        float m_AspectRatio = 1.0f;
        float m_ZNear;
        float m_ZFar;

        union
        {
            float m_ZoomLevel; // Orthographic
            float m_POV;       // Perspective
        };

        EventListener<ViewportResizedEvent> m_ViewportResizedListener{[this](ViewportResizedEvent e)
        {
            ResizeCamera(e);
        }};
    };

}
