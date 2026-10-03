/**
* @file IndexBufferTest.cpp
* @author Andrew Fagan
* @date 9/21/25
*/

#include "gtest/gtest.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Resources/IndexBuffer.h"

namespace Astral {

    class IndexBufferTests : public ::testing::TestWithParam<std::tuple<std::vector<uint32>, GPUMemoryType>>
    {
    public:

        void SetUp() override
        {
            RendererAPI::GetContext().ClearNumValidationErrorsAndWarnings();
        }

    };

    TEST_P(IndexBufferTests, Constructor_CorrectlyCreatesBufferWithDataPopulated)
    {
        {
            auto& [indexData, gpuMemoryType] = GetParam();
            IndexBufferHandle indexBuffer = RendererAPI::GetDevice().CreateIndexBuffer((uint32*)indexData.data(), indexData.size() * sizeof(uint32), gpuMemoryType);

            ASSERT_NE(indexBuffer, nullptr);
            ASSERT_NE(indexBuffer->GetNativeHandle(), nullptr);
            ASSERT_EQ(indexBuffer->GetCount(), indexData.size());
        }

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    TEST_P(IndexBufferTests, Constructor_ConstructsBufferAndReadsBackDataCorrectly)
    {
        {
            auto& [indexData, gpuMemoryType] = GetParam();
            IndexBufferHandle indexBuffer = RendererAPI::GetDevice().CreateIndexBuffer((uint32*)indexData.data(), indexData.size() * sizeof(uint32), gpuMemoryType);

            ASSERT_NE(indexBuffer, nullptr);
            ASSERT_NE(indexBuffer->GetNativeHandle(), nullptr);
            ASSERT_EQ(indexBuffer->GetCount(), indexData.size());

            if (gpuMemoryType == GPUMemoryType::DEVICE_LOCAL)
            {
                indexBuffer->ChangeMemoryType(GPUMemoryType::HOST_VISIBLE);
            }

            void* bufferIndices;
            indexBuffer->MapPointer(&bufferIndices);
            ASSERT_EQ(memcmp(indexData.data(), bufferIndices, indexData.size() * sizeof(uint32)), 0);
            indexBuffer->UnmapPointer();
        }

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }


    INSTANTIATE_TEST_SUITE_P(
        IndexBufferTestsParams,
        IndexBufferTests,
        ::testing::Combine(
            ::testing::Values(
                std::vector<uint32>{0, 1, 2, 4, 5, 6, 3, 6, 1, 6, 8, 9, 1, 6, 7},
                std::vector<uint32>{1, 3, 4, 12, 3344, 15125, 12512, 111, 63212, 6666, 121, 0},
                std::vector<uint32>{1, 3, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 12, 34, 1, 46788, 12512, 1252, 12521, 25125, 12512},
                std::vector<uint32>{112, 4241, 123124, 566, 333, 33, 3, 1231, 568, 3785, 4125, 1352, 23576, 12451, 2225, 22,
                                    44212, 124332, 123123, 123, 12312, 25125, 563, 35135, 235231, 12435, 5356, 345235, 2352,
                                    435235, 12412, 12452, 6785, 87654, 124, 242, 779, 12, 52, 1, 5677, 3124, 1253, 6325, 43},
                std::vector<uint32>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
            ),
            ::testing::Values(GPUMemoryType::HOST_VISIBLE, GPUMemoryType::DEVICE_LOCAL)
        )
    );

}