//
// Created by Andrew Fagan on 7/12/24.
//

#include "DebugMacros.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#ifdef AYLA_PLATFORM_WINDOWS
    #include <windows.h>
#endif

namespace Ayla::Debug::Macros {

    namespace {
        std::fstream LogFile;

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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
            #ifdef AYLA_PLATFORM_MACOS
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
                }
            #elif defined(AYLA_PLATFORM_WINDOWS)
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


    void initLogForMacros()
    {

    #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE

        const std::time_t t = std::time(nullptr);
        const std::tm* currentTime = std::localtime(&t);

        /**
         * Overall filepath of log file. It will progressively have folders appended to it in the lines
         * following to form the full filepath.
         * */
        std::stringstream filePathStream;

        /** Root folder name for all the console logs. */
        const std::string rootFolder = "../../ConsoleLogs";
        filePathStream << rootFolder << '/';

        /** create_directories will fail most of the time because the folders have already been made most of the time. */
        std::filesystem::create_directories(rootFolder); // filePathStream = "ConsoleLog/"

        /** Creates a folder to subdivide the logs by the month and year */
        std::stringstream monthAndYearFolderNameStream;
        monthAndYearFolderNameStream << currentTime->tm_year + 1900 << "-" << currentTime->tm_mon + 1;
        filePathStream << monthAndYearFolderNameStream.str() << '/';
        std::filesystem::create_directories(filePathStream.str()); // filePathStream = "ConsoleLog/[Year]-[Month]/"

        /** Creates another folder to subdivide the logs by the day in a month */
        std::stringstream dayFolderNameStream;
        dayFolderNameStream << "Day-" << currentTime->tm_mday;
        filePathStream << dayFolderNameStream.str() << '/';
        std::filesystem::create_directories(filePathStream.str()); // filePathStream = "ConsoleLog/[Year]-[Month]/[Day]/"

        /** Name of the text file based on the time it was created. */
        std::stringstream hrMinSecTextFileNameStream;
        hrMinSecTextFileNameStream << currentTime->tm_hour << "-" << currentTime->tm_min << "-" << currentTime->tm_sec;

        /** filePathStream = "ConsoleLog/[Year]-[Month]/[Day]/[Hour]_[Minute]_[Second].txt" */
        filePathStream << hrMinSecTextFileNameStream.str() << ".txt";

        LogFile.open(filePathStream.str(), std::ios::out);
        if (LogFile.fail())
        {
            AY_WARN("DebugMacros.cpp: Log file failed to open!");
        }
    #endif
    }


    void closeLogForMacros()
    {
    #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
        LogFile.close();
        if (LogFile.fail())
        {
            AY_WARN("DebugMacros.cpp: Log file failed to close!");
        }
    #endif
    }


    void macro_AY_TRACE(const std::string&& title)
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
        std::cout << "\n" << SetColor(CYAN) << "[" << currentTime->tm_hour
                  << ":" << currentTime->tm_min << ":"
                  << currentTime->tm_sec << "." << elapsedPrecisionTime
                  << "] " << SetColor(LIGHT_CYAN) << title << SetColor(DEFAULT);

        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "\n[" << currentTime->tm_hour
                    << ":" << currentTime->tm_min
                    << ":" << currentTime->tm_sec
                    << "." << elapsedPrecisionTime
                    << "]" << title;
        #endif
    #endif
    }


    void macro_AY_LOG(const std::string&& message)
    {
    #ifndef TURN_OFF_DEBUG_MACROS
        CheckIfCoutFailed();
        std::cout << "\n" << SetColor(LIGHT_GREEN) << message << SetColor(DEFAULT); // Color is bright green
        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "\n" << message;
        #endif
    #endif
    }


    void macro_AY_LOG_SS(const std::ostream& message)
    {
    #ifndef TURN_OFF_DEBUG_MACROS
        CheckIfCoutFailed();
        std::ostringstream oss;
        oss << message.rdbuf();

        std::cout << "\n" << SetColor(LIGHT_GREEN) << oss.str() << SetColor(DEFAULT); // Color is bright green
        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "\n" << oss.str();
        #endif
    #endif
    }


    void macro_AY_WARN(const std::string&& message)
    {
        #ifndef TURN_OFF_DEBUG_MACROS
            CheckIfCoutFailed();
            std::cout << "\n" << SetColor(YELLOW) << "[Warning] " << message << SetColor(DEFAULT); // Color is bright yellow
            #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
                LogFile << "\n" << message;
            #endif
        #endif
    }


    void macro_AY_WARN_SS(const std::ostream& message)
    {
        #ifndef TURN_OFF_DEBUG_MACROS
            CheckIfCoutFailed();
            std::ostringstream oss;
            oss << message.rdbuf();

            std::cout << "\n" << SetColor(YELLOW) << "[Warning] " << oss.str() << SetColor(DEFAULT); // Color is bright green
            #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
                LogFile << "\n" << oss.str();
            #endif
        #endif
    }


    bool macro_AY_ASSERT(const bool expression, const std::ostream& errorMessage)
    {
    #ifndef TURN_OFF_DEBUG_MACROS
        if (!expression){
            std::ostringstream oss;
            oss << errorMessage.rdbuf() << "\n\n";
            #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
                LogFile << "\n\nAssert failed. \n\nError: " << oss.str();
                LogFile.close();
            #endif
                std::cout << "\n\n" << SetColor(RED) << "AY_ASSERT failed. \n\nError: " << oss.str() << SetColor(DEFAULT);
                return false; // assert failed

            //throw std::runtime_error(oss.str());  // color defaulted to red
        }
    #endif
        return true;
    }


    // Always returns false in order to fail the assert() in the macro. This way, the file name and line is outputted from the assert()
    bool macro_AY_ERROR(const std::ostream& errorMessage)
    {
    #ifndef TURN_OFF_DEBUG_MACROS
        std::ostringstream oss;
        oss << errorMessage.rdbuf() << "\n\n";
        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "\n\nAY_ERROR called. \n\nError: " << oss.str();
            LogFile.close();
        #endif
        std::cout << "\n\n" << SetColor(RED) << "AY_ERROR called. \n\nError: " << oss.str() << "\033[0m";
        return false;
    #else
        return true; // if the debug macros are off then it will always return without causing the error through assert()
    #endif
    }


    AY_PROFILER::AY_PROFILER(std::string&& title)
    #ifndef TURN_OFF_PROFILER_MACRO
        :
        m_title(std::move(title)),
        m_startTime(std::chrono::high_resolution_clock::now()),
        m_endTime()
    #endif
    {}


    AY_PROFILER::~AY_PROFILER()
    {
    #ifndef TURN_OFF_PROFILER_MACRO
        m_endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(m_endTime - m_startTime);

        std::cout << "\n" << SetColor(LIGHT_PURPLE) << "[Profiling " << m_title << "] Elapsed Time: " << (float)duration.count() * .000001 << " seconds" << SetColor(DEFAULT);

        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "\n[Profiling " << m_title << "] Elapsed Time: " << (float)duration.count() * .000001 << " seconds";
        #endif
    #endif
    }


    void CheckIfCoutFailed()
    {
        if (std::cout.fail())
        {
            std::cout << "\n" << SetColor(YELLOW) << "std::cout failed!" << SetColor(DEFAULT);
            std::cout.clear();
        }
    }

} // namespace Ayla::Debug::Macros