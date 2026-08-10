/**
* @file SpinLock.cpp
* @author Andrew Fagan
* @date 7/21/26
*/

#include "SpinLock.h"

#include <thread>

namespace Astral {

    void SpinLock::lock()
    {
        while (m_WriterFlag.test_and_set(std::memory_order_acquire))
        {
            while (m_WriterFlag.test(std::memory_order_relaxed))
            {
                std::this_thread::yield();
            }
        }
    }


    void SpinLock::unlock()
    {
        m_WriterFlag.clear(std::memory_order_release);
    }

}
