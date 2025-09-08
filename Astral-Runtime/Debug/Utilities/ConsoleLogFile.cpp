//
// Created by Andrew Fagan on 11/17/24.
//

#include "ConsoleLogFile.h"

#include <filesystem>
#include <sstream>

#include "Loggers.h"

namespace Astral {

     bool ConsoleLogFile::m_IsLogFileOpen = false;

    void ConsoleLogFile::Init()
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
        const std::string rootFolder = std::string(ROOT_DIR) + "Logs";
        filePathStream << rootFolder << '/';

        /** create_directories will fail most of the time because the folders have already been made most of the time. */
        std::filesystem::create_directories(rootFolder); // filePathStream = "ConsoleLog/"

        /** Creates a folder to subdivide the logs by the month and year */
        std::stringstream monthAndYearFolderNameStream;
        monthAndYearFolderNameStream << currentTime->tm_year + 1900 << "-" << currentTime->tm_mon + 1;
        filePathStream << monthAndYearFolderNameStream.view() << '/';
        std::filesystem::create_directories(filePathStream.view()); // filePathStream = "ConsoleLog/[Year]-[Month]/"

        /** Creates another folder to subdivide the logs by the day in a month */
        std::stringstream dayFolderNameStream;
        dayFolderNameStream << "Day-" << currentTime->tm_mday;
        filePathStream << dayFolderNameStream.str() << '/';
        std::filesystem::create_directories(filePathStream.view()); // filePathStream = "ConsoleLog/[Year]-[Month]/[Day]/"

        /** Name of the text file based on the time it was created. */
        std::stringstream hrMinSecTextFileNameStream;
        hrMinSecTextFileNameStream << currentTime->tm_hour << "-" << currentTime->tm_min << "-" << currentTime->tm_sec << "-Console";

        /** filePathStream = "ConsoleLog/[Year]-[Month]/[Day]/[Hour]_[Minute]_[Second].txt" */
        filePathStream << hrMinSecTextFileNameStream.view() << ".txt";

        GetFileStream().open(filePathStream.str(), std::ios::out);
        if (GetFileStream().fail())
        {
            AE_WARN("Console log file failed to open!");
        }
        else
        {
            m_IsLogFileOpen = true;
            std::atexit(Shutdown);
        }
#endif
    }


    void ConsoleLogFile::Shutdown()
    {
#ifdef TURN_ON_LOGGING_CONSOLE_TO_FILE
        ConsoleLogFile& logFile = ConsoleLogFile::GetInstance();
        logFile.GetFileStream().close();
        if (logFile.GetFileStream().fail())
        {
            AE_WARN("ConsoleLogFile.cpp: Log file failed to close!");
        }
        else
        {
            m_IsLogFileOpen = false;
        }
#endif
    }

    ConsoleLogFile& ConsoleLogFile::operator<<(const std::ostringstream& ostream)
    {
        if (m_IsCurrentlyLogging)
        {
            GetFileStream() << ostream.str();
        }
        return *this;
    }

    ConsoleLogFile::ConsoleLogFile()
    {
        Init();
    }

    ConsoleLogFile::~ConsoleLogFile()
    {

    }



} // Debug