//
// Created by Andrew Fagan on 1/2/25.
//

#include "Loggers.h"


#include "ConsoleLogFile.h"
#include "ConsoleColors.h"
#include "Core/CoreMacroDefinitions.h"

namespace Astral::Macros {

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
            std::ostringstream outputStream;
            outputStream  << "[" << currentTime->tm_hour
                    << ":" << currentTime->tm_min
                    << ":" << currentTime->tm_sec
                    << "." << elapsedPrecisionTime
                    << "] " << message.str() << "\n";
            logFile << outputStream;
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
            std::ostringstream outputStream;
            outputStream << message.str() << "\n";
            logFile << outputStream;
        }
        else
        {
            WARN("Attempted write to a log file that is already closed!");
        }
#endif
    }


    void macro_WARN(const std::ostringstream& message)
    {
        CheckIfCoutFailed();

        std::cout << SetColor(YELLOW) << "[Warning] " << message.str() << SetColor(DEFAULT) << "\n";

    #ifdef TURN_ON_LOGGING_CONSOLE_TO_FILE
        ConsoleLogFile& logFile = ConsoleLogFile::GetInstance();
        if (logFile.IsOpen())
        {
            std::ostringstream outputStream;
            outputStream  << message.str() << "\n";
            logFile << outputStream;
        }
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

}
