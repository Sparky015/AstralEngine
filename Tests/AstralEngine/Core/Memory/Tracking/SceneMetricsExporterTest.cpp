/**
* @file SceneMetricsAccumulatorTest.cpp
* @author Andrew Fagan
* @date 2/10/2025
*/

#include <gtest/gtest.h>

#include "Core/Memory/Tracking/SceneMetricsExporter.h"

class SceneMetricsAccumulatorTest : public ::testing::Test
{
public:
    Core::SceneMetricsExporter m_SceneMetricsAccumulator;
};

TEST_F(SceneMetricsAccumulatorTest, BeginScene_SceneBecomesActive)
{
    m_SceneMetricsAccumulator.BeginScene("test");
    EXPECT_TRUE(m_SceneMetricsAccumulator.IsSceneActive());
}

TEST_F(SceneMetricsAccumulatorTest, BeginScene_ThrowsWhenBeginningSceneDuringActiveScene)
{
    EXPECT_NO_THROW(m_SceneMetricsAccumulator.BeginScene("test"));
    EXPECT_TRUE(m_SceneMetricsAccumulator.IsSceneActive());
    EXPECT_ANY_THROW(m_SceneMetricsAccumulator.BeginScene("new_test"));
}

TEST_F(SceneMetricsAccumulatorTest, EndScene_SceneBecomesInactive)
{
    m_SceneMetricsAccumulator.EndScene();
    EXPECT_FALSE(m_SceneMetricsAccumulator.IsSceneActive());
}

TEST_F(SceneMetricsAccumulatorTest, EndScene_EndingInactiveSceneDoesNothing)
{
    EXPECT_NO_THROW(m_SceneMetricsAccumulator.BeginScene("test"));
    EXPECT_NO_THROW(m_SceneMetricsAccumulator.EndScene());
    EXPECT_FALSE(m_SceneMetricsAccumulator.IsSceneActive());
    EXPECT_NO_THROW(m_SceneMetricsAccumulator.EndScene());
    EXPECT_FALSE(m_SceneMetricsAccumulator.IsSceneActive());
}