/**
* @file OrthographicCamera.cpp
* @author Andrew Fagan
* @date 1/14/2025
*/
#include "OrthographicCamera.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/gtc/matrix_transform.hpp"

namespace Astral {

    OrthographicCamera::OrthographicCamera() :
        m_ProjectionMatrix(glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1000.0f, 1000.0f)),
        m_ViewMatrix(1.0f),
        m_ProjectionViewMatrix(1.0f),
        m_Position(0, 0, 0),
        m_Rotation(0.0f)
    {
        RecreateProjectionMatrix();
        CalculateProjectionViewMatrix();
        m_ViewportResizedListener.StartListening();
    }


    OrthographicCamera::OrthographicCamera(float aspectRatio, float zoomLevel)
        : m_ProjectionMatrix(),
        m_ViewMatrix(1.0f),
        m_ProjectionViewMatrix(1.0f),
        m_Position(0, 0, 0),
        m_Rotation(0.0f)
    {
        m_ZoomLevel = zoomLevel;
        m_AspectRatio = aspectRatio;
        RecreateProjectionMatrix();
        CalculateProjectionViewMatrix();
        m_ViewportResizedListener.StartListening();
    }


    OrthographicCamera::~OrthographicCamera()
    {
        m_ViewportResizedListener.StopListening();
    }


    const Mat4& OrthographicCamera::GetProjectionViewMatrix() const
    {
        return m_ProjectionViewMatrix;
    }


    void OrthographicCamera::RecreateProjectionMatrix()
    {
        m_ProjectionMatrix = glm::ortho(-m_AspectRatio / 2 * m_ZoomLevel, m_AspectRatio / 2 * m_ZoomLevel, -m_ZoomLevel / 2, m_ZoomLevel / 2, -1000.0f, 1000.0f);
    }


    void OrthographicCamera::CalculateProjectionViewMatrix()
    {
        Mat4 transform = glm::translate(Mat4(1.0f), m_Position) * glm::rotate(Mat4(1.0f), glm::radians(m_Rotation), Vec3(0, 0, 1));
        m_ViewMatrix = glm::inverse(transform);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

}
