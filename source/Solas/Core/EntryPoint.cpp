//
// Created by Andrew Fagan on 7/18/24.
//

#include "EntryPoint.h"

#include "Solas/Client/UserSetup.h"
#include "Solas/Core/Application.h"
#include "Solas/Debug/LogUsedToolsAndLibraries.h"

int main(int argc, char *argv[])
{
    #ifdef AYLA_PLATFORM_UNKNOWN
    AY_ERROR("[Ayla] Core/EntryPoint.h: Unsupported/Unknown Platform!");
    #endif
    Solas::Debug::Macros::initLogForMacros();
    AY_LOG_SS("Compiled on " __DATE__ << " " << __TIME__);

    /**
     * Makes the engine on the heap in order to be able to delete the engine and log all the destructor behaviors
     * before I close the logging file.
     */
    Solas::Core::Application* Engine = new Solas::Core::Application();

    /** Grab the client's loop and give it to the engine */
    Solas::Client::ClientLoop* ClientLoop = Solas::Client::LinkClientToEngine();
    AY_ASSERT(ClientLoop != nullptr, "[Sholas] Core/Application.cpp: ClientLoop is not set up!");
    Engine->SetClientLoop(ClientLoop);

    /** Start the engine's loop */
    Engine->Run();

    delete Engine;

    Solas::Debug::Macros::closeLogForMacros();

    return 0;
}