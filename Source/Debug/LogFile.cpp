//
// Created by Andrew Fagan on 11/17/24.
//

#include "LogFile.h"

namespace Debug {

     bool LogFile::m_IsLogFileOpen = false;

    void LogFile::Init()
    {
#ifdef TURN_ON_LOGGING_CONSOLE_TO_FILE

        const std::time_t t = std::time(nullptr);
        const std::tm* currentTime = std::localtime(&t);

        /**
         * Overall filepath of log file. It will progressively have folders appended to it in the lines
         * following to form the full filepath.
         * */
        std::stringstream filePathStream;

        /** Root folder name for all the console logs. */
        const std::string rootFolder = std::string(ROOT_DIR) + "/ConsoleLogs";
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

        GetFileStream().open(filePathStream.str(), std::ios::out);
        if (GetFileStream().fail())
        {
            WARN("DebugMacros.cpp: Log file failed to open!");
        }
        else
        {
            m_IsLogFileOpen = true;
            std::atexit(Shutdown);
        }
#endif
    }


    void LogFile::Shutdown()
    {
#ifdef TURN_ON_LOGGING_CONSOLE_TO_FILE
        LogFile& logFile = LogFile::GetInstance();
        logFile.GetFileStream().close();
        if (logFile.GetFileStream().fail())
        {
            WARN("DebugMacros.cpp: Log file failed to close!");
        }
        else
        {
            m_IsLogFileOpen = false;
        }
#endif
    }

    LogFile::LogFile()
    {
        Init();
    }

    LogFile::~LogFile()
    {

    }

} // Debug