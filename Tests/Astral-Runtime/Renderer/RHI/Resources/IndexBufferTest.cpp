/**
* @file BufferTest.cpp
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

    };

    // NOTE: Index buffers are hardcoded to use device local memory, so I can't verify the memory contents without setting
    //       up staging buffers back to CPU readable memory just to view the memory contents. In the future, I will set up
    //       an optional parameter to declare which memory type you want to use.

    TEST_F(IndexBufferTests, Constructor_CorrectlyCreatesBufferWithDataPopulated)
    {
        std::array<uint32, 3> indices = {0, 1, 2};
        IndexBufferHandle indexBuffer = RendererAPI::GetDevice().CreateIndexBuffer(indices.data(), sizeof(indices));
        ASSERT_EQ(indexBuffer->GetCount(), 3);
    }

    TEST_F(IndexBufferTests, Constructor_2_CorrectlyCreatesBufferWithDataPopulated)
    {
        std::array<uint32, 7> indices = {0, 1, 2, 1, 2, 5, 2};
        IndexBufferHandle indexBuffer = RendererAPI::GetDevice().CreateIndexBuffer(indices.data(), sizeof(indices));
        ASSERT_EQ(indexBuffer->GetCount(), 7);
    }

    TEST_F(IndexBufferTests, GetCount_CorrectlyReturnsTheCountOfIndices)
    {
        std::array<uint32, 6> indices = {0, 1, 2, 1, 2, 3};
        IndexBufferHandle indexBuffer = RendererAPI::GetDevice().CreateIndexBuffer(indices.data(), sizeof(indices));
        ASSERT_EQ(indexBuffer->GetCount(), 6);
    }

    TEST_F(IndexBufferTests, GetCount_2_CorrectlyReturnsTheCountOfIndices)
    {
        std::array<uint32, 10> indices = {0, 1, 2, 1, 2, 3, 5, 4, 1, 0};
        IndexBufferHandle indexBuffer = RendererAPI::GetDevice().CreateIndexBuffer(indices.data(), sizeof(indices));
        ASSERT_EQ(indexBuffer->GetCount(), 10);
    }

}