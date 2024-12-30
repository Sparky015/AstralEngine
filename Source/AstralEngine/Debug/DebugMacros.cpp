//
// Created by Andrew Fagan on 7/12/24.
//

#include "DebugMacros.h"

#include "Loggers/ConsoleLogFile.h"
#include "Loggers/ProfilerLogFile.h"
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


    void macro_TRACE(const std::ostringstream& message)
    {
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
    }


    void macro_LOG(const std::ostringstream& message)
    {
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
    }


    void macro_WARN(const std::ostringstream& message, const char* file, int line)
    {
        CheckIfCoutFailed();

        std::cout << SetColor(YELLOW) << "[Warning] "  << "[" << file << ": Line " << line << "] " << message.str() << SetColor(DEFAULT) << "\n";

    #ifdef TURN_ON_LOGGING_CONSOLE_TO_FILE
        ConsoleLogFile& logFile = ConsoleLogFile::GetInstance();
        if (logFile.IsOpen())
        {
            logFile.GetFileStream() << message.str() << "\n";
        }
    #endif
    }


    void macro_ASSERT(const char* expressionString,  const char* errorMessage, const char* file, const int lineNumber, const char* func)
    {
        //// IF ASSERT FAILED ////

        // Stripping the part of the file path prior to the root directory
        std::string filePath(file);
        std::string rootDir(ROOT_DIR);
        size_t pos = filePath.find(rootDir);
        if (pos != std::string::npos)
        {
            filePath.erase(pos, rootDir.length());
        }

#ifdef TURN_ON_LOGGING_CONSOLE_TO_FILE
        ConsoleLogFile& logFile = ConsoleLogFile::GetInstance();
        if (logFile.IsOpen())
        {
            logFile.GetFileStream() << "\n\nASSERT failed. "
                                    << "\nFile: " << filePath
                                    << "\nLine: " << lineNumber
                                    << "\nFunction: " << func
                                    << "\nCondition: (" << expressionString
                                    << ")\nError Message: " << errorMessage << "\n";
            ConsoleLogFile::Shutdown();
        }
        else
        {
            WARN("Attempted write to a log file that is already closed!");
        }
#endif

        std::cout << "\n\n" << SetColor(RED)
                  << "ASSERT failed. "
                  << "\nFile: " << filePath
                  << "\nLine: " << lineNumber
                  << "\nFunction: " << func
                  << "\nCondition: (" << expressionString
                  << ")\nError Message: " << errorMessage << SetColor(DEFAULT) << "\n";
    }


    void macro_ASSERT(const char* expressionString, std::ostringstream& errorMessage, const char* file, const int lineNumber, const char* func)
    {
        //// IF ASSERT FAILED ////

        // Stripping the part of the file path prior to the root directory
        std::string filePath(file);
        std::string rootDir(ROOT_DIR);
        size_t pos = filePath.find(rootDir);
        if (pos != std::string::npos)
        {
            filePath.erase(pos, rootDir.length());
        }

    #ifdef TURN_ON_LOGGING_CONSOLE_TO_FILE
        ConsoleLogFile& logFile = ConsoleLogFile::GetInstance();
        if (logFile.IsOpen())
        {
            logFile.GetFileStream()
                    << "\n\nASSERT failed. "
                    << "\nFile: " << filePath
                    << "\nLine: " << lineNumber
                    << "\nFunction: " << func
                    << "\nCondition: (" << expressionString
                    << ")\nError Message: " << errorMessage.str() << "\n";
            ConsoleLogFile::Shutdown();
        }
        else
        {
            WARN("Attempted write to a log file that is already closed!");
        }
    #endif

        std::cout << "\n\n" << SetColor(RED)
        << "ASSERT failed. "
        << "\nFile: " << filePath
        << "\nLine: " << lineNumber
        << "\nFunction: " << func
        << "\nCondition: (" << expressionString
        << ")\nError Message: " << errorMessage.str() << SetColor(DEFAULT) << "\n";
    }


    void macro_ERROR(std::ostringstream& errorMessage, const char* file, const int lineNumber, const char* func)
    {
        // Stripping the part of the file path prior to the root directory
        std::string filePath(file);
        std::string rootDir(ROOT_DIR);
        size_t pos = filePath.find(rootDir);
        if (pos != std::string::npos)
        {
            filePath.erase(pos, rootDir.length());
        }

    #ifdef TURN_ON_LOGGING_CONSOLE_TO_FILE
        ConsoleLogFile& logFile = ConsoleLogFile::GetInstance();
        if (logFile.IsOpen())
        {
            logFile.GetFileStream()
                    << "\n\nERROR called. \n" << filePath
                    << "\nLine: " << lineNumber
                    << "\nFunction: " << func
                    << "\nError Message: " << errorMessage.str() << "\n";
            ConsoleLogFile::Shutdown();
        }
        else
        {
            WARN("Attempted write to a log file that is already closed!");
        }
    #endif

        std::cout << "\n\n" << SetColor(RED)
        << "ERROR called. \nFile: " << filePath
        << "\nLine: " << lineNumber
        << "\nFunction: " << func
        << "\nError Message: " << errorMessage.str() << SetColor(DEFAULT) << "\n";
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