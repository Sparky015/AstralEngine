/**
* @file VertexBufferTest.cpp
* @author Andrew Fagan
* @date 9/21/25
*/

#include "gtest/gtest.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Resources/VertexBuffer.h"

namespace Astral {

    class VertexBufferTests : public ::testing::TestWithParam<std::tuple<std::pair<std::vector<float>, VertexBufferLayout>, GPUMemoryType>>
    {
    public:

        void SetUp() override
        {
            RendererAPI::GetContext().ClearNumValidationErrorsAndWarnings();
        }

    };

    TEST_P(VertexBufferTests, Constructor_CorrectlyCreatesBufferWithDataPopulated)
    {
        {
            auto& [dataBufferLayoutPair, memoryType] = GetParam();
            VertexBufferLayout bufferLayout = dataBufferLayoutPair.second;
            std::vector<float> vertexData = dataBufferLayoutPair.first;
            VertexBufferHandle vertexBuffer = RendererAPI::GetDevice().CreateVertexBuffer(vertexData.data(), sizeof(vertexData), bufferLayout, memoryType);

            ASSERT_NE(vertexBuffer, nullptr);
            ASSERT_NE(vertexBuffer->GetNativeHandle(), nullptr);
        }
        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }

    TEST_P(VertexBufferTests, Constructor_ConstructsBufferAndReadsBackDataCorrectly)
    {
        {
            auto& [dataBufferLayoutPair, memoryType] = GetParam();
            VertexBufferLayout bufferLayout = dataBufferLayoutPair.second;
            std::vector<float> vertexData = dataBufferLayoutPair.first;
            VertexBufferHandle vertexBuffer = RendererAPI::GetDevice().CreateVertexBuffer(vertexData.data(), sizeof(vertexData), bufferLayout, memoryType);

            ASSERT_NE(vertexBuffer, nullptr);
            ASSERT_NE(vertexBuffer->GetNativeHandle(), nullptr);

            if (memoryType == GPUMemoryType::DEVICE_LOCAL)
            {
                vertexBuffer->ChangeMemoryType(GPUMemoryType::HOST_VISIBLE);
            }

            void* bufferVertexData;
            vertexBuffer->MapPointer(&bufferVertexData);
            ASSERT_EQ(memcmp(vertexData.data(), bufferVertexData, sizeof(vertexData)), 0);
            vertexBuffer->UnmapPointer();
        }

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }

    INSTANTIATE_TEST_SUITE_P(
        VertexBufferTestsParams,
        VertexBufferTests,
        ::testing::Combine(
            ::testing::Values(
                std::pair<std::vector<float>, VertexBufferLayout>({0, 1, 2, 4, 5, 6, 3, 6, 1, 6, 8, 9, 1, 6, 7},
                                                                  {{Float3, "TestVec3"}, {Float2, "TestVec2"}}),
                std::pair<std::vector<float>, VertexBufferLayout>({1, 3, 4, 12, 3344, 15125, 12512, 111, 63212, 6666, 121, 0},
                                                                  {{Float4, "TestVec4"}}),
                std::pair<std::vector<float>, VertexBufferLayout>({1, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 12, 34, 1, 46788, 12512, 1252, 12521, 25125, 12512},
                                                                 {{Float4, "TestVec4"}, {Float3, "TestVec3"}}),
                std::pair<std::vector<float>, VertexBufferLayout>({112, 4241, 123124, 566, 333, 33, 3, 1231, 568, 3785, 4125, 1352, 23576, 12451, 2225, 22,
                                                                   44212, 124332, 123123, 123, 12312, 25125, 563, 35135, 235231, 12435, 5356, 345235, 2352,
                                                                   435235, 12412, 12452, 6785, 87654, 124, 242, 779, 12, 52, 1, 5677, 3124, 1253, 6325, 43},
                                                                  {{Float4, "TestVec4"}, {Float2, "TestVec2"}, {Float3, "TestVec3"}}),
                std::pair<std::vector<float>, VertexBufferLayout>({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                                                 {{Float, "TestFloat"}})
            ),
            ::testing::Values(GPUMemoryType::HOST_VISIBLE, GPUMemoryType::DEVICE_LOCAL)
        )
    );

}