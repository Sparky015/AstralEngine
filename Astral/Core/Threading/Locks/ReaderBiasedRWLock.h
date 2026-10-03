/**
* @file ReaderBiasedRWLock.h
* @author Andrew Fagan
* @date 7/21/26
*/

#pragma once

#include <vector>
#include <atomic>
#include <mutex>

namespace Astral {

    /**
     * @brief A reader-biased read-write lock that scales with the number of readers.
     * @note Reader counts are stored separately in per-hardware thread counts
     */
    class ReaderBiasedRWLock
    {
    public:
        ReaderBiasedRWLock();

        /**
         * @brief Acquires the reader's shared lock
         */
        void lock_shared();

        /**
         * @brief Releases the reader's shared lock
         */
        void unlock_shared();

        /**
         * @brief Acquires the writer exclusive lock
         */
        void lock();

        /**
         * @brief Releases the writer exclusive lock
         */
        void unlock();

    private:

        struct ThreadLocalReaderCount
        {
            alignas(128) std::atomic_uint32_t ReaderCount; // 128 byte-aligned to handle Apple M1 CPU cache line size
        };

        ThreadLocalReaderCount& GetThreadLocalReaderCount();

        std::vector<ThreadLocalReaderCount> m_ReaderCounts;
        std::atomic_flag m_WriterActive;
        std::mutex m_WriterLock;
    };

}
