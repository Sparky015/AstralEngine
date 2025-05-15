/**
* @file VulkanSemaphore.h
* @author Andrew Fagan
* @date 5/14/25
*/

#pragma once

namespace Astral {

    class VulkanSemaphore
    {
    public:

        // TODO:
        // - Evaluate the base Resource class to enforce GetNativeHandle
        // - Evaluate how to apply smart pointers to resource objects
        // - Typedef objects with smart pointers to easier names to use
        // - Add description structs to resource objects that need it
        // - Note: The Device can prepare and create all the resources needed for a resource to function. It has the
        //         most context, so let it cook all the ingredients together and pass it through the description
        //         structs. Let the Device object manage all the actual creation logic of vulkan objects. This way,
        //         I don't have to pass in the device object into everything and then create objects with the device
        //         inside the resource objects (which is very cumbersome and tedious when changes are needed).
        //         This also allows the resource objects to be more lean and manage just the functionality that it owns.
        // TODO:
    };

}