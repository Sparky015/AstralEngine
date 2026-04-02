/**
* @file Resource.h
* @author Andrew Fagan
* @date 5/14/25
*/

#pragma once

namespace Astral {

    class Resource
    {
    public:
        virtual ~Resource();

        virtual void* GetNativeHandle() = 0;
    };

}