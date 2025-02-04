/**
* @file DoubleBufferedAllocator.cpp
* @author Andrew Fagan
* @date 2/3/25
*/

#include "DoubleBufferedAllocator.h"

namespace Core {

    DoubleBufferedAllocator::DoubleBufferedAllocator(size_t memoryBlockSize) :
            m_Buffers{FrameAllocator(memoryBlockSize), FrameAllocator(memoryBlockSize)},
            m_ActiveBuffer(&m_Buffers[0])
    {
        ASSERT(memoryBlockSize > 0, "The memory block size must be greater than 0");
    }


    void DoubleBufferedAllocator::SwapBuffers()
    {
        if (m_ActiveBuffer == &m_Buffers[0])
        {
            m_ActiveBuffer = &m_Buffers[1];
        }
        else
        {
            m_ActiveBuffer = &m_Buffers[0];
        }
    }


    bool DoubleBufferedAllocator::ResizeActiveBuffer()
    {
        if (GetUsedBlockSize() != 0) { return false; }
        return m_ActiveBuffer->ResizeBuffer();
    }


    DoubleBufferedAllocator& DoubleBufferedAllocator::operator=(const DoubleBufferedAllocator& other)
    {
        if (this != &other)
        {
            m_Buffers[0] = other.m_Buffers[0];
            m_Buffers[1] = other.m_Buffers[1];
            m_ActiveBuffer = other.m_ActiveBuffer;
        }
        return *this;
    }


    DoubleBufferedAllocator::DoubleBufferedAllocator(const DoubleBufferedAllocator& other) :
            m_Buffers{other.m_Buffers[0], other.m_Buffers[1]},
            m_ActiveBuffer(other.m_ActiveBuffer)
    {}


    DoubleBufferedAllocator::DoubleBufferedAllocator(DoubleBufferedAllocator&& other) noexcept :
            m_Buffers{std::move(other.m_Buffers[0]), std::move(other.m_Buffers[1])},
            m_ActiveBuffer(other.m_ActiveBuffer == &other.m_Buffers[0] ? &m_Buffers[0] : &m_Buffers[1])
    {
        other.m_ActiveBuffer = nullptr;
    }


    DoubleBufferedAllocator& DoubleBufferedAllocator::operator=(DoubleBufferedAllocator&& other) noexcept
    {
        if (this != &other)
        {
            m_ActiveBuffer = other.m_ActiveBuffer == &other.m_Buffers[0] ? &m_Buffers[0] : &m_Buffers[1];
            m_Buffers[0] = std::move(other.m_Buffers[0]);
            m_Buffers[1] = std::move(other.m_Buffers[1]);
            other.m_ActiveBuffer = nullptr;
        }
        return *this;
    }


    bool DoubleBufferedAllocator::operator==(const DoubleBufferedAllocator& other) noexcept
    {
        return (&m_Buffers[0] == &other.m_Buffers[0] &&
                m_ActiveBuffer == other.m_ActiveBuffer);
    }


    bool DoubleBufferedAllocator::operator!=(const DoubleBufferedAllocator& other) noexcept
    {
        return !(*this == other);
    }

}