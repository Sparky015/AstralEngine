/**
* @file CommandBuffer.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

namespace Astral {

    class CommandBuffer
    {
    public:
        virtual ~CommandBuffer() = default;


        static CommandBuffer* CreateCommandBuffer();
    };

}