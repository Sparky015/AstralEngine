//
// Created by Andrew Fagan on 11/21/24.
//

#pragma once

#include <fstream>

namespace Debug {

    struct ProfileResult
    {
        std::string Name;
        int64 StartTimeStamp;
        int64 EndTimeStamp;
    };


    class ProfilerLogFile
    {
    public:
        void Init();
        static void Shutdown();

        void WriteProfile(const ProfileResult& profileResult);


        static inline ProfilerLogFile& GetInstance()
        {
            static ProfilerLogFile profilerLog;
            return profilerLog;
        };

        [[nodiscard]] inline std::ofstream& GetFileStream() const
        {
            static std::ofstream profilerLog;
            return profilerLog;
        }

        ProfilerLogFile(const ProfilerLogFile&) = delete;
        ProfilerLogFile& operator=(const ProfilerLogFile&) = delete;
        ProfilerLogFile(ProfilerLogFile&&) = delete;
        ProfilerLogFile& operator=(ProfilerLogFile&&) = delete;
    private:
        ProfilerLogFile();
        ~ProfilerLogFile() = default;

        void GenerateFilePath();
        void SetUpJson();
        void EndJson();

        static bool m_IsLogFileOpen;
        std::string m_FilePath;
        int m_ProfileCount;
    };

} // Debug
