/**
* @file Camera.cpp
* @author Andrew Fagan
* @date 1/14/2025
*/
#include "Camera.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/gtc/matrix_transform.hpp"

namespace Astral {

    Camera::Camera() :
        m_CameraType(CameraType::ORTHOGRAPHIC),
        m_ProjectionMatrix(),
        m_ViewMatrix(1.0f),
        m_ProjectionViewMatrix(1.0f),
        m_Position(0, 0, 0),
        m_Rotation(0.0f)
    {
        m_ZoomLevel = 1.0f;

        RecreateProjectionMatrix();
        CalculateProjectionViewMatrix();
        m_ViewportResizedListener.StartListening();
    }


    Camera::Camera(CameraType cameraType, float aspectRatio, float zoomLevel) :
        m_CameraType(cameraType),
        m_ProjectionMatrix(),
        m_ViewMatrix(1.0f),
        m_ProjectionViewMatrix(1.0f),
        m_Position(0, 0, 0),
        m_Rotation(0.0f)
    {
        if (m_CameraType == CameraType::PERSPECTIVE)
        {
            m_POV = 60.0f;
        }
        else if (m_CameraType == CameraType::ORTHOGRAPHIC)
        {
            m_ZoomLevel = zoomLevel;
        }

        m_AspectRatio = aspectRatio;
        RecreateProjectionMatrix();
        CalculateProjectionViewMatrix();
        m_ViewportResizedListener.StartListening();
    }


    Camera::~Camera()
    {
        m_ViewportResizedListener.StopListening();
    }


    const Mat4& Camera::GetProjectionViewMatrix() const
    {
        return m_ProjectionViewMatrix;
    }


    void Camera::RecreateProjectionMatrix()
    {
        if (m_CameraType == CameraType::PERSPECTIVE)
        {
            m_ProjectionMatrix = glm::perspective(glm::radians(m_POV), m_AspectRatio, .1f, 1000.0f);
        }
        else if (m_CameraType == CameraType::ORTHOGRAPHIC)
        {
            m_ProjectionMatrix = glm::ortho(-m_AspectRatio / 2 * m_ZoomLevel, m_AspectRatio / 2 * m_ZoomLevel, -m_ZoomLevel / 2, m_ZoomLevel / 2, -1000.0f, 1000.0f);
        }
    }


    void Camera::CalculateProjectionViewMatrix()
    {
        Mat4 orientation;

        if (m_CameraType == CameraType::PERSPECTIVE)
        {
            Mat4 rotationX = glm::rotate(Mat4(1.0f), glm::radians(m_Pitch), Vec3(1, 0, 0)); // Rotate around X
            Mat4 rotationY = glm::rotate(Mat4(1.0f), glm::radians(m_Yaw), Vec3(0, 1, 0));   // Rotate around Y
            Mat4 rotationZ = glm::rotate(Mat4(1.0f), glm::radians(m_Roll), Vec3(0, 0, 1)); // Rotate around Z

            orientation = rotationZ * rotationY * rotationX;
        }
        else if (m_CameraType == CameraType::ORTHOGRAPHIC)
        {
            orientation = glm::rotate(Mat4(1.0f), glm::radians(m_Rotation), Vec3(0, 0, 1));
        }


        Mat4 transform = glm::translate(Mat4(1.0f), m_Position) * orientation;
        m_ViewMatrix = glm::inverse(transform);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

}
