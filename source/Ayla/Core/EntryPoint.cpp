//
// Created by Andrew Fagan on 7/18/24.
//

#include "EntryPoint.h"

#include "Ayla/Client/UserSetup.h"
#include "Ayla/Core/Application.h"
#include "Ayla/Debug/LogUsedToolsAndLibraries.h"

int main(int argc, char *argv[])
{
    #ifdef AYLA_PLATFORM_UNKNOWN
    AY_ERROR("[Ayla] Core/EntryPoint.h: Unsupported/Unknown Platform!");
    #endif
    Ayla::Debug::Macros::initLogForMacros();


    /**
     * Makes the engine on the heap in order to be able to delete the engine and log all the destructor behaviors
     * before I close the logging file.
     */
    Ayla::Core::Application* Engine = new Ayla::Core::Application();

    /** Grab the client's loop and give it to the engine */
    Ayla::Client::ClientLoop* ClientLoop = Ayla::Client::LinkClientToEngine();
    AY_ASSERT(ClientLoop != nullptr, "[Sholas] Core/Application.cpp: ClientLoop is not set up!");
    Engine->SetClientLoop(ClientLoop);

    /** Start the engine's loop */
    Engine->Run();

    delete Engine;

    Ayla::Debug::Macros::closeLogForMacros();

    return 0;
}