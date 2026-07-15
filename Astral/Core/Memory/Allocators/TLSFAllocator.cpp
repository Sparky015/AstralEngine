/**
* @file TLSFAllocator.cpp
* @author Andrew Fagan
* @date 7/11/26
*/

#include "TLSFAllocator.h"

#include "Utilities/Asserts.h"

#include <algorithm>
#include <bit>

namespace Astral {

    TLSFAllocator::TLSFAllocator(size_t memoryPoolSize) :
        m_MemoryPoolSize(memoryPoolSize),
        m_FirstLevelLists(nullptr),
        m_NumFirstLevelIndex(0),
        m_NumSecondLevelIndex(0)
    {
        void* memoryPool = malloc(memoryPoolSize);
        InitializeTLSFStructure(memoryPool, memoryPoolSize);
    }


    TLSFAllocator::~TLSFAllocator()
    {

    }


    void* TLSFAllocator::Allocate(size_t size)
    {
        if (size == 0) { return nullptr; }

        BlockHeader* suitableMemoryBlock = (BlockHeader*)FindSuitableBlockFreeList(size);

        if (!suitableMemoryBlock)
        {
            return nullptr; // Out of memory
        }

        RemoveBlock(suitableMemoryBlock);

        if (suitableMemoryBlock->GetBlockSize() > size)
        {
            BlockHeader* remainingBlock = (BlockHeader*)SplitMemoryBlock(suitableMemoryBlock, size);

            uint32 firstLevelIndex{};
            uint32 secondLevelIndex{};
            Mapping(remainingBlock->GetBlockSize(), &firstLevelIndex, &secondLevelIndex);

            if (remainingBlock)
            {
                InsertBlock(remainingBlock, firstLevelIndex, secondLevelIndex);
            }
        }

        return suitableMemoryBlock;
    }


    void TLSFAllocator::Free(void* memoryBlock)
    {
        if (!memoryBlock) { return; }

        void* coalescedBlock = CoalesceMemoryBlocks(memoryBlock);

        BlockHeader* usedBlockHeader = (BlockHeader*)memoryBlock;
        size_t memoryBlockSize = usedBlockHeader->GetBlockSize();
        uint32 blockFirstLevelIndex = 0;
        uint32 blockSecondLevelIndex = 0;
        Mapping(memoryBlockSize, &blockFirstLevelIndex, &blockSecondLevelIndex);

        InsertBlock(coalescedBlock, blockFirstLevelIndex, blockSecondLevelIndex);
    }


    void TLSFAllocator::Reset()
    {
        InitializeTLSFStructure(m_FirstLevelLists, m_MemoryPoolSize);
    }


    size_t TLSFAllocator::GetUsedBlockSize() const
    {
    }


    size_t TLSFAllocator::GetCapacity() const
    {
        return GetOwnedMemorySize() - GetTLSFStructureSize();
    }


    size_t TLSFAllocator::GetOwnedMemorySize() const
    {
        return m_MemoryPoolSize;
    }


    AllocatorType TLSFAllocator::GetAllocatorType() const
    {
        return AllocatorType::TLSF;
    }


    // ======= Block Header ===================================

    size_t TLSFAllocator::BlockHeader::GetBlockSize()
    {
        return this->SizeAndFlags & ~(0b11);
    }


    void TLSFAllocator::BlockHeader::SetBlockSize(size_t blockSize)
    {
        size_t blockSizeMask = (0b11);
        size_t clearedSize = this->SizeAndFlags & blockSizeMask;
        this->SizeAndFlags = clearedSize | blockSize;
    }


    void TLSFAllocator::BlockHeader::SetIsBlockFree(bool isBlockFree)
    {
        size_t isBlockFreeMask = ~(0b01);
        size_t sizeWithClearedIsBlockFree = this->SizeAndFlags & isBlockFreeMask;
        this->SizeAndFlags = sizeWithClearedIsBlockFree | isBlockFree;
    }


    void TLSFAllocator::BlockHeader::SetIsLastPoolPhysicalBlock(bool isLastPoolPhysicalBlock)
    {
        size_t isLastPoolPhysicalBlockMask = ~(0b10);
        size_t sizeWithClearedIsLastPoolPhysicalBlock = this->SizeAndFlags & isLastPoolPhysicalBlockMask;
        this->SizeAndFlags = sizeWithClearedIsLastPoolPhysicalBlock | isLastPoolPhysicalBlock;
    }


    bool TLSFAllocator::BlockHeader::IsBlockFree()
    {
        return (this->SizeAndFlags & 0b01);
    }


    bool TLSFAllocator::BlockHeader::IsLastPoolPhysicalBlock()
    {
        return (this->SizeAndFlags & 0b10);
    }


