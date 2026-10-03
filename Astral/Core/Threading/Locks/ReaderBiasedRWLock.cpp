/**
* @file ReaderBiasedRWLock.cpp
* @author Andrew Fagan
* @date 7/21/26
*/

#include "ReaderBiasedRWLock.h"

#include <thread>

namespace Astral {

    ReaderBiasedRWLock::ReaderBiasedRWLock() :
        m_ReaderCounts(std::thread::hardware_concurrency()),
        m_WriterActive(false),
        m_WriterLock()
    {

    }


    void ReaderBiasedRWLock::lock_shared()
    {
        while (true)
        {
            while (m_WriterActive.test())
            {
                std::this_thread::yield();
            }

            GetThreadLocalReaderCount().ReaderCount.fetch_add(1);
            if (!m_WriterActive.test()) { break; }
            GetThreadLocalReaderCount().ReaderCount.fetch_sub(1);
        }
    }


    void ReaderBiasedRWLock::unlock_shared()
    {
        GetThreadLocalReaderCount().ReaderCount.fetch_sub(1);
    }


    void ReaderBiasedRWLock::lock()
    {
        m_WriterLock.lock();
        m_WriterActive.test_and_set();

        size_t totalReaderCount = 0;
        do
        {
            totalReaderCount = 0;
            for (ThreadLocalReaderCount& threadLocalReaderCount : m_ReaderCounts)
            {
                totalReaderCount += threadLocalReaderCount.ReaderCount.load();
            }

            std::this_thread::yield();
        }
        while (totalReaderCount > 0);
    }


    void ReaderBiasedRWLock::unlock()
    {
        m_WriterActive.clear();
        m_WriterLock.unlock();
    }


    ReaderBiasedRWLock::ThreadLocalReaderCount& ReaderBiasedRWLock::GetThreadLocalReaderCount()
    {
        size_t threadIDHash = std::hash<std::thread::id>{}(std::this_thread::get_id());
        return m_ReaderCounts[threadIDHash % m_ReaderCounts.size()];
    }

}
