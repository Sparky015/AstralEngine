/**
* @file DescriptorPool.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

namespace Astral {

    class DescriptorPool
    {
    public:
        virtual ~DescriptorPool() = default;


        static DescriptorPool* CreateDescriptorPool();
    };

}