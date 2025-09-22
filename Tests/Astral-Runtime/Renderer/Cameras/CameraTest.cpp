/**
* @file CameraTest.cpp
* @author Andrew Fagan
* @date 1/15/2025
*/

#include "gtest/gtest.h"

#include "Renderer/Cameras/Camera.h"

namespace Astral {

class CameraTests : public ::testing::Test
{
public:

};

/**@brief Tests that the constructor defaults the position and rotation to what they need to be */
TEST_F(CameraTests, Constructor_InitializesCorrectly)
{
    Camera camera(CameraType::PERSPECTIVE, 1.0, 800.0f);

    EXPECT_EQ(camera.GetPosition(), Vec3(0.0f, 0.0f, 0.0f)); // Assuming default position is (0, 0, 0)
    EXPECT_EQ(camera.GetRotation(), Vec3(0.0f)); // Assuming default rotation is 0
}

/**@brief Tests that the position is set properly and that the projection view matrix is updated. */
TEST_F(CameraTests, SetPosition_UpdatesPosition) {
    Camera camera(CameraType::PERSPECTIVE, 1.0, 800.0f);
    Mat4 oldProjectionViewMatrix = camera.GetProjectionViewMatrix();

    Vec3 newPosition(1.0f, 2.0f, 3.0f);
    camera.SetPosition(newPosition);
    EXPECT_EQ(camera.GetPosition(), newPosition);
    EXPECT_NE(oldProjectionViewMatrix, camera.GetProjectionViewMatrix());
}

/**@brief Tests that the rotation is set properly and that the projection view matrix is updated. */
TEST_F(CameraTests, SetRotation_UpdatesRotation) {
    Camera camera(CameraType::PERSPECTIVE, 1.0, 800.0f);
    Mat4 oldProjectionViewMatrix = camera.GetProjectionViewMatrix();

    float newRotation = 45.0f;
    camera.SetRotation(Vec3(newRotation));
    EXPECT_EQ(camera.GetRotation(), Vec3(newRotation));
    EXPECT_NE(oldProjectionViewMatrix, camera.GetProjectionViewMatrix());
}

}