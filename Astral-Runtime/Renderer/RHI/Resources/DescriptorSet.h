/**
* @file DescriptorSet.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

namespace Astral {

    class DescriptorSet
    {
    public:
        virtual ~DescriptorSet() = default;


        static DescriptorSet* CreateDescriptorSet();
    };

}