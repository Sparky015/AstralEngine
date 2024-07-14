//
// Created by Andrew Fagan on 7/12/24.
//

#include "Ayla/Core/DebugMacros.h"

#include <fstream>
#include <iostream>

namespace Ayla::Core::Macros {


    namespace {
        std::fstream LogFile;
    } // namespace


    void initLogForMacros()
    {
        #ifdef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            return;
        #endif

        const std::time_t t = std::time(nullptr);
        const std::tm* currentTime = std::localtime(&t);

        /**
         * Overall filepath of log file. It will progressively have folders appended to it in the lines
         * following to form the full filepath.
         * */
        std::stringstream filePathStream;

        /** Root folder name for all the console logs. */
        const std::string rootFolder = "../ConsoleLogs";
        filePathStream << rootFolder << '/';

        /** create_directories will fail most of the time because the folders have already been made most of the time. */
        std::filesystem::create_directories(rootFolder); // filePathStream = "ConsoleLog/"

        /** Creates a folder to subdivide the logs by the month and year */
        std::stringstream monthAndYearFolderNameStream;
        monthAndYearFolderNameStream << currentTime->tm_mon + 1 << "-" << currentTime->tm_year + 1900;
        filePathStream << monthAndYearFolderNameStream.str() << '/';
        std::filesystem::create_directories(filePathStream.str()); // filePathStream = "ConsoleLog/[Month]-[Year]/"

        /** Creates another folder to subdivide the logs by the day in a month */
        std::stringstream dayFolderNameStream;
        dayFolderNameStream << currentTime->tm_mday;
        filePathStream << dayFolderNameStream.str() << '/';
        std::filesystem::create_directories(filePathStream.str()); // filePathStream = "ConsoleLog/[Month]-[Year]/[Day]/"

        /** Name of the text file based on the time it was created. */
        std::stringstream hrMinSecTextFileNameStream;
        hrMinSecTextFileNameStream << currentTime->tm_hour << "_" << currentTime->tm_min << "_" << currentTime->tm_sec;

        /** filePathStream = "ConsoleLog/[Month]-[Year]/[Day]/[Hour]_[Minute]_[Second].txt" */
        filePathStream << hrMinSecTextFileNameStream.str() << ".txt";

        LogFile.open(filePathStream.str(), std::ios::out);
        if (LogFile.fail())
        {
            AY_WARN("DebugMacros.cpp: Log file failed to open!");
        }

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
        std::cout << "\n\033[0;36m[" << currentTime->tm_hour
                  << ":" << currentTime->tm_min << ":"
                  << currentTime->tm_sec << "." << elapsedPrecisionTime
                  << "]\033[0;96m " << title << "\033[0m";

        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "\n[" << currentTime->tm_hour
                    << ":" << currentTime->tm_min
                    << ":" << currentTime->tm_sec
                    << "." << elapsedPrecisionTime
                    << "]" << title;
        #endif
    }


    void macro_AY_LOG(const std::string&& message)
    {
        CheckIfCoutFailed();
        std::cout << "\n" << "\033[0;92m" << message << "\033[0m"; // Color is bright green
        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "\n" << message;
        #endif
    }


    void macro_AY_LOG_SS(const std::ostream& message)
    {
        CheckIfCoutFailed();
        std::ostringstream oss;
        oss << message.rdbuf();

        std::cout << "\n\033[0;92m" << oss.str() << "\033[0m"; // Color is bright green
        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "\n" << oss.str();
        #endif
    }


    void macro_AY_WARN(const std::string&& message)
    {
        CheckIfCoutFailed();
        std::cout << "\n\033[0;93m[Warning] " << message << "\033[0m"; // Color is bright yellow
        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "\n" << message;
        #endif
    }


    void macro_AY_ASSERT(const bool expression, const std::ostream& errorMessage)
    {
        if (!expression){
            std::ostringstream oss;
            oss << "\n\n" << errorMessage.rdbuf();
            #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
                LogFile << "Assert failed. Error: " << oss.str();
                LogFile.close();
            #endif
            throw std::runtime_error(oss.str());  // color defaulted to red
        }
    }


    void macro_AY_ERROR(const std::ostream& errorMessage)
    {
        std::ostringstream oss;
        oss << "\n\n" << errorMessage.rdbuf();
        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "AY_ERROR called. Error: " << oss.str();
            LogFile.close();
        #endif
        throw std::runtime_error(oss.str()); // color defaulted to red
    }


    AY_PROFILER::AY_PROFILER(std::string&& title) :
        m_title(std::move(title)),
        m_startTime(std::chrono::high_resolution_clock::now()),
        m_endTime()
    {}


    AY_PROFILER::~AY_PROFILER()
    {
        m_endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(m_endTime - m_startTime);

        std::cout << "\n\033[0;95m[Profiling " << m_title << "] Elapsed Time: " << (float)duration.count() * .000001 << " seconds\033[0m";

        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "\n[Profiling " << m_title << "] Elapsed Time: " << (float)duration.count() * .000001 << " seconds";
        #endif
    }


    void CheckIfCoutFailed()
    {
        if (std::cout.fail())
        {
            std::cout << "\n" << "\033[0;93m" << "std::cout failed!" << "\033[0m"; // Color is bright yellow
            std::cout.clear();
        }
    }

} // namespace Ayla::Core::Macros