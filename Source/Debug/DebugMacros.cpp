//
// Created by Andrew Fagan on 7/12/24.
//

#include "DebugMacros.h"

#include "Debug/ConsoleLogFile.h"
#include "Debug/ProfilerLogFile.h"
#include <sstream>

#include "pch.h"

#ifdef PLATFORM_WINDOWS
    #include <windows.h>
#endif

namespace Debug::Macros {

    namespace {

        enum ConsoleOutputColors : uint8
        {
            BLACK,
            BLUE,
            GREEN,
            CYAN,
            RED,
            PURPLE,
            YELLOW,
            DEFAULT,
            GRAY,
            LIGHT_BLUE,
            LIGHT_GREEN,
            LIGHT_CYAN,
            LIGHT_RED,
            LIGHT_PURPLE,
            LIGHT_YELLOW,
            BRIGHT_WHITE
        };

        std::string SetColor(ConsoleOutputColors color)
        {
            #ifdef PLATFORM_MACOS
                switch (color)
                {
                    case ConsoleOutputColors::DEFAULT:
                        return "\033[0m";
                    case ConsoleOutputColors::CYAN:
                        return "\033[36m";
                    case ConsoleOutputColors::LIGHT_CYAN:
                        return "\033[96m";
                    case ConsoleOutputColors::YELLOW:
                        return "\033[93m";
                    case ConsoleOutputColors::RED:
                        return "\033[31m";
                    case ConsoleOutputColors::LIGHT_GREEN:
                        return "\033[92m";
                    case ConsoleOutputColors::LIGHT_PURPLE:
                        return "\033[95m";
                    default:
                        std::cout << "\n" << "Color not defined!";
                        return "";
                }
            #elif PLATFORM_WINDOWS

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

    } // namespace


//TODO: Utilize __func__ __FILE__ and __LINE__ in the debugging macros
    void macro_TRACE(const std::ostringstream& message)
    {
    #ifndef TURN_OFF_DEBUG_MACROS
        CheckIfCoutFailed();

        /** Gets the current time */
        const std::time_t t = std::time(nullptr);
        const std::tm* currentTime = std::localtime(&t); // Puts the information of the time into a struct that is separated into min, sec, hr, day, etc.


    #ifdef DEBUG_TRACE_USE_MICROSECONDS
        /** Calculates the elapsed microseconds as they were not included in the std::tm struct */
        auto microsecondsSinceEpoch = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        int64 elapsedPrecisionTime = microsecondsSinceEpoch % 1000000; // 1000000 microseconds in a millisecond
    #else
        /** Calculates the elapsed milliseconds as they were not included in the std::tm struct */
        int64 millisecondsSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        int64 elapsedPrecisionTime = millisecondsSinceEpoch % 1000;
    #endif

        /** Outputs the message with the time info prefixing it */
        std::cout << SetColor(CYAN) << "[" << currentTime->tm_hour
                  << ":" << currentTime->tm_min << ":"
                  << currentTime->tm_sec << "." << elapsedPrecisionTime
                  << "] " << SetColor(LIGHT_CYAN) << message.str() << SetColor(DEFAULT) << "\n";

        #ifdef TURN_ON_LOGGING_CONSOLE_TO_FILE
        ConsoleLogFile& logFile = ConsoleLogFile::GetInstance();
        if (logFile.IsOpen())
        {
            logFile.GetFileStream() << "[" << currentTime->tm_hour
                    << ":" << currentTime->tm_min
                    << ":" << currentTime->tm_sec
                    << "." << elapsedPrecisionTime
                    << "] " << message.str() << "\n";
        }
        else
        {
            WARN("Attempted write to a log file that is already closed!");
        }
        #endif
    #endif
    }


    void macro_LOG(const std::ostringstream& message)
    {
    #ifndef TURN_OFF_DEBUG_MACROS
        CheckIfCoutFailed();

        std::cout << SetColor(LIGHT_GREEN) << message.str() << SetColor(DEFAULT) << "\n"; // Color is bright green
        #ifdef TURN_ON_LOGGING_CONSOLE_TO_FILE
        ConsoleLogFile& logFile = ConsoleLogFile::GetInstance();
        if (logFile.IsOpen())
        {
            logFile.GetFileStream() << message.str() << "\n";
        }
        else
        {
            WARN("Attempted write to a log file that is already closed!");
        }
        #endif
    #endif
    }


