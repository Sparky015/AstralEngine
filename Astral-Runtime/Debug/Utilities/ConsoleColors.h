//
// Created by Andrew Fagan on 1/2/25.
//

#pragma once

#include "pch.h"

namespace Astral::Macros {

    enum ConsoleOutputColors : Astral::uint8
    {
        BLACK, BLUE, GREEN,
        CYAN, RED, PURPLE,
        YELLOW, DEFAULT, GRAY,
        LIGHT_BLUE, LIGHT_GREEN,
        LIGHT_CYAN, LIGHT_RED,
        LIGHT_PURPLE, LIGHT_YELLOW, BRIGHT_WHITE
    };

    std::string_view SetColor(ConsoleOutputColors color);

}