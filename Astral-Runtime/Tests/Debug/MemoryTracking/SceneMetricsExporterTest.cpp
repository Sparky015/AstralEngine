/**
* @file SceneMetricsExporterTest.cpp
* @author Andrew Fagan
* @date 2/10/2025
*/

#include <gtest/gtest.h>

#include "Debug/MemoryTracking/Serialization/SceneMetricsExporter.h"

class SceneMetricsAccumulatorTest : public ::testing::Test
{
public:
    Core::SceneMetricsExporter m_SceneMetricsExporter;
};

TEST_F(SceneMetricsAccumulatorTest, BeginScene_SceneBecomesActive)
{
    (void)m_SceneMetricsExporter.BeginScene("test");
    EXPECT_TRUE(m_SceneMetricsExporter.IsSceneActive());
}

TEST_F(SceneMetricsAccumulatorTest, BeginScene_ThrowsWhenBeginningSceneDuringActiveScene)
{
    EXPECT_NO_THROW((void)m_SceneMetricsExporter.BeginScene("test"));
    EXPECT_TRUE(m_SceneMetricsExporter.IsSceneActive());
    EXPECT_ANY_THROW((void)m_SceneMetricsExporter.BeginScene("new_test"));
}

TEST_F(SceneMetricsAccumulatorTest, EndScene_SceneBecomesInactive)
{
    m_SceneMetricsExporter.EndScene();
    EXPECT_FALSE(m_SceneMetricsExporter.IsSceneActive());
}

TEST_F(SceneMetricsAccumulatorTest, EndScene_EndingInactiveSceneDoesNothing)
{
    EXPECT_NO_THROW((void)m_SceneMetricsExporter.BeginScene("test"));
    EXPECT_NO_THROW(m_SceneMetricsExporter.EndScene());
    EXPECT_FALSE(m_SceneMetricsExporter.IsSceneActive());
    EXPECT_NO_THROW(m_SceneMetricsExporter.EndScene());
    EXPECT_FALSE(m_SceneMetricsExporter.IsSceneActive());
}