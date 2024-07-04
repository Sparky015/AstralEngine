//
// Created by sparky on 4/16/2024.
//

#include "Ayla/Core/Application.h"
#include "Ayla/Client/UserSetup.h"


int main(int argc, char *argv[]) {
#ifdef UNKNOWN_PLATFORM
    AY_ERROR("[Ayla] Core/EntryPoint.h: Unsupported/Unknown Platform!");
#endif

    auto Engine = new Ayla::Core::Application();
    Engine->setClientLoop(Ayla::Client::LinkClientToEngine());
    Engine->Run();
    delete Engine;
    return 0;

}

