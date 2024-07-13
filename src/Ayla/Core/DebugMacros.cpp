//
// Created by Andrew Fagan on 7/12/24.
//

#include "Ayla/Core/DebugMacros.h"

#include <fstream>
#include <iostream>

namespace Ayla::Core::Macros {

    std::fstream LogFile;

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
        if (std::filesystem::create_directories(rootFolder) == false) // filePathStream = "ConsoleLog/"
        {
            std::cout << "\nDebugMacros.cpp: Failed to create console logs root folder";
        }

        /** Creates a folder to subdivide the logs by the month and year */
        std::stringstream monthAndYearFolderNameStream;
        monthAndYearFolderNameStream << currentTime->tm_mon + 1 << "-" << currentTime->tm_year + 1900;
        filePathStream << monthAndYearFolderNameStream.str() << '/';
        if (std::filesystem::create_directories(filePathStream.str()) == false)  // filePathStream = "ConsoleLog/[Month]-[Year]/"
        {
            std::cout << "\nDebugMacros.cpp: Failed to create console logs month/year folder";
        }

        /** Creates another folder to subdivide the logs by the day in a month */
        std::stringstream dayFolderNameStream;
        dayFolderNameStream << currentTime->tm_mday;
        filePathStream << dayFolderNameStream.str() << '/';
        if (std::filesystem::create_directories(filePathStream.str()) == false)  // filePathStream = "ConsoleLog/[Month]-[Year]/[Day]/"
        {
            std::cout << "\nDebugMacros.cpp: Failed to create console logs day folder";
        }

        /** Name of the text file based on the time it was created. */
        std::stringstream hrMinSecTextFileNameStream;
        hrMinSecTextFileNameStream << currentTime->tm_hour << "_" << currentTime->tm_min << "_" << currentTime->tm_sec;

        /** filePathStream = "ConsoleLog/[Month]-[Year]/[Day]/[Hour]_[Minute]_[Second].txt" */
        filePathStream << hrMinSecTextFileNameStream.str() << ".txt";

        LogFile.open(filePathStream.str(), std::ios::out);
        if (LogFile.fail())
        {
            AY_ERROR("Log file failed to open!") //TODO: Change this to a warning (AY_WARN)
        }
    }


    void closeLogForMacros()
    {
    #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
        LogFile.close();
        if (LogFile.fail())
        {
            AY_ERROR("Log file failed to close!") //TODO: Change this to a warning (AY_WARN)
        }
    #endif
    }


    void macro_AY_TRACE(const std::string&& title)
    {
        CheckIfCoutFailed();
        /** Gets the current time */
        const std::time_t t = std::time(nullptr);
        const std::tm* currentTime = std::localtime(&t); // Puts the information of the time into a struct that is separated into min, sec, hr, day, etc.

        /** Calculates the elapsed milliseconds as they were not included in the std::tm struct */
        int64 millisecondsSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        int64 elapsedMilliseconds = millisecondsSinceEpoch % 1000;

        /** Outputs the message with the time info prefixing it */
        std::cout << "\n\033[0;32m[" << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << ":" << elapsedMilliseconds << "] " << title << "\033[0m";
    #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
        LogFile << "\n[" << currentTime->tm_hour << ":" << currentTime->tm_min << ":" << currentTime->tm_sec << ":" << elapsedMilliseconds << "] " << title;
    #endif

    }


    void macro_AY_ASSERT(const bool expression, const std::ostream& errorMessage)
    {
        if (!expression){
            std::ostringstream oss;
            oss << "\n\n" << errorMessage.rdbuf();
            #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
                LogFile << "AY_ASSERT failed. Error: " << oss.str();
                LogFile.close();
            #endif
            throw std::runtime_error(oss.str());
        }
    }


    void macro_AY_LOG(const std::string&& message)
    {
        CheckIfCoutFailed();
        std::cout << "\n" << "\033[0;34m" << message << "\033[0m";
        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "\n" << message;
        #endif

    }


    void macro_AY_ERROR(const std::ostream& errorMessage)
    {
        std::ostringstream oss;
        oss << "\n\n" << errorMessage.rdbuf();
        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "AY_ERROR called. Error: " << oss.str();
            LogFile.close();
        #endif
        throw std::runtime_error(oss.str());
    }


    AY_PROFILER::AY_PROFILER(std::string&& title)
    {
        m_title = title;
        //m_startTime = std::chrono::high_resolution_clock::now();
    }


    AY_PROFILER::~AY_PROFILER()
    {
        //m_endTime = std::chrono::high_resolution_clock::now();
        //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(m_endTime - m_startTime);
        //std::cout << "\n" << m_title << " time: " << duration.count() << " microseconds\n" << std::endl;
        //#ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            //LogFile << "\n" << m_title << " time: " << duration.count() << " microseconds\n" << std::endl;
        //#endif
    }


    void CheckIfCoutFailed()
    {
        if (std::cout.fail())
        {
            AY_ERROR("std::cout failed"); //TODO: Change this to a warning (AY_WARN)
            std::cout.clear();
        }
    }

} // namespace Ayla::Core::Macros