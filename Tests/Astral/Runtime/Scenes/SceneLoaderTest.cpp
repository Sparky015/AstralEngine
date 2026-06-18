/**
* @file SceneLoaderTest.cpp
* @author Andrew Fagan
* @date 9/22/25
*/

#include "gtest/gtest.h"
#include "gtest/gtest-param-test.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Scenes/Scene.h"
#include "Scenes/SceneManager.h"

namespace Astral {

class SceneLoaderTest : public ::testing::TestWithParam<const char*>
{
public:

    void SetUp() override
    {
        Engine::Get().GetAssetManager().GetRegistry().SetAssetDirectoryPath(ASTRAL_TEST_SCENES_DIRECTORY);
        Engine::Get().GetSceneManager().CreateEmptyScene();
        RendererAPI::GetContext().ClearNumValidationErrorsAndWarnings();
    }
};

TEST_P(SceneLoaderTest, LoadScene_LoadsSceneAndAssetsCorrectlyWithoutError)
{
    ASSERT_EQ(Engine::Get().GetSceneManager().GetECS().GetNumberOfActiveEntities(), 0);

    const char* path = GetParam();
    Engine::Get().GetSceneManager().LoadScene(path);
    ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
}

INSTANTIATE_TEST_SUITE_P(
    SceneLoaderTestPaths,
    SceneLoaderTest,
    ::testing::Values(
        "Cerberus/Cerberus.aescene",
        "WaterBendingGauntlet/WaterBendingGauntlet.aescene"
    )
);


}
