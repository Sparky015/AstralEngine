/**
* @file Engine.h
* @author Andrew Fagan
* @date 10/31/2024
*/

#pragma once

#include "IO/IOManager.h"

class Engine
{
public:
    Engine();
    ~Engine();

    void Run() const;

private:
    IOManager m_IOManager;


};