    void macro_WARN(const std::ostringstream& message, const char* file, int line)
    {
        #ifndef TURN_OFF_DEBUG_MACROS
            CheckIfCoutFailed();

            std::cout << SetColor(YELLOW) << "[Warning] "  << "[" << file << ": Line " << line << "] " << message.str() << SetColor(DEFAULT) << "\n";
            #ifdef TURN_ON_LOGGING_CONSOLE_TO_FILE
            ConsoleLogFile& logFile = ConsoleLogFile::GetInstance();
            if (logFile.IsOpen())
            {
                logFile.GetFileStream() << message.str() << "\n";
            }
            else
            {
                std::cout << SetColor(YELLOW) << "[Warning] [In macro_WARN] Attempted write to a log file that is already closed!" << SetColor(DEFAULT) << "\n";
            }
            #endif
        #endif
    }


    bool macro_ASSERT(const bool expression, std::ostringstream& errorMessage)
    {
    #ifndef TURN_ON_DEBUG_MACROS
        if (!expression){
            errorMessage << "\n\n";
            #ifdef TURN_ON_LOGGING_CONSOLE_TO_FILE
            ConsoleLogFile& logFile = ConsoleLogFile::GetInstance();
            if (logFile.IsOpen())
            {
                logFile.GetFileStream() << "\n\nAssert failed. \n\nError: " << errorMessage.str();
                ConsoleLogFile::Shutdown();
            }
            else
            {
                WARN("Attempted write to a log file that is already closed!");
            }
            #endif
                std::cout << "\n\n" << SetColor(RED) << "ASSERT failed. \n\nError: " << errorMessage.str() << SetColor(DEFAULT);
                return false; // assert failed

            //throw std::runtime_error(errorMessage.str());  // color defaulted to red
        }
    #endif
        return true;
    }

// TODO: Redesign this without the assert and use compiler intrinsics instead
    // Always returns false in order to fail the assert() in the macro. This way, the file name and line is outputted from the assert()
    bool macro_ERROR(std::ostringstream& errorMessage)
    {
    #ifndef TURN_OFF_DEBUG_MACROS
        errorMessage << "\n\n";
        #ifdef TURN_ON_LOGGING_CONSOLE_TO_FILE
        ConsoleLogFile& logFile = ConsoleLogFile::GetInstance();
        if (logFile.IsOpen())
        {
            logFile.GetFileStream() << "\n\nERROR called. \n\nError: " << errorMessage.str();
            ConsoleLogFile::Shutdown();
        }
        else
        {
            WARN("Attempted write to a log file that is already closed!");
        }
        #endif
        std::cout << "\n\n" << SetColor(RED) << "ERROR called. \n\nError: " << errorMessage.str() << "\033[0m";
        return false;
    #else
        return true; // if the debug macros are off then it will always return without causing the error through assert()
    #endif
    }


    macro_SCOPE_PROFILER::macro_SCOPE_PROFILER(const char* title) noexcept
        : m_title(title),
        m_startTime(std::chrono::high_resolution_clock::now())
    {}


    macro_SCOPE_PROFILER::~macro_SCOPE_PROFILER() noexcept
    {
        m_endTime = std::chrono::high_resolution_clock::now();

        int64 startTimePoint = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTime).time_since_epoch().count();
        int64 endTimePoint = std::chrono::time_point_cast<std::chrono::microseconds>(m_endTime).time_since_epoch().count();

        ProfilerLogFile::GetInstance().WriteProfile({m_title,startTimePoint,endTimePoint});
    }


    void CheckIfCoutFailed()
    {
        if (std::cout.fail())
        {
            std::cout << "\n" << SetColor(YELLOW) << "std::cout failed!" << SetColor(DEFAULT);
            std::cout.clear();
        }
    }

} // namespace Debug::Macros