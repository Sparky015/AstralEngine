/**
* @file CommandBuffer.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

#include "Renderer/RHI/Common/GraphicsSmartPointers.h"

namespace Astral {

    class CommandBuffer
    {
    public:
        virtual ~CommandBuffer() = default;

        virtual void BeginRecording() = 0;
        virtual void EndRecording() = 0;
        virtual void Reset() = 0;

        virtual void BindPipeline() = 0;
        virtual void BindDescriptorSet() = 0;

        virtual void* GetNativeHandle() = 0;

        bool IsValid() { return m_IsValid; }
        bool IsRecording() { return m_State == State::RECORDING; }
        bool IsRecorded() { return m_State == State::RECORDED; }
        bool IsEmpty() { return m_State == State::EMPTY; }

    protected:

        enum class State
        {
            EMPTY,
            RECORDING,
            RECORDED
        };

        State m_State = State::EMPTY;
        bool m_IsValid = false;
    };

    using CommandBufferHandle = GraphicsRef<CommandBuffer>;

}
