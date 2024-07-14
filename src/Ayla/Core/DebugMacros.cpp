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
            AY_WARN("DebugMacros.cpp: Failed to create console logs root folder!");
        }

        /** Creates a folder to subdivide the logs by the month and year */
        std::stringstream monthAndYearFolderNameStream;
        monthAndYearFolderNameStream << currentTime->tm_mon + 1 << "-" << currentTime->tm_year + 1900;
        filePathStream << monthAndYearFolderNameStream.str() << '/';
        if (std::filesystem::create_directories(filePathStream.str()) == false)  // filePathStream = "ConsoleLog/[Month]-[Year]/"
        {
            AY_WARN("DebugMacros.cpp: Failed to create console logs month/year folder!");
        }

        /** Creates another folder to subdivide the logs by the day in a month */
        std::stringstream dayFolderNameStream;
        dayFolderNameStream << currentTime->tm_mday;
        filePathStream << dayFolderNameStream.str() << '/';
        if (std::filesystem::create_directories(filePathStream.str()) == false)  // filePathStream = "ConsoleLog/[Month]-[Year]/[Day]/"
        {
            AY_WARN("DebugMacros.cpp: Failed to create console logs day folder!");
        }

        /** Name of the text file based on the time it was created. */
        std::stringstream hrMinSecTextFileNameStream;
        hrMinSecTextFileNameStream << currentTime->tm_hour << "_" << currentTime->tm_min << "_" << currentTime->tm_sec;

        /** filePathStream = "ConsoleLog/[Month]-[Year]/[Day]/[Hour]_[Minute]_[Second].txt" */
        filePathStream << hrMinSecTextFileNameStream.str() << ".rtf";

        LogFile.open(filePathStream.str(), std::ios::out);
        if (LogFile.fail())
        {
            AY_WARN("DebugMacros.cpp: Log file failed to open!");
        }
        LogFile << "{\\rtf1\\ansi\\ansicpg1252\\cocoartf2759 \\cocoatextscaling0\\cocoaplatform0{\\fonttbl\\f0\\fswiss\\fcharset0 Helvetica;} \\"
                   "\\colortbl;\\red255\\green0\\blue0;\\red0\\green128\\blue0;\\red0\\green0\\blue255;";
    }


    void closeLogForMacros()
    {
    #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
        //LogFile << "}";
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

        /** Calculates the elapsed milliseconds as they were not included in the std::tm struct */
        int64 millisecondsSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        int64 elapsedMilliseconds = millisecondsSinceEpoch % 1000;

        /** Outputs the message with the time info prefixing it */
        std::cout << "\n\033[0;36m[" << currentTime->tm_hour
                  << ":" << currentTime->tm_min << ":"
                  << currentTime->tm_sec << ":" << elapsedMilliseconds
                  << "]\033[0;96m " << title << "\033[0m";

        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "\n \\cf1[" << currentTime->tm_hour
                    << ":" << currentTime->tm_min
                    << ":" << currentTime->tm_sec
                    << ":" << elapsedMilliseconds
                    << "] \\cf2" << title << "\\cf0 \\";
        #endif
    }


    void macro_AY_LOG(const std::string&& message)
    {
        CheckIfCoutFailed();
        std::cout << "\n" << "\033[0;92m" << message << "\033[0m"; // Color is bright green
        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "\n\\cf1" << message << "\\cf0 \\";
        #endif
    }


    void macro_AY_WARN(const std::string&& message)
    {
        CheckIfCoutFailed();
        std::cout << "\n" << "\033[0;93m" << message << "\033[0m"; // Color is bright yellow
        #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
            LogFile << "\n\\cf2" << message << "\\cf0 \\";
        #endif
    }


    void macro_AY_ASSERT(const bool expression, const std::ostream& errorMessage)
    {
        if (!expression){
            std::ostringstream oss;
            oss << "\n\n" << errorMessage.rdbuf();
            #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
                LogFile << "\\cf1 AY_ASSERT failed. Error: " << oss.str() << " \\";
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
            LogFile << "\\cf1 AY_ERROR called. Error: " << oss.str() << " \\";
            LogFile.close();
        #endif
        throw std::runtime_error(oss.str()); // color defaulted to red
    }


    AY_PROFILER::AY_PROFILER(std::string&& title)
    {
        m_title = title;
        //m_startTime = std::chrono::high_resolution_clock::now();
    }


    AY_PROFILER::~AY_PROFILER()
    {
        //m_endTime = std::chrono::high_resolution_clock::now(); //TODO: add color to profiler output
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
            std::cout << "\n" << "\033[0;93m" << "std::cout failed!" << "\033[0m"; // Color is bright yellow
            std::cout.clear();
        }
    }

} // namespace Ayla::Core::Macros