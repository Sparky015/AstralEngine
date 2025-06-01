//
// Created by Andrew Fagan on 1/2/25.
//

#include "ConsoleColors.h"

#ifdef ASTRAL_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

namespace Astral::Macros {

    std::string_view SetColor(ConsoleOutputColors color)
    {
        #ifdef ASTRAL_PLATFORM_MACOS
            switch (color)
            {
                case ConsoleOutputColors::DEFAULT: return "\033[0m";
                case ConsoleOutputColors::CYAN: return "\033[36m";
                case ConsoleOutputColors::LIGHT_CYAN: return "\033[96m";
                case ConsoleOutputColors::YELLOW: return "\033[93m";
                case ConsoleOutputColors::RED: return "\033[31m";
                case ConsoleOutputColors::LIGHT_GREEN: return "\033[92m";
                case ConsoleOutputColors::LIGHT_PURPLE: return "\033[95m";
                default:
                    std::cout << "\n" << "[Color not defined!]";
                    return "";
            }
        #elif ASTRAL_PLATFORM_WINDOWS

            static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

            switch (color)
            {
                case ConsoleOutputColors::DEFAULT:
                    SetConsoleTextAttribute(hConsole, DEFAULT);
                    break;
                case ConsoleOutputColors::CYAN:
                    SetConsoleTextAttribute(hConsole, CYAN);
                    break;
                case ConsoleOutputColors::LIGHT_CYAN:
                    SetConsoleTextAttribute(hConsole, LIGHT_CYAN);
                    break;
                case ConsoleOutputColors::YELLOW:
                    SetConsoleTextAttribute(hConsole, YELLOW);
                    break;
                case ConsoleOutputColors::RED:
                    SetConsoleTextAttribute(hConsole, RED);
                    break;
                case ConsoleOutputColors::LIGHT_GREEN:
                    SetConsoleTextAttribute(hConsole, LIGHT_GREEN);
                    break;
                case ConsoleOutputColors::LIGHT_PURPLE:
                    SetConsoleTextAttribute(hConsole, LIGHT_PURPLE);
                    break;
                default:
                    std::cout << "\n" << "Color not defined!";
            }
            return "";
        #else
            return "";
        #endif
    }

}