/**
* @file IndexBufferTest.cpp
* @author Andrew Fagan
* @date 9/21/25
*/

#include "gtest/gtest.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Resources/Buffer.h"

namespace Astral {

    class IndexBufferTests : public ::testing::Test
    {
    public:

        void SetUp() override
        {
            RendererAPI::GetContext().ClearNumValidationErrorsAndWarnings();
        }

    };

    TEST_F(IndexBufferTests, Constructor_CorrectlyCreatesBufferWithDataPopulated)
    {
        {
            std::array<uint32, 3> indices = {0, 1, 2};
            IndexBufferHandle indexBuffer = RendererAPI::GetDevice().CreateIndexBuffer(indices.data(), sizeof(indices));
            ASSERT_EQ(indexBuffer->GetCount(), 3);
        }
        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }

    TEST_F(IndexBufferTests, Constructor_2_CorrectlyCreatesBufferWithDataPopulated)
    {
        {
            std::array<uint32, 7> indices = {0, 1, 2, 1, 2, 5, 2};
            IndexBufferHandle indexBuffer = RendererAPI::GetDevice().CreateIndexBuffer(indices.data(), sizeof(indices));
            ASSERT_EQ(indexBuffer->GetCount(), 7);
        }
        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }

    TEST_F(IndexBufferTests, ConstructorUsingHostVisibleMemory_CorrectlyCreatesBufferWithDataPopulated)
    {
        {
            std::array<uint32, 4> indices = {0, 1, 2, 1};
            IndexBufferHandle indexBuffer = RendererAPI::GetDevice().CreateIndexBuffer(indices.data(), sizeof(indices), GPUMemoryType::HOST_VISIBLE);
            ASSERT_EQ(indexBuffer->GetCount(), 4);

            void* bufferIndices;
            indexBuffer->MapPointer(&bufferIndices);
            ASSERT_EQ(memcmp(indices.data(), bufferIndices, sizeof(indices)), 0);
            indexBuffer->UnmapPointer();
        }

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }

    TEST_F(IndexBufferTests, ConstructorUsingHostVisibleMemory_2_CorrectlyCreatesBufferWithDataPopulated)
    {
        {
            std::array<uint32, 6> indices = {0, 1, 2, 1, 8, 4};
            IndexBufferHandle indexBuffer = RendererAPI::GetDevice().CreateIndexBuffer(indices.data(), sizeof(indices), GPUMemoryType::HOST_VISIBLE);
            ASSERT_EQ(indexBuffer->GetCount(), 6);

            void* bufferIndices;
            indexBuffer->MapPointer(&bufferIndices);
            ASSERT_EQ(memcmp(indices.data(), bufferIndices, sizeof(indices)), 0);
            indexBuffer->UnmapPointer();
        }

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }

    TEST_F(IndexBufferTests, GetCount_CorrectlyReturnsTheCountOfIndices)
    {
        {
            std::array<uint32, 6> indices = {0, 1, 2, 1, 2, 3};
            IndexBufferHandle indexBuffer = RendererAPI::GetDevice().CreateIndexBuffer(indices.data(), sizeof(indices));
            ASSERT_EQ(indexBuffer->GetCount(), 6);
        }
        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }

    TEST_F(IndexBufferTests, GetCount_2_CorrectlyReturnsTheCountOfIndices)
    {
        {
            std::array<uint32, 10> indices = {0, 1, 2, 1, 2, 3, 5, 4, 1, 0};
            IndexBufferHandle indexBuffer = RendererAPI::GetDevice().CreateIndexBuffer(indices.data(), sizeof(indices));
            ASSERT_EQ(indexBuffer->GetCount(), 10);
        }
        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }



}