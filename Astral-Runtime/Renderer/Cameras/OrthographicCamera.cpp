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
        m_ProjectionMatrix(glm::ortho(0.0f, 800.0f, 0.0f, 800.0f, -1000.0f, 1000.0f)), // Fits 0-800 range
        m_ViewMatrix(1.0f),
        m_ProjectionViewMatrix(1.0f),
        m_Position(0, 0, 0),
        m_Rotation(0.0f)
    {
        CalculateProjectionViewMatrix();
    }


    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near, float far)
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, near, far)),
        m_ViewMatrix(1.0f),
        m_ProjectionViewMatrix(1.0f),
        m_Position(0, 0, 0),
        m_Rotation(0.0f)
    {
        CalculateProjectionViewMatrix();
    }


    const Mat4& OrthographicCamera::GetProjectionViewMatrix() const
    {
        return m_ProjectionViewMatrix;
    }


    void OrthographicCamera::CalculateProjectionViewMatrix()
    {
        Mat4 transform = glm::translate(Mat4(1.0f), m_Position) * glm::rotate(Mat4(1.0f), glm::radians(m_Rotation), Vec3(0, 0, 1));
        m_ViewMatrix = glm::inverse(transform);
        m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

}
