/**
* @file CommandQueue.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

namespace Astral {

    class CommandQueue
    {
    public:
        virtual ~CommandQueue() = default;


        static CommandQueue* CreateCommandQueue();
    };

}