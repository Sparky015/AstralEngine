/**
* @file TLSFAllocator.cpp
* @author Andrew Fagan
* @date 7/11/26
*/

#include "TLSFAllocator.h"

#include "Utilities/Asserts.h"
#include "AllocatorUtils.h"

#include <algorithm>
#include <bit>
#include <unordered_set>

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
        // if (size == 0)
        // {
        //     return nullptr;
        // }
        if (size < m_MinimumBlockSize) { size = m_MinimumBlockSize; }
        if (size + m_UsedBlockHeaderSize > GetCapacity())
        {
            return nullptr;
        }

        AssertMemoryUsage(); // DEBUG
        ValidateBlockHeaders(); // DEBUG

        BlockHeader* suitableMemoryBlock = (BlockHeader*)FindSuitableBlockFreeList(size + m_UsedBlockHeaderSize);

        if (!suitableMemoryBlock)
        {
            // AE_ERROR("Memory ran out!")
            return nullptr; // Out of memory
        }

        RemoveBlock(suitableMemoryBlock);

        if (suitableMemoryBlock->GetBlockSize() > size + m_UsedBlockHeaderSize)
        {
            BlockHeader* remainingBlock = (BlockHeader*)SplitMemoryBlock(suitableMemoryBlock, size + m_UsedBlockHeaderSize);

            if (remainingBlock)
            {
                uint32 firstLevelIndex{};
                uint32 secondLevelIndex{};
                size_t remainingBlockSize = remainingBlock->GetBlockSize();
                Mapping(remainingBlockSize, &firstLevelIndex, &secondLevelIndex);

                InsertBlock(remainingBlock, firstLevelIndex, secondLevelIndex);
            }
        }

        size_t selectedBlockMemorySize = suitableMemoryBlock->GetBlockSize();
        suitableMemoryBlock->SetIsBlockFree(false);

        m_SizeOfCurrentlyAllocatedBlocks += suitableMemoryBlock->GetBlockSize();
        m_NumBlocksCurrentlyAllocated++;


        void* userMemory = (void*)((uintptr_t)suitableMemoryBlock + m_UsedBlockHeaderSize);

        ASSERT(suitableMemoryBlock->GetBlockSize() >= size, "Selected block is not greater than or equal to the requested size!")
        ASSERT(((uintptr_t)userMemory & (uintptr_t)(0b1111)) == 0, "not 16 byte aligned")


        AssertMemoryUsage(); // DEBUG
        ValidateBlockHeaders(); // DEBUG

        return userMemory;
    }


    void TLSFAllocator::Free(void* memoryBlock)
    {
        if (!memoryBlock) { return; }

        AssertMemoryUsage(); // DEBUG
        ValidateBlockHeaders(); // DEBUG

        BlockHeader* usedBlockHeader = (BlockHeader*)((uintptr_t)memoryBlock - m_UsedBlockHeaderSize);
        usedBlockHeader->SetNextFreeBlock(nullptr);
        usedBlockHeader->SetPreviousFreeBlock(nullptr);


        m_NumBlocksCurrentlyAllocated--;
        m_SizeOfCurrentlyAllocatedBlocks -= usedBlockHeader->GetBlockSize();

        ASSERT(usedBlockHeader->GetBlockSize() < GetCapacity(), "Overwrite?")


        void* coalescedBlock = CoalesceMemoryBlocks((void*)((uintptr_t)memoryBlock - m_UsedBlockHeaderSize));

        usedBlockHeader = (BlockHeader*)coalescedBlock;

        size_t memoryBlockSize = usedBlockHeader->GetBlockSize();
        uint32 blockFirstLevelIndex = 0;
        uint32 blockSecondLevelIndex = 0;
        Mapping(memoryBlockSize, &blockFirstLevelIndex, &blockSecondLevelIndex);

        InsertBlock(coalescedBlock, blockFirstLevelIndex, blockSecondLevelIndex);

        AssertMemoryUsage(); // DEBUG
        ValidateBlockHeaders(); // DEBUG
    }


    bool TLSFAllocator::DoesOwnPointer(void* pointer)
    {
        return pointer >= m_FirstLevelLists && pointer <= (void*)((uintptr_t)m_FirstLevelLists + GetOwnedMemorySize());
    }


    void TLSFAllocator::Reset()
    {
        InitializeTLSFStructure(m_FirstLevelLists, m_MemoryPoolSize);
    }


    size_t TLSFAllocator::GetUsedBlockSize() const
    {
        return m_SizeOfCurrentlyAllocatedBlocks + m_NumBlocksCurrentlyAllocated * m_UsedBlockHeaderSize;
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

    size_t TLSFAllocator::BlockHeader::GetBlockSize() const
    {
        return this->SizeAndFlags & ~(0b11);
    }


    void TLSFAllocator::BlockHeader::SetBlockSize(size_t blockSize)
    {
        ASSERT(!(blockSize & 0b11), "Block size should never have first two bits set!")

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
        this->SizeAndFlags = sizeWithClearedIsLastPoolPhysicalBlock | (isLastPoolPhysicalBlock << 1);
    }


    bool TLSFAllocator::BlockHeader::IsBlockFree() const
    {
        return (this->SizeAndFlags & 0b01);
    }


    bool TLSFAllocator::BlockHeader::IsLastPoolPhysicalBlock() const
    {
        return this->SizeAndFlags & 0b10;
    }


    void* TLSFAllocator::BlockHeader::GetPreviousBlock() const
    {
        return this->PreviousBlock;
    }


    void TLSFAllocator::BlockHeader::SetPreviousBlock(void* previousBlock)
    {
        this->PreviousBlock = previousBlock;
    }


    void* TLSFAllocator::BlockHeader::GetNextFreeBlock() const
    {
        return this->NextFreeBlock;
    }


    void TLSFAllocator::BlockHeader::SetNextFreeBlock(void* nextFreeBlock)
    {
        this->NextFreeBlock = nextFreeBlock;
    }


    void* TLSFAllocator::BlockHeader::GetPreviousFreeBlock() const
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

        BlockHeader* firstMemoryBlock = (BlockHeader*)&m_FirstLevelLists[(uint32)((m_NumFirstLevelIndex + 1) * std::pow(2, m_NumSecondLevelIndex))];

        ASSERT((uintptr_t)firstMemoryBlock == (uintptr_t)m_FirstLevelLists + GetTLSFStructureSize(), "GetTLSFStructureSize does not accurately represent structure size!");

        m_DebugInitialAlignmentBytes = 0;
        if ((uintptr_t)firstMemoryBlock % 16 != 0)
        {
            // Round up to next 16 byte boundary
            m_DebugInitialAlignmentBytes = 16 - ((uintptr_t)firstMemoryBlock % 16); // bytes needed to reach 16 byte boundary
            firstMemoryBlock = (BlockHeader*)((uintptr_t)firstMemoryBlock + m_DebugInitialAlignmentBytes);
        }

        firstMemoryBlock->SetBlockSize(memoryPoolSize - GetTLSFStructureSize() - m_UsedBlockHeaderSize - m_DebugInitialAlignmentBytes);
        firstMemoryBlock->SetIsLastPoolPhysicalBlock(true);
        firstMemoryBlock->SetIsBlockFree(false);
        firstMemoryBlock->SetNextFreeBlock(nullptr);
        firstMemoryBlock->SetPreviousFreeBlock(nullptr);
        firstMemoryBlock->SetPreviousBlock(nullptr);

        m_DebugBlocks[firstMemoryBlock] = firstMemoryBlock->GetBlockSize(); // DEBUG

        uint32 blockFirstLevelIndex = 0;
        uint32 blockSecondLevelIndex = 0;
        Mapping(firstMemoryBlock->GetBlockSize(), &blockFirstLevelIndex, &blockSecondLevelIndex);
        InsertBlock(firstMemoryBlock, blockFirstLevelIndex, blockSecondLevelIndex);

    }


    void TLSFAllocator::Mapping(size_t size, uint32* outFirstLevel, uint32* outSecondLevel)
    {
        ASSERT(size <= GetCapacity(), "Not possible to map to size bigger than allocator pool")
        *outFirstLevel = std::log2(size);
        *outSecondLevel = (size - std::pow(2, *outFirstLevel)) * (std::pow(2, m_NumSecondLevelIndex) / std::pow(2, *outFirstLevel));
    }


    void* TLSFAllocator::GetMemoryBlockFreeList(uint32 blockFirstLevelIndex, uint32 blockSecondLevelIndex)
    {
        void* freeList = m_FirstLevelLists[(uint32)(blockFirstLevelIndex * std::pow(2, m_NumSecondLevelIndex) + blockSecondLevelIndex)];
        if (freeList)
        {
            BlockHeader* freeListBlockHeader = (BlockHeader*)freeList;
            ASSERT(freeListBlockHeader->GetBlockSize() < GetCapacity(), "Overwrite?")
        }
        return freeList;
    }


    void TLSFAllocator::SetMemoryBlockFreeListHead(void* memoryBlockHead, uint32 blockFirstLevelIndex, uint32 blockSecondLevelIndex)
    {
        ASSERT(blockSecondLevelIndex <= std::pow(2, m_NumSecondLevelIndex), "Second level index mapping is invalid!")
        ASSERT(blockFirstLevelIndex <= m_NumFirstLevelIndex, "First level index mapping is invalid!")

        m_FirstLevelLists[(uint32)(blockFirstLevelIndex * std::pow(2, m_NumSecondLevelIndex) + blockSecondLevelIndex)] = (uintptr_t*)memoryBlockHead;
    }


    void TLSFAllocator::SetMemoryBlockFreeListHead(void* memoryBlockHead, uint32 size)
    {
        uint32 firstLevelIndex = 0;
        uint32 secondLevelIndex = 0;
        Mapping(size, &firstLevelIndex, &secondLevelIndex);
        SetMemoryBlockFreeListHead(memoryBlockHead, firstLevelIndex, secondLevelIndex);
    }


    void* TLSFAllocator::FindSuitableBlockFreeList(size_t memoryBlockSize)
    {
        uint32 blockFirstLevelIndex = 0;
        uint32 blockSecondLevelIndex = 0;
        Mapping(memoryBlockSize, &blockFirstLevelIndex, &blockSecondLevelIndex);
        blockSecondLevelIndex++;
        void* memoryBlockFreeList = GetMemoryBlockFreeList(blockFirstLevelIndex, blockSecondLevelIndex);

        while (!memoryBlockFreeList)
        {
            blockSecondLevelIndex++;
            if (blockSecondLevelIndex >= std::pow(2, m_NumSecondLevelIndex))
            {
                blockSecondLevelIndex = 0;
                blockFirstLevelIndex++;
            }

            if (blockFirstLevelIndex >= m_NumFirstLevelIndex + 1)
            {
                return nullptr; // No available free list
            }

            memoryBlockFreeList = GetMemoryBlockFreeList(blockFirstLevelIndex, blockSecondLevelIndex);
        }
        ASSERT(blockSecondLevelIndex <= std::pow(2, m_NumSecondLevelIndex), "Second level index mapping is invalid!")
        ASSERT(blockFirstLevelIndex <= m_NumFirstLevelIndex, "First level index mapping is invalid!")

        return memoryBlockFreeList;
    }


    void TLSFAllocator::InsertBlock(void* memoryBlock, uint32 firstLevelIndex, uint32 secondLevelIndex)
    {
        if (memoryBlock == nullptr) { return; }

        // Update doubly linked free list links between current head and new memory block
        BlockHeader* currentFreeListHeader = (BlockHeader*)GetMemoryBlockFreeList(firstLevelIndex, secondLevelIndex);

        BlockHeader* memoryBlockHeader = (BlockHeader*)memoryBlock;
        size_t memoryBlockSize = memoryBlockHeader->GetBlockSize();
        memoryBlockHeader->SetNextFreeBlock(currentFreeListHeader);
        memoryBlockHeader->SetPreviousFreeBlock(nullptr);
        memoryBlockHeader->SetIsBlockFree(true);
        m_DebugBlocks[memoryBlockHeader] = memoryBlockSize; // DEBUG

        if (currentFreeListHeader)
        {
            currentFreeListHeader->SetPreviousFreeBlock(memoryBlock);
        }

        // Set memoryBlock as new head
        SetMemoryBlockFreeListHead(memoryBlock, firstLevelIndex, secondLevelIndex);
    }


    void* TLSFAllocator::CoalesceMemoryBlocks(void* memoryBlock)
    {
        if (memoryBlock == nullptr) { return nullptr; }

        BlockHeader* blockHeader = (BlockHeader*)memoryBlock;
        BlockHeader* previousPhysicalBlockHeader = (BlockHeader*)blockHeader->GetPreviousBlock();

        size_t coalescedSize = 0;
        while (previousPhysicalBlockHeader && previousPhysicalBlockHeader->IsBlockFree())
        {
            RemoveBlock(memoryBlock);
            RemoveBlock(previousPhysicalBlockHeader);
            m_DebugBlocks.erase(blockHeader); // DEBUG

            previousPhysicalBlockHeader->SetIsLastPoolPhysicalBlock(blockHeader->IsLastPoolPhysicalBlock());
            coalescedSize += blockHeader->GetBlockSize() + m_UsedBlockHeaderSize;

            if (!blockHeader->IsLastPoolPhysicalBlock())
            {
                BlockHeader* nextPhysicalBlock = (BlockHeader*)((uintptr_t)blockHeader + coalescedSize);

                nextPhysicalBlock->SetPreviousBlock(previousPhysicalBlockHeader);
            }

            blockHeader = previousPhysicalBlockHeader;
            previousPhysicalBlockHeader = (BlockHeader*)blockHeader->GetPreviousBlock();
        }
        coalescedSize += blockHeader->GetBlockSize();
        blockHeader->SetBlockSize(coalescedSize);
        ASSERT(coalescedSize < GetCapacity(), "Not possible to map to size bigger than allocator pool")

        return blockHeader;
    }


    void TLSFAllocator::RemoveBlock(void* memoryBlockFreeList)
    {
        if (memoryBlockFreeList == nullptr) { return; }

        // Set next free block as head of the segregated free list
        BlockHeader* freeBlockHeader = (BlockHeader*)memoryBlockFreeList;

        BlockHeader* nextFreeBlockHeader = (BlockHeader*)freeBlockHeader->GetNextFreeBlock();

        if (freeBlockHeader->GetPreviousFreeBlock() == nullptr) // if previous physical block was head of a free list
        {
            // Remove from free list head
            SetMemoryBlockFreeListHead(nextFreeBlockHeader, freeBlockHeader->GetBlockSize());
        }
        else
        {
            BlockHeader* previousFreeBlock = (BlockHeader*)freeBlockHeader->GetPreviousFreeBlock();
            previousFreeBlock->SetNextFreeBlock(nextFreeBlockHeader);
        }

        if (nextFreeBlockHeader)
        {
            nextFreeBlockHeader->SetPreviousFreeBlock(freeBlockHeader->GetPreviousFreeBlock());
        }


        // Clear removed memory block's flags and update next free block's flags
        freeBlockHeader->SetPreviousFreeBlock(nullptr);
        freeBlockHeader->SetNextFreeBlock(nullptr);
        freeBlockHeader->SetIsBlockFree(false);
    }


    void* TLSFAllocator::SplitMemoryBlock(void* memoryBlock, size_t reducedBlockSize)
    {
        if (memoryBlock == nullptr) { return nullptr; }

        BlockHeader* freeBlockHeader = (BlockHeader*)memoryBlock;

        if (reducedBlockSize % 16 != 0)
        {
            // Round up to next 16 byte boundary
            uint32 bytesNeeded = 16 - (reducedBlockSize % 16); // bytes needed to reach 16 byte boundary
            reducedBlockSize += bytesNeeded;
        }

        if ((int64)freeBlockHeader->GetBlockSize() - (int64)reducedBlockSize <= (int64)(sizeof(BlockHeader) + m_MinimumBlockSize))
        {
            // Block header alone would not leave enough space to fill minimum block size, so a split is not possible
            return nullptr;
        }

        // Initialize remaining block

        BlockHeader* remainingBlock = (BlockHeader*)((uintptr_t)freeBlockHeader + reducedBlockSize);

        remainingBlock->SetBlockSize(freeBlockHeader->GetBlockSize() - reducedBlockSize);
        remainingBlock->SetPreviousBlock(freeBlockHeader);
        remainingBlock->SetIsBlockFree(false);
        remainingBlock->SetNextFreeBlock(nullptr); // Outside scope of function
        remainingBlock->SetPreviousFreeBlock(nullptr);
        remainingBlock->SetIsLastPoolPhysicalBlock(freeBlockHeader->IsLastPoolPhysicalBlock());
        m_DebugBlocks[remainingBlock] = remainingBlock->GetBlockSize(); // DEBUG

        if (!remainingBlock->IsLastPoolPhysicalBlock())
        {
            BlockHeader* nextPhysicalBlock = (BlockHeader*)((uintptr_t)remainingBlock + remainingBlock->GetBlockSize() + m_UsedBlockHeaderSize);

            nextPhysicalBlock->SetPreviousBlock(remainingBlock);
        }


        freeBlockHeader->SetIsLastPoolPhysicalBlock(false);
        freeBlockHeader->SetBlockSize(reducedBlockSize - m_UsedBlockHeaderSize);
        freeBlockHeader->SetNextFreeBlock(nullptr); // Outside scope of function
        freeBlockHeader->SetPreviousFreeBlock(nullptr);
        m_DebugBlocks[freeBlockHeader] = freeBlockHeader->GetBlockSize(); // DEBUG

        return remainingBlock;
    }


    size_t TLSFAllocator::GetTLSFStructureSize() const
    {
        return std::pow(2, m_NumSecondLevelIndex) * sizeof(void*) * (m_NumFirstLevelIndex + 1);
    }


    void TLSFAllocator::AssertMemoryUsage()
    {// DEBUG
        return;
        size_t totalDeclaredUsage = m_DebugInitialAlignmentBytes;

        for (auto [pointer, blockSize] : m_DebugBlocks)
        {
            totalDeclaredUsage += blockSize + m_UsedBlockHeaderSize;
        }

        ASSERT(GetCapacity() + GetTLSFStructureSize() == GetOwnedMemorySize(), "Error with capacity or tlsf structure size calc!");
        ASSERT(GetCapacity() == totalDeclaredUsage, "Tracked memory size does not match owned memory size! (Capacity: " << GetCapacity() << " vs Declared: " << totalDeclaredUsage << ")");
    }


    void TLSFAllocator::ValidateBlockHeaders()
    {
        return;
        std::unordered_set<BlockHeader*, std::hash<const BlockHeader*>, std::equal_to<const BlockHeader*>, NoTrackingSTLAllocator<BlockHeader*>> physicalBlockHeaders = {};

        // Collect all physical block addresses

        BlockHeader* current = (BlockHeader*)((uintptr_t)m_FirstLevelLists + GetTLSFStructureSize());
        physicalBlockHeaders.insert(current);

        while (current)
        {
            if (!current->IsLastPoolPhysicalBlock())
            {
                BlockHeader* nextPhysicalBlock = (BlockHeader*)((uintptr_t)current + current->GetBlockSize() + m_UsedBlockHeaderSize);

                physicalBlockHeaders.insert(nextPhysicalBlock);
                current = nextPhysicalBlock;

            }
            else
            {
                current = nullptr;
                break;
            }

        }

        // ====


        // Check for dangling physical blocks
        current = (BlockHeader*)((uintptr_t)m_FirstLevelLists + GetTLSFStructureSize());

        while (current)
        {
            if (!current->IsLastPoolPhysicalBlock())
            {
                BlockHeader* nextPhysicalBlock = (BlockHeader*)((uintptr_t)current + current->GetBlockSize() + m_UsedBlockHeaderSize);

                if (current->GetPreviousBlock())
                {
                    ASSERT(physicalBlockHeaders.contains((BlockHeader*)current->GetPreviousBlock()), "Dangling physical block!");
                }

                current = nextPhysicalBlock;
            }
            else
            {
                current = nullptr;
                break;
            }

        }



        // Check for dangling free list blocks

        current = (BlockHeader*)((uintptr_t)m_FirstLevelLists + GetTLSFStructureSize());

        while (current)
        {
            if (!current->IsLastPoolPhysicalBlock())
            {
                BlockHeader* nextPhysicalBlock = (BlockHeader*)((uintptr_t)current + current->GetBlockSize() + m_UsedBlockHeaderSize);

                if (current->IsBlockFree())
                {
                    if (current->GetNextFreeBlock())
                    {
                        ASSERT(physicalBlockHeaders.contains((BlockHeader*)current->GetNextFreeBlock()), "Dangling next free block!");
                    }
                    if (current->GetPreviousFreeBlock())
                    {
                        ASSERT(physicalBlockHeaders.contains((BlockHeader*)current->GetPreviousFreeBlock()), "Dangling previous free block!");
                    }
                }

                current = nextPhysicalBlock;
            }
            else
            {
                current = nullptr;
                break;
            }

        }

    }

}