    void* TLSFAllocator::BlockHeader::GetPreviousBlock()
    {
        return this->PreviousBlock;
    }


    void TLSFAllocator::BlockHeader::SetPreviousBlock(void* previousBlock)
    {
        this->PreviousBlock = previousBlock;
    }


    void* TLSFAllocator::BlockHeader::GetNextFreeBlock()
    {
        return this->NextFreeBlock;
    }


    void TLSFAllocator::BlockHeader::SetNextFreeBlock(void* nextFreeBlock)
    {
        this->NextFreeBlock = nextFreeBlock;
    }


    void* TLSFAllocator::BlockHeader::GetPreviousFreeBlock()
    {
        return this->PreviousFreeBlock;
    }


    void TLSFAllocator::BlockHeader::SetPreviousFreeBlock(void* previousFreeBlock)
    {
        this->PreviousFreeBlock = previousFreeBlock;
    }

    // ===============================================================


    void TLSFAllocator::InitializeTLSFStructure(void* memoryPool, size_t memoryPoolSize)
    {
        // Initialize TLSF structure values

        ASSERT(memoryPool != nullptr, "Given memory pool pointer is null!")
        m_FirstLevelLists = (uintptr_t**)memoryPool;
        m_NumFirstLevelIndex = std::min((uint32)std::log2(memoryPoolSize), 31u);
        m_NumSecondLevelIndex = 5; // Recommended value by TLSF research paper

        memset(memoryPool, 0, memoryPoolSize);


        // Initialize first memory block

        BlockHeader* firstMemoryBlock = (BlockHeader*)&m_FirstLevelLists[m_NumFirstLevelIndex * m_NumSecondLevelIndex + 1];
        firstMemoryBlock->SetBlockSize(memoryPoolSize - (m_NumFirstLevelIndex * m_NumSecondLevelIndex * sizeof(void*)));
        firstMemoryBlock->SetIsLastPoolPhysicalBlock(true);
        firstMemoryBlock->SetIsBlockFree(true);
        firstMemoryBlock->SetNextFreeBlock(nullptr);
        firstMemoryBlock->SetPreviousFreeBlock(nullptr);

        uint32 blockFirstLevelIndex = 0;
        uint32 blockSecondLevelIndex = 0;
        Mapping(firstMemoryBlock->GetBlockSize(), &blockFirstLevelIndex, &blockSecondLevelIndex);
        InsertBlock(firstMemoryBlock, blockFirstLevelIndex, blockSecondLevelIndex);
    }


    void TLSFAllocator::Mapping(size_t size, uint32* outFirstLevel, uint32* outSecondLevel)
    {
        *outFirstLevel = std::bit_width(size) - 1; // Finds the last bit set
        *outSecondLevel = ((size ^ (1 << *outFirstLevel)) >> (*outFirstLevel - *outSecondLevel));
    }


    void* TLSFAllocator::GetMemoryBlockFreeList(uint32 blockFirstLevelIndex, uint32 blockSecondLevelIndex)
    {
        return m_FirstLevelLists[blockFirstLevelIndex * m_NumSecondLevelIndex + blockSecondLevelIndex];
    }


    void TLSFAllocator::SetMemoryBlockFreeListHead(void* memoryBlockHead, uint32 blockFirstLevelIndex, uint32 blockSecondLevelIndex)
    {
        m_FirstLevelLists[blockFirstLevelIndex * m_NumSecondLevelIndex + blockSecondLevelIndex] = (uintptr_t*)memoryBlockHead;
    }


    void* TLSFAllocator::FindSuitableBlockFreeList(size_t memoryBlockSize)
    {
        uint32 blockFirstLevelIndex = 0;
        uint32 blockSecondLevelIndex = 0;
        Mapping(memoryBlockSize, &blockFirstLevelIndex, &blockSecondLevelIndex);
        void* memoryBlockFreeList = GetMemoryBlockFreeList(blockFirstLevelIndex, blockSecondLevelIndex);

        while (!memoryBlockFreeList)
        {
            blockSecondLevelIndex++;
            if (blockSecondLevelIndex == m_NumSecondLevelIndex)
            {
                blockSecondLevelIndex = 0;
                blockFirstLevelIndex++;
            }

            if (blockFirstLevelIndex == m_NumFirstLevelIndex && blockSecondLevelIndex == m_NumSecondLevelIndex)
            {
                return nullptr; // No available free list
            }

            memoryBlockFreeList = GetMemoryBlockFreeList(blockFirstLevelIndex, blockSecondLevelIndex);
        }

        return memoryBlockFreeList;
    }


    void TLSFAllocator::InsertBlock(void* memoryBlock, uint32 firstLevelIndex, uint32 secondLevelIndex)
    {
        if (memoryBlock == nullptr) { return; }

        // Update doubly linked free list links between current head and new memory block
        BlockHeader* currentFreeListHeader = (BlockHeader*)GetMemoryBlockFreeList(firstLevelIndex, secondLevelIndex);

        if (currentFreeListHeader)
        {
            currentFreeListHeader->SetPreviousFreeBlock(memoryBlock);

            BlockHeader* usedBlockHeader = (BlockHeader*)memoryBlock;
            usedBlockHeader->SetNextFreeBlock(currentFreeListHeader);
            usedBlockHeader->SetPreviousFreeBlock(nullptr);
        }

        // Set memoryBlock as new head
        SetMemoryBlockFreeListHead(memoryBlock, firstLevelIndex, secondLevelIndex);
    }


    void* TLSFAllocator::CoalesceMemoryBlocks(void* memoryBlock)
    {
        if (memoryBlock == nullptr) { return nullptr; }

        BlockHeader* blockHeader = (BlockHeader*)memoryBlock;
        BlockHeader* previousPhysicalBlockHeader = (BlockHeader*)blockHeader->GetPreviousBlock();

        while (previousPhysicalBlockHeader && previousPhysicalBlockHeader->IsBlockFree())
        {
            previousPhysicalBlockHeader->SetNextFreeBlock(blockHeader->GetNextFreeBlock());
            previousPhysicalBlockHeader->SetIsLastPoolPhysicalBlock(blockHeader->IsLastPoolPhysicalBlock());

            blockHeader = previousPhysicalBlockHeader;
            previousPhysicalBlockHeader = (BlockHeader*)blockHeader->GetPreviousBlock();
        }

        return blockHeader;
    }


    void TLSFAllocator::RemoveBlock(void* memoryBlockFreeList)
    {
        if (memoryBlockFreeList == nullptr) { return; }

        // Set next free block as head of the segregated free list
        BlockHeader* freeBlockHeader = (BlockHeader*)memoryBlockFreeList;
        BlockHeader* nextFreeBlockHeader = (BlockHeader*)freeBlockHeader->GetNextFreeBlock();

        uint32 blockSize = freeBlockHeader->GetBlockSize();
        uint32 firstLevelIndex{};
        uint32 secondLevelIndex{};
        Mapping(blockSize, &firstLevelIndex, &secondLevelIndex);
        SetMemoryBlockFreeListHead(nextFreeBlockHeader, firstLevelIndex, secondLevelIndex);


        // Clear removed memory block's flags and update next free block's flags
        freeBlockHeader->SetPreviousFreeBlock(nullptr);
        freeBlockHeader->SetNextFreeBlock(nullptr);
        freeBlockHeader->SetIsBlockFree(false);
        freeBlockHeader->SetIsLastPoolPhysicalBlock(false);

        if (nextFreeBlockHeader)
        {
            nextFreeBlockHeader->SetPreviousFreeBlock(nullptr);
        }
    }


    void* TLSFAllocator::SplitMemoryBlock(void* memoryBlock, size_t reducedBlockSize)
    {
        if (memoryBlock == nullptr) { return nullptr; }

        BlockHeader* freeBlockHeader = (BlockHeader*)memoryBlock;

        if (freeBlockHeader->GetBlockSize() - reducedBlockSize <= sizeof(BlockHeader) + m_MinimumBlockSize)
        {
            // Block header alone would not leave enough space to fill minimum block size, so a split is not possible
            return nullptr;
        }

        // Initialize remaining block
        BlockHeader* remainingBlock = (BlockHeader*)((uintptr_t)freeBlockHeader + reducedBlockSize);
        memset(remainingBlock, 0, sizeof(BlockHeader));

        remainingBlock->SetBlockSize(freeBlockHeader->GetBlockSize() - reducedBlockSize - m_UsedBlockHeaderSize);
        remainingBlock->SetPreviousBlock(freeBlockHeader);
        remainingBlock->SetIsBlockFree(true); // Needs to be free to modify a block
        remainingBlock->SetNextFreeBlock(nullptr); // Outside scope of function
        remainingBlock->SetPreviousFreeBlock(nullptr);

        remainingBlock->SetIsLastPoolPhysicalBlock(freeBlockHeader->IsLastPoolPhysicalBlock());
        freeBlockHeader->SetIsLastPoolPhysicalBlock(false);

        return remainingBlock;
    }


    size_t TLSFAllocator::GetTLSFStructureSize() const
    {
        return m_NumSecondLevelIndex * sizeof(void*) - (m_NumFirstLevelIndex - m_MinimumBlockSize);
    }

}
