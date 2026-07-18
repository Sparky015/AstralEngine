/**
* @file TLSFAllocatorTest.cpp
* @author Andrew Fagan
* @date 7/15/26
*/

#include <gtest/gtest.h>
#include <cstring>

#include "Core/Memory/Allocators/TLSFAllocator.h"
#include "Utilities/Loggers.h"

class TLSFAllocatorTest : public ::testing::TestWithParam<uint32>
{
public:
    static constexpr int DEFAULT_ALLOCATION_SIZE = 400'000;
    Astral::TLSFAllocator testAllocator = Astral::TLSFAllocator(DEFAULT_ALLOCATION_SIZE);

protected:

    void SetUp() override
    {
    }
};


TEST_F(TLSFAllocatorTest, Allocate_ReturnsUseableAddresses)
{
    char* allocatedAddress = (char*) testAllocator.Allocate(5); // allocates 5 chars
    std::strcpy(allocatedAddress, "abcd\0");
    EXPECT_STREQ(allocatedAddress, "abcd\0");

    char* allocatedAddress2 = (char*) testAllocator.Allocate(27);
    std::strcpy(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
    EXPECT_STREQ(allocatedAddress2, "abcdefghijklmnopqrstuvwxyz\0");
}

TEST_F(TLSFAllocatorTest, Fuzz)
{
    Astral::TLSFAllocator allocator = Astral::TLSFAllocator(1'000'000);

    void* pointers[10000];
    void* pointers2[10000];
    for (size_t j = 0; j < 500; j++)
    {
        for (uint32 i = 0; i < 100; i++)
        {
            int allocationSize  = std::rand() % 1024;
            pointers[i] = allocator.Allocate(allocationSize);
            if (pointers[i])
            {
                memset(pointers[i], 145, allocationSize);
            }
        }

        for (uint32 i = 0; i < 100; i++)
        {
            int allocationSize  = std::rand() % 1024;
            pointers2[i] = allocator.Allocate(allocationSize);
            if (pointers2[i])
            {
                memset(pointers2[i], 191, allocationSize);
            }
        }

        for (uint32 i = 0; i < 100; i++)
        {
            allocator.Free(pointers2[i]);
        }

        for (uint32 i = 0; i < 100; i++)
        {
            allocator.Free(pointers[i]);
        }
    }
}

void TestBoundaries(void* memory, size_t size)
{
    EXPECT_NO_FATAL_FAILURE(std::memset(memory, 21, size));
    // EXPECT_DEATH(std::memset(memory, 241, size + 1), "");
    // EXPECT_DEATH(std::memset((void*)((uintptr_t)memory - 16), 241, size + 16), "");
};

TEST_P(TLSFAllocatorTest, Allocate2_ReturnsUseableAddresses)
{
    uint32 size = GetParam();
    AE_LOG("Testing size: " << size)

    for (uint i = 0; i < 10; i++)
    {
        char* allocatedAddress = (char*)testAllocator.Allocate(size);
        if (!allocatedAddress) { continue;}

        TestBoundaries(allocatedAddress, size);

        testAllocator.Free(allocatedAddress);
    }

    for (uint i = 0; i < 10; i++)
    {
        char* allocatedAddress = (char*)testAllocator.Allocate(size);
        char* allocatedAddress2 = (char*)testAllocator.Allocate(size);
        if (!allocatedAddress || !allocatedAddress2) { continue;}

        TestBoundaries(allocatedAddress, size);
        TestBoundaries(allocatedAddress2, size);

        testAllocator.Free(allocatedAddress);
        testAllocator.Free(allocatedAddress2);
    }

    for (uint i = 0; i < 10; i++)
    {
        char* allocatedAddress = (char*)testAllocator.Allocate(size);
        char* allocatedAddress2 = (char*)testAllocator.Allocate(size);
        if (!allocatedAddress || !allocatedAddress2) { continue;}

        TestBoundaries(allocatedAddress, size);
        TestBoundaries(allocatedAddress2, size);

        testAllocator.Free(allocatedAddress);
        testAllocator.Free(allocatedAddress2);
    }

    for (uint i = 0; i < 10; i++)
    {
        char* allocatedAddress = (char*)testAllocator.Allocate(size);
        char* allocatedAddress2 = (char*)testAllocator.Allocate(size);
        if (!allocatedAddress || !allocatedAddress2) { continue;}

        TestBoundaries(allocatedAddress, size);
        TestBoundaries(allocatedAddress2, size);

        testAllocator.Free(allocatedAddress);
        testAllocator.Free(allocatedAddress2);
    }

    for (uint i = 0; i < 10; i++)
    {
        char* allocatedAddress = (char*)testAllocator.Allocate(size);
        char* allocatedAddress2 = (char*)testAllocator.Allocate(size);
        char* allocatedAddress3 = (char*)testAllocator.Allocate(size);
        char* allocatedAddress4 = (char*)testAllocator.Allocate(size);
        if (!allocatedAddress || !allocatedAddress2 || !allocatedAddress3 || !allocatedAddress4) { continue;}

        TestBoundaries(allocatedAddress, size);
        TestBoundaries(allocatedAddress2, size);
        TestBoundaries(allocatedAddress3, size);
        TestBoundaries(allocatedAddress4, size);

        testAllocator.Free(allocatedAddress);
        testAllocator.Free(allocatedAddress2);
        testAllocator.Free(allocatedAddress3);
        testAllocator.Free(allocatedAddress4);
    }

    for (uint i = 0; i < 500; i++)
    {
        char* allocatedAddress = (char*)testAllocator.Allocate(size);
        char* allocatedAddress2 = (char*)testAllocator.Allocate(size);
        testAllocator.Free(allocatedAddress2);
        char* allocatedAddress3 = (char*)testAllocator.Allocate(size);
        testAllocator.Free(allocatedAddress);
        testAllocator.Free(allocatedAddress3);
        char* allocatedAddress4 = (char*)testAllocator.Allocate(size);
        testAllocator.Free(allocatedAddress4);
    }

    for (uint i = 0; i < 10; i++)
    {
        char* allocatedAddress = (char*)testAllocator.Allocate(size);
        char* allocatedAddress2 = (char*)testAllocator.Allocate(size);
        char* allocatedAddress3 = (char*)testAllocator.Allocate(size);
        char* allocatedAddress4 = (char*)testAllocator.Allocate(size);
        testAllocator.Free(allocatedAddress);
        testAllocator.Free(allocatedAddress2);
        testAllocator.Free(allocatedAddress3);
        testAllocator.Free(allocatedAddress4);

        allocatedAddress3 = (char*)testAllocator.Allocate(size);
        allocatedAddress2 = (char*)testAllocator.Allocate(size);
        allocatedAddress = (char*)testAllocator.Allocate(size);
        allocatedAddress4 = (char*)testAllocator.Allocate(size);
        testAllocator.Free(allocatedAddress);
        testAllocator.Free(allocatedAddress2);
        testAllocator.Free(allocatedAddress3);
        testAllocator.Free(allocatedAddress4);
    }
}

INSTANTIATE_TEST_SUITE_P(
    TLSFAllocatorTestParams,
    TLSFAllocatorTest,
    ::testing::Values(4, 5, 1029, 19552, 29192, 1922, 19212, 22419, 9412, 15129, 1259, 1211, 145, 412, 1245, 1255,
                   1, 45, 12, 56, 122, 565, 1111, 4151, 6895, 1251, 29992, 11249, 9129, 9912, 2424, 50000, 1214
    )
);
