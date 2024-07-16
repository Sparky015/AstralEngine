//
// Created by Andrew Fagan on 7/4/24.
//

#pragma once

#include "Ayla/Core/CoreDefinitions.h"


namespace Ayla::Client
{

    class ClientLoop
    {
    public:

        ClientLoop();
        virtual ~ClientLoop();

        /** Provides a method to the client for an update loop. */
        virtual void Update();

        /** Provides a method to the client to initialize anything before the application loop starts. */
        virtual void Init();
    };

    /** Allows the client to give the functions that were derived from ClientLoop to the engine, so it is callable by the engine. */
    extern ClientLoop* LinkClientToEngine();

} // namespace Ayla::Client
