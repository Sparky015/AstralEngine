/**
* @file SpinLock.h
* @author Andrew Fagan
* @date 7/21/26
*/

#pragma once

#include <atomic>

namespace Astral {

    /**
     * @brief Standard spin lock with thread yielding during spins
     */
    class SpinLock
    {
    public:

        /**
         * @brief Spins until the lock is acquired
         * @note May yield thread while spinning
         */
        void lock();

        /**
         * @brief Releases any acquired lock
         */
        void unlock();

    private:

        std::atomic_flag m_WriterFlag;
    };

}