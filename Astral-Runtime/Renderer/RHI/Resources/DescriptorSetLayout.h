/**
* @file DescriptorSetLayout.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

namespace Astral {

    class DescriptorSetLayout
    {
    public:
        virtual ~DescriptorSetLayout() = default;


        static DescriptorSetLayout* CreateDescriptorSetLayout();
    };

}