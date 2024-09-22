//
// Created by Andrew Fagan on 7/5/24.
//

#pragma once

#include "Solas/aypch.h"

namespace Ayla::Core::Time
{
    /** Shorthands to use because chrono is so verbose */
    using TimeStamp = std::chrono::time_point<std::chrono::high_resolution_clock>;
    using TimeDuration = std::chrono::duration<float>;
};