//
// Created by Andrew Fagan on 11/21/24.
//

#pragma once

#include <fstream>

#include "Core/FixedIntegerTypes.h"

namespace Astral {

    struct ProfileResult
    {
        std::string_view Name;
        Astral::int64 StartTimeStamp;
        Astral::int64 EndTimeStamp;
        size_t AllocationCount;
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
        void SetUpJson() const;
        void EndJson() const;

        static bool m_IsLogFileOpen;
        std::string m_FilePath;
        int m_ProfileCount;
    };

} // Debug
