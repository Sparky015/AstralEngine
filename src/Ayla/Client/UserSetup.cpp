//
// Created by Andrew Fagan on 7/4/24.
//

#include "UserSetup.h"

namespace Ayla::Client {


    ClientLoop::ClientLoop()
    {
        AY_TRACE("\n[Sholas] Client: Initializing ClientLoop");
    }


    ClientLoop::~ClientLoop()
    {
        AY_TRACE("[Sholas] Client: Destroying ClientLoop");
    }


    /** Init is not a pure virtual function so an error can occur if the client does not override the Init function */
    void ClientLoop::Init()
    {
        AY_ERROR("[Ayla] Client.cpp: No Init function was set!");
    }


    /** Update is not a pure virtual function so an error can occur if the client does not override the Update function */
    void ClientLoop::Update()
    {
        AY_ERROR("[Ayla] Client.cpp: No Update function was set!");
    }

}


