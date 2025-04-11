/**
* @file OrthographicCamera.h
* @author Andrew Fagan
* @date 1/14/2025
*/

#pragma once

#include "Core/Math/Math.h"

namespace Astral {

    /**@class OrthographicCamera
     * @brief An orthographic camera to use in scenes. */
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top, float near = -1, float far = 1);

        /**@brief Gets the ProjectionView matrix. The matrix is calculated on each call. */
        const Mat4& GetProjectionViewMatrix();

        /**@brief Sets the position of the camera in world space */
        void SetPosition(const Vec3& position)
        {
            m_Position = position;
            CalculateViewProjectionMatrix();
        }

        /**@brief Gets the position of the camera */
        [[nodiscard]] const Vec3& GetPosition() const { return m_Position; }

        /**@brief Sets the rotation of the camera */
        void SetRotation(float rotation)
        {
            m_Rotation = rotation;
            CalculateViewProjectionMatrix();
        }

        /**@brief Gets the rotation of the camera */
        [[nodiscard]] float GetRotation() const { return m_Rotation; }

    private:

        void CalculateViewProjectionMatrix();

        Mat4 m_ProjectionMatrix; // Contains aspect ratio and fov
        Mat4 m_ViewMatrix; // Kinda like a world transformation that moves the whole scene
        Mat4 m_ProjectionViewMatrix; // Cache for projection matrix

        Vec3 m_Position;
        float m_Rotation = 0.0f;
    };

}