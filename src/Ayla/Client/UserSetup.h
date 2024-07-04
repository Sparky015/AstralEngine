//
// Created by Andrew Fagan on 7/4/24.
//

#pragma once

#include "Ayla/Core/Core.h"


namespace Ayla::Client{

    class AYLA_API ClientLoop {
    public:
        ClientLoop();
        virtual ~ClientLoop();

        virtual void Update();
        virtual void Init();

    };

    AYLA_API extern ClientLoop* LinkClientToEngine();
}
