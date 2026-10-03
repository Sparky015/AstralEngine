/**
* @file TLSFAllocator.h
* @author Andrew Fagan
* @date 7/11/26
*/

#pragma once

#include "IAllocator.h"
#include "Core/FixedIntegerTypes.h"
#include "Profiler/Library/Profiler/MemoryTracking/NoTrackingSTLAllocator.h"

#include <unordered_map>


namespace Astral {

    /**
     * @brief A Two-Level Segregated Fit Allocator
     */
    class TLSFAllocator : public IAllocator
    {
    public:
        /**
         * @brief Constructs a TLSF allocator
         * @param memoryPoolSize The memory pool's size
         */
        TLSFAllocator(size_t memoryPoolSize);
        ~TLSFAllocator();

        /**
         * @brief Retrieves a block of free memory of the requested size
         * @param size The size of the memory block to retrieve
         * @return A pointer to a block of free memory of the requested size (or nullptr if there if allocation can not be made)
         */
        void* Allocate(size_t size);

        /**
         * @brief Returns the given memory block to the allocator
         */
        void Free(void* memoryBlock);

        /**
         * @brief Checks if a pointer is owned by the allocator
         * @return True if the allocator owns the pointer
         * @note Pointer can be free or in use
         */
        bool DoesOwnPointer(void* pointer);

        /**
         * @brief Resets all memory that the allocator owns. Every previous allocation is deallocated.
         */
        void Reset() override;

        /**
         * @brief Gets the amount of memory currently allocated out by the allocator.
         * @return The number of bytes currently allocated.
         */
        [[nodiscard]] size_t GetUsedBlockSize() const override;

        /**
         * @brief Gets the memory capacity of the allocator.
         * @return The max number of bytes the allocator can allocate
         * @note The max number of bytes might not be able to be used if more than one allocation is made due to overhead depending on allocator type
         */
        [[nodiscard]] size_t GetCapacity() const override;

        /**
         * @brief Gets the total owned memory size of an allocator (including overhead)
         * @return The total owned memory size of an allocator (including overhead)
         */
        [[nodiscard]] size_t GetOwnedMemorySize() const override;

        /**
         * @brief Gets the allocator's type
         * @return The allocator's type
         */
        [[nodiscard]] AllocatorType GetAllocatorType() const override;

    private:

        /**
         * @brief A struct with the data format of the memory block's header
         */
        struct BlockHeader
        {
            /**
             * @brief Gets the memory block's size
             * @return The memory block's size
             */
            size_t GetBlockSize() const;

            /**
             * @brief Sets the memory block's size
             * @param blockSize The memory block size to set
             */
            void SetBlockSize(size_t blockSize);

            /**
             * @brief Gets the isBlockFree flag in the block header
             * @return The isBlockFree flag value in the block header
             */
            bool IsBlockFree() const;

            /**
             * @brief Sets the isBlockFree flag in the block header
             * @param isBlockFree The value to set the isBlockFree flag to
             */
            void SetIsBlockFree(bool isBlockFree);

            /**
             * @brief Gets the isLastPoolPhysicalBlock flag in the block header
             * @return The isLastPoolPhysicalBlock flag value in the block header
             */
            bool IsLastPoolPhysicalBlock() const;

            /**
             * @brief Sets the isLastPoolPhysicalBlock flag in the block header
             * @param isLastPoolPhysicalBlock The value to set the isLastPoolPhysicalBlock flag to
             */
            void SetIsLastPoolPhysicalBlock(bool isLastPoolPhysicalBlock);

            /**
             * @brief Gets the previous block pointer in the block header (can be nullptr)
             * @return The previous block pointer in the block header (can be nullptr)
             */
            void* GetPreviousBlock() const;

            /**
             * @brief Sets the previous block pointer in the block header (can be nullptr)
             * @param previousBlock The previous block pointer to set in the block header (can be nullptr)
             */
            void SetPreviousBlock(void* previousBlock);

            /**
             * @brief Gets the next free block pointer in the block header (can be nullptr)
             * @return The previous block pointer in the block header (can be nullptr)
             */
            void* GetNextFreeBlock() const;

            /**
             * @brief Sets the next free block pointer in the block header (can be nullptr)
             * @param nextFreeBlock The free block pointer to set in the block header (can be nullptr)
             */
            void SetNextFreeBlock(void* nextFreeBlock);

            /**
             * @brief Gets the previous free block pointer in the block header (can be nullptr)
             * @return The previous free block pointer in the block header (can be nullptr)
             */
            void* GetPreviousFreeBlock() const;

            /**
             * @brief Gets the previous free block pointer in the block header (can be nullptr)
             * @param previousFreeBlock The previous free block pointer to set in the block header (can be nullptr)
             */
            void SetPreviousFreeBlock(void* previousFreeBlock);

        private:

            size_t SizeAndFlags{};
            void* PreviousBlock{};
            void* NextFreeBlock{};     // Only valid for free blocks
            void* PreviousFreeBlock{}; // Only valid for free blocks
        };

        static constexpr uint32 m_UsedBlockHeaderSize = sizeof(size_t) + sizeof(void*); // SizeAndFlags + PreviousBlock in BlockHeader


        /**
         * @brief Initializes the allocator's TLSF structure
         * @param memoryPool A pointer to the allocator's memory pool
         * @param memoryPoolSize The size of the memory pool
         */
        void InitializeTLSFStructure(void* memoryPool, size_t memoryPoolSize);

        /**
         * @brief Calculates the first and second level index mapped for a given block size
         * @param size The size of the memory block to map to a segregated list
         * @param outFirstLevel The output of the mapped first-level index
         * @param outSecondLevel The output of the mapped second-level index
         */
        void Mapping(size_t size, uint32* outFirstLevel, uint32* outSecondLevel);

        /**
         * @brief Gets the first memory block in the segregated list
         * @param blockFirstLevelIndex The first level index to retrieve from
         * @param blockSecondLevelIndex The second level index to retrieve from
         * @return The first memory block in the associated segregated list
         */
        void* GetMemoryBlockFreeList(uint32 blockFirstLevelIndex, uint32 blockSecondLevelIndex);

        /**
         * @brief Sets the first memory block in the segregated list
         * @param memoryBlockHead The memory block to make the head in the segregated list
         * @param blockFirstLevelIndex The first level index of the list to set to
         * @param blockSecondLevelIndex The second level index of the list to set to
         */
        void SetMemoryBlockFreeListHead(void* memoryBlockHead, uint32 blockFirstLevelIndex, uint32 blockSecondLevelIndex);

        /**
         * @brief Sets the first memory block in the segregated list
         * @param memoryBlockHead The memory block to make the head in the segregated list
         * @param size Size of the associated free list
         */
        void SetMemoryBlockFreeListHead(void* memoryBlockHead, uint32 size);

        /**
         * @brief Finds a block suitable to the memory block size requirement
         * @param memoryBlockSize The requested memory block size
         * @return A pointer to a suitable memory block for the requested memory block size
         */
        void* FindSuitableBlockFreeList(size_t memoryBlockSize);

        /**
         * @brief Inserts a memory block into the TLSF structure
         * @param memoryBlock The memory block to insert
         * @param firstLevelIndex The first level index corresponding to the memory block size
         * @param secondLevelIndex The second level index corresponding to the memory block size
         */
        void InsertBlock(void* memoryBlock, uint32 firstLevelIndex, uint32 secondLevelIndex);

        /**
         * @brief Coalesces the given memory block and surrounding free blocks
         * @param memoryBlock The memory block to coalesce
         * @return A coalesced memory block if applicable
         */
        void* CoalesceMemoryBlocks(void* memoryBlock);

        /**
         * @brief Removes block from the TLSF structure
         * @param memoryBlockFreeList The memory block to remove from the TLSF structure
         */
        void RemoveBlock(void* memoryBlockFreeList);

        /**
         * @brief Splits a memory block into a block of the given size and into a second leftover block
         * @param memoryBlock The memory block to split
         * @param reducedBlockSize The size to reduce the given block to
         * @return The remaining leftover block
         */
        void* SplitMemoryBlock(void* memoryBlock, size_t reducedBlockSize);

        // ================================================================

        /**
         * @brief Calculates the memory footprint of the TLSF structure
         * @return The memory footprint of the TLSF structure
         */
        size_t GetTLSFStructureSize() const;


        size_t m_MemoryPoolSize;
        size_t m_NumBlocksCurrentlyAllocated = 0;
        size_t m_SizeOfCurrentlyAllocatedBlocks = 0;
        uintptr_t** m_FirstLevelLists = nullptr;

        /// The number of first-level segregated classes. Classes are a power of two apart from each other.
        uint32 m_NumFirstLevelIndex;

        /// This index subdivides linearly the first-level ranges. For efficiency reasons, SLI has to be a power
        /// of two, and should be in the range [1, 32]. For convenience, SLI is expressed as the log2 of the number
        /// of second-level division (e.g. SLI=4 subdivides each fist-level segregated class into 16 segregated lists).
        /// A bigger second level index leads to smaller fragmentation
        uint32 m_NumSecondLevelIndex;

        /// Defines the minimum block size. For implementation reasons, the MBS constant is set to 16 bytes.
        static constexpr uint32 m_MinimumBlockSize = 16;

        std::unordered_map<const BlockHeader*, size_t, std::hash<const BlockHeader*>, std::equal_to<const void*>, NoTrackingSTLAllocator<std::pair<const BlockHeader* const, size_t>>> m_DebugBlocks;
        size_t m_DebugInitialAlignmentBytes = 0;
        void AssertMemoryUsage();
        void ValidateBlockHeaders();
    };


}
