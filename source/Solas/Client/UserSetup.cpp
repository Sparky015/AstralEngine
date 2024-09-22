//
// Created by Andrew Fagan on 7/4/24.
//

#include "UserSetup.h"

namespace Solas::Client
{

    ClientLoop::ClientLoop()
    {
        AY_TRACE("[Sholas] Client: Initializing ClientLoop");
    }


    ClientLoop::~ClientLoop()
    {
        AY_TRACE("[Sholas] Client: Destroying ClientLoop");
    }


    /** Init is not a pure virtual function so we can throw an error if the client does not override the Init function */
    void ClientLoop::Init()
    {
        AY_ERROR("[Ayla] Client.cpp: Init function was not defined or overridden!");
    }


    /** Update is not a pure virtual function so we can throw an error if the client does not override the Update function */
    void ClientLoop::Update(int deltaTime)
    {
        AY_ERROR("[Ayla] Client.cpp: Update function was not defined or overridden!");
    }

} // namespace Solas::Client


