/**
* @file OrthographicCamera.cpp
* @author Andrew Fagan
* @date 1/15/2025
*/

#include "gtest/gtest.h"

#include "Renderer/Cameras/OrthographicCameraTest.h"

namespace Graphics {

    class OrthographicCameraTests : public ::testing::Test
    {
    public:

    };

    /**@brief Tests that the constructor defaults the position and rotation to what they need to be */
    TEST_F(OrthographicCameraTests, Constructor_InitializesCorrectly)
    {
        OrthographicCameraTest camera(-10.0f, 10.0f, -10.0f, 10.0f);

        EXPECT_EQ(camera.GetPosition(), Vec3(0.0f, 0.0f, 0.0f)); // Assuming default position is (0, 0, 0)
        EXPECT_EQ(camera.GetRotation(), 0.0f); // Assuming default rotation is 0
    }

    /**@brief Tests that the position is set properly and that the projection view matrix is updated. */
    TEST_F(OrthographicCameraTests, SetPosition_UpdatesPosition) {
        OrthographicCameraTest camera(-10.0f, 10.0f, -10.0f, 10.0f);
        Mat4 oldProjectionViewMatrix = camera.GetProjectionViewMatrix();

        Vec3 newPosition(1.0f, 2.0f, 3.0f);
        camera.SetPosition(newPosition);
        EXPECT_EQ(camera.GetPosition(), newPosition);
        EXPECT_NE(oldProjectionViewMatrix, camera.GetProjectionViewMatrix());
    }

    /**@brief Tests that the rotation is set properly and that the projection view matrix is updated. */
    TEST_F(OrthographicCameraTests, SetRotation_UpdatesRotation) {
        OrthographicCameraTest camera(-10.0f, 10.0f, -10.0f, 10.0f);
        Mat4 oldProjectionViewMatrix = camera.GetProjectionViewMatrix();

        float newRotation = 45.0f;
        camera.SetRotation(newRotation);
        EXPECT_EQ(camera.GetRotation(), newRotation);
        EXPECT_NE(oldProjectionViewMatrix, camera.GetProjectionViewMatrix());
    }

}