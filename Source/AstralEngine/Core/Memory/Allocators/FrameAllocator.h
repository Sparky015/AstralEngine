/**
* @file FrameAllocator.h
* @author Andrew Fagan
* @date 1/20/2025
*/

#pragma once

#include "Core/CoreMacroDefinitions.h"
#include "Debug/Utils/Public/Asserts.h"
#include <memory>

#include "AllocatorUtils.h"

namespace Core {

    /**@brief An allocator that can be used to allocate during a frame. Do not store or cache any pointers from this allocator.
     *        It will be deallocated after the frame has ended.
     * @thread_safety This class is NOT thread safe.
     * @note Copying is not allowed with this allocator. */
    class FrameAllocator
    {
    public:

        explicit FrameAllocator(size_t memoryBlockSize);
        ~FrameAllocator();


        using Marker = unsigned char*;

        /**@brief Gets a marker to the top of the memory block */
        [[nodiscard]] Marker GetMarker() const { return m_CurrentMarker; }

        /**@brief Rolls the stack back to the passed marker. Deallocates memory that was allocated after the marker. */
        void RollbackToMarker(const Marker marker);

        /**@brief Allocates a memory block of the given size with the given required alignment.
         * @param size Size of the requested allocated block
         * @param alignment The alignment requirement for the allocation
         * @return A pointer to the allocated block or nullptr if the allocation failed. */
        void* Allocate(size_t size, uint16 alignment);

        /**@brief Resets ALL memory that the allocator owns. Everything gets deallocated. */
        void Reset();

        /**@brief Gets the amount of memory currently allocated out by the allocator.
         * @return The number of bytes currently allocated. */
        [[nodiscard]] size_t GetUsedBlockSize() const;

        /**@brief Gets the memory capacity of the allocator.
         * @return The max number of bytes the allocator can allocate. */
        [[nodiscard]] size_t GetCapacity() const;

        /**@brief Doubles the size of the internal buffer of the allocator.
         * @return True if the resize operation succeeded and false if the operation failed.
         * @note Only resizes when the allocator is empty. If it is not empty then this function does nothing. */
        [[nodiscard]] bool ResizeBuffer();


        // Deleting copy constructor and operator because the copied data in use won't be able to be freed.
        // The new allocator will copy the same address range (some of that will include addresses in use by the user).
        // Because the new allocator's memory is a freshly allocated, no one has the pointers to the new allocator's memory
        // for the data in use, so they can't be freed.
        FrameAllocator(const FrameAllocator& other) = delete;
        FrameAllocator& operator=(const FrameAllocator& other) = delete;

        FrameAllocator(FrameAllocator&& other) noexcept;
        FrameAllocator& operator=(FrameAllocator&& other) noexcept;

        bool operator==(const FrameAllocator& other) noexcept;
        bool operator!=(const FrameAllocator& other) noexcept;


    private:

        /** @brief Attempts to resize the internal buffer of the allocator.
         *  @return True if the resize operation succeeded and false if the operation failed. */
        inline bool ResizeInternalMemoryBlock();

        unsigned char* m_StartBlockAddress;
        unsigned char* m_EndBlockAddress;
        unsigned char* m_CurrentMarker;
    };

}
