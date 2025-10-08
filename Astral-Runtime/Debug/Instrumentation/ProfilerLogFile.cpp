//
// Created by Andrew Fagan on 11/21/24.
//

#include "ProfilerLogFile.h"

#include "Debug/Utilities/Loggers.h"

#include <filesystem>
#include <thread>
#include <unordered_map>


namespace Astral {

    bool ProfilerLogFile::m_IsLogFileOpen = false;

    void ProfilerLogFile::Init()
    {
        GenerateFilePath();
        m_FilePath += ".json";

        GetFileStream().open(m_FilePath);
        if (GetFileStream().fail())
        {
            AE_WARN("Profiler log file failed to open!");
        }
        else
        {
            SetUpJson();
            m_IsLogFileOpen = true;
            std::atexit(Shutdown);
        }
    }


    void ProfilerLogFile::Shutdown()
    {
        ProfilerLogFile& profilerLogFile = GetInstance();
        profilerLogFile.EndJson();

        profilerLogFile.GetFileStream().close();
        if (profilerLogFile.GetFileStream().fail())
        {
            AE_WARN("DebugMacros.cpp: Log file failed to close!");
        }
        else
        {
            m_IsLogFileOpen = false;
        }
    }


    void ProfilerLogFile::WriteProfile(const ProfileResult& profileResult)
    {
        std::lock_guard lock(m_WriteLock);

        std::ofstream& fileStream = GetFileStream();

        static std::unordered_map<size_t, uint32> threadHashToID{};
        size_t threadHash = std::hash<std::thread::id>{}(std::this_thread::get_id());
        if (!threadHashToID.contains(threadHash))
        {
            threadHashToID[threadHash] = threadHashToID.size();
        }

        if (m_ProfileCount > 0)
        {
            fileStream << ",\n";
        }
        fileStream << "{"
        << "\"cat\": \"function\","
        << "\"dur\": " << profileResult.EndTimeStamp - profileResult.StartTimeStamp << ","
        << "\"name\": \"" << profileResult.Name << "\","
        << "\"ph\": \"X\","
        << "\"pid\": 0,"
        << "\"tid\": " << threadHashToID[threadHash] << ","
        << 	"\"ts\": " << profileResult.StartTimeStamp << ","
        << "\"args\": {"
            << "\"Allocation Count\": " << profileResult.AllocationCount
            << "}"
        << " }";

//        fileStream.flush(); // Only use if the program is crashing.

        m_ProfileCount++;
    }


    ProfilerLogFile::ProfilerLogFile()
    {
        Init();
    }


    void ProfilerLogFile::GenerateFilePath()
    {
        /**
         * Overall filepath of log file. It will progressively have folders appended to it in the lines
         * following to form the full filepath.
         * */
        std::stringstream filePathStream;

        const std::time_t t = std::time(nullptr);
        const std::tm* currentTime = std::localtime(&t);

        /** Root folder name for all the console logs. */
        const std::string rootFolder = std::string(ROOT_DIR) + "/Logs";
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
        hrMinSecTextFileNameStream << currentTime->tm_hour << "-" << currentTime->tm_min << "-" << currentTime->tm_sec << "-Profiler";

        /** filePathStream = "ConsoleLog/[Year]-[Month]/[Day]/[Hour]_[Minute]_[Second].txt" */
        filePathStream << hrMinSecTextFileNameStream.str();

        m_FilePath = filePathStream.str();
    }


    void ProfilerLogFile::SetUpJson() const
    {
        std::ofstream& fileStream = GetFileStream();
        fileStream << R"({ "otherData": {},
 "traceEvents": [)";
        fileStream.flush();
    }


    void ProfilerLogFile::EndJson() const
    {
        std::ofstream& fileStream = GetFileStream();
        fileStream << "]\n}";
        fileStream.flush();
    }


} // Debug