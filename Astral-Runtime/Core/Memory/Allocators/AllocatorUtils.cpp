/**
* @file AllocatorUtils.cpp
* @author Andrew Fagan
* @date 2/3/25
*/

#include "AllocatorUtils.h"

namespace Astral::AllocatorUtils {

    bool ResizeMemoryBlock(void* memoryBlockPtr, size_t memoryBlockSize, void*& outNewMemoryBufferPointer, size_t& newMemoryBufferSize)
    {
        static constexpr float resizeMultiplier = 2.0f;
        newMemoryBufferSize = memoryBlockSize * resizeMultiplier;
        outNewMemoryBufferPointer = AllocMaxAlignedBlock(newMemoryBufferSize);
        if (!outNewMemoryBufferPointer) { return false; }
        FreeMaxAlignedBlock(memoryBlockPtr);

        WARN("Resizing Allocator Memory Block! (" << memoryBlockSize << " bytes -> " << newMemoryBufferSize << " bytes)");
        return true;
    }

}