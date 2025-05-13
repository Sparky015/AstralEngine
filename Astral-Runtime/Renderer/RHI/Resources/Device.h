/**
* @file Device.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

namespace Astral {

    class Device
    {
    public:
        virtual ~Device() = default;


        static Device* CreateDevice();
    };

}