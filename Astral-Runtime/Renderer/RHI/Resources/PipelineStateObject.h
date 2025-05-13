/**
* @file PipelineStateObject.h
* @author Andrew Fagan
* @date 5/13/25
*/

#pragma once

namespace Astral {

    class PipelineStateObject
    {
    public:
        virtual ~PipelineStateObject() = default;


        static PipelineStateObject* CreatePipelineStateObject();
    };

}