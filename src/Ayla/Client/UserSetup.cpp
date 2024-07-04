//
// Created by Andrew Fagan on 7/4/24.
//

#include "UserSetup.h"

namespace Ayla::Client{

    ClientLoop::ClientLoop() {
        AY_TRACE("\n[Sholas] Client: Initializing ClientLoop");
    }

    ClientLoop::~ClientLoop() {
        AY_TRACE("[Sholas] Client: Destroying ClientLoop");
    }

    void ClientLoop::Init() {
        AY_ERROR("[Ayla] Client.cpp: No Init function was set!");
    }

    void ClientLoop::Update() {
        AY_ERROR("[Ayla] Client.cpp: No Update function was set!");
    }

}


