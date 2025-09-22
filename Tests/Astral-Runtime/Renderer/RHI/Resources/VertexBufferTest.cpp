/**
* @file VertexBufferTest.cpp
* @author Andrew Fagan
* @date 9/21/25
*/

#include "gtest/gtest.h"

#include "Renderer/RHI/RendererAPI.h"
#include "Renderer/RHI/Resources/VertexBuffer.h"

namespace Astral {

    class VertexBufferTests : public ::testing::Test
    {
    public:

        void SetUp() override
        {
            RendererAPI::GetContext().ClearNumValidationErrorsAndWarnings();
        }

    };

    TEST_F(VertexBufferTests, Constructor_CorrectlyCreatesBufferWithDataPopulated)
    {
        {
            VertexBufferLayout bufferLayout = {{Float, "TestFloat"}};
            std::array<uint32, 3> vertexData = {0, 1, 2};
            VertexBufferHandle vertexBuffer = RendererAPI::GetDevice().CreateVertexBuffer(vertexData.data(), sizeof(vertexData), bufferLayout);
        }
        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }

    TEST_F(VertexBufferTests, Constructor_2_CorrectlyCreatesBufferWithDataPopulated)
    {
        {
            VertexBufferLayout bufferLayout = {{Float, "TestFloat"}, {Float2, "TestVec2"}};
            std::array<uint32, 12> vertexData = {0, 1, 2, 4, 5, 6, 3, 6, 1, 6, 8, 9};
            VertexBufferHandle vertexBuffer = RendererAPI::GetDevice().CreateVertexBuffer(vertexData.data(), sizeof(vertexData), bufferLayout);
        }
        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }

    TEST_F(VertexBufferTests, ConstructorUsingHostVisibleMemory_CorrectlyCreatesBufferWithDataPopulated)
    {
        {
            VertexBufferLayout bufferLayout = {{Float, "TestFloat"}, {Float2, "TestVec2"}};
            std::array<uint32, 12> vertexData = {0, 1, 2, 4, 5, 6, 3, 6, 1, 6, 8, 9};
            VertexBufferHandle vertexBuffer = RendererAPI::GetDevice().CreateVertexBuffer(vertexData.data(), sizeof(vertexData), bufferLayout, GPUMemoryType::HOST_VISIBLE);

            void* bufferVertexData;
            vertexBuffer->MapPointer(&bufferVertexData);
            ASSERT_EQ(memcmp(vertexData.data(), bufferVertexData, sizeof(vertexData)), 0);
            vertexBuffer->UnmapPointer();
        }

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }

    TEST_F(VertexBufferTests, ConstructorUsingHostVisibleMemory_2_CorrectlyCreatesBufferWithDataPopulated)
    {
        {
            VertexBufferLayout bufferLayout = {{Float3, "TestVec3"}, {Float2, "TestVec2"}};
            std::array<uint32, 15> vertexData = {0, 1, 2, 4, 5, 6, 3, 6, 1, 6, 8, 9, 1, 6, 7};
            VertexBufferHandle vertexBuffer = RendererAPI::GetDevice().CreateVertexBuffer(vertexData.data(), sizeof(vertexData), bufferLayout, GPUMemoryType::HOST_VISIBLE);

            void* bufferVertexData;
            vertexBuffer->MapPointer(&bufferVertexData);
            ASSERT_EQ(memcmp(vertexData.data(), bufferVertexData, sizeof(vertexData)), 0);
            vertexBuffer->UnmapPointer();
        }

        ASSERT_EQ(RendererAPI::GetContext().GetNumValidationErrorsAndWarnings(), 0);
    }

}