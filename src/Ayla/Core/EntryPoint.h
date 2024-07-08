//
// Created by sparky on 4/16/2024.
//

#include "Ayla/Core/Application.h"
#include "Ayla/Client/UserSetup.h"


int main(int argc, char *argv[])
{
#ifdef UNKNOWN_PLATFORM
    AY_ERROR("[Ayla] Core/EntryPoint.h: Unsupported/Unknown Platform!");
#endif

    auto Engine = new Ayla::Core::Application();

    /** Grab the client's loop and give it to the engine */
    Ayla::Client::ClientLoop* ClientLoop = Ayla::Client::LinkClientToEngine();
    AY_ASSERT(ClientLoop != nullptr, "[Sholas] Core/Application.cpp: ClientLoop is not set up!");
    Engine->SetClientLoop(ClientLoop);

    /** Start the engine's loop */
    Engine->Run();

    delete Engine;

    exit(EXIT_SUCCESS);
}

