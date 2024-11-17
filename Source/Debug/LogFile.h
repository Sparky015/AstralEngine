//
// Created by Andrew Fagan on 11/17/24.
//

#pragma once

#include <fstream>

namespace Debug {

    /** @class Singleton that represents the engine-wide log file that all logging macros write into. */
    class LogFile
    {
    public:
        /** Initializes the file stream for logging the macro outputs. This includes the creation of the folder structure that contains the logs. */
        void Init();

        /** Closes the file stream for logging macro outputs. */
        static void Shutdown();

        /** @brief Creates the LogFile instance on first call.
         * @note This set up ensures the log file is set up when static and global objects start trying to log messages. */
        inline static LogFile& GetInstance()
        {
            static LogFile m_Instance;
            return m_Instance;
        }

        // Constructs and initializes the LogFile on first call
        inline std::fstream& GetFileStream()
        {
            static std::fstream m_LogFile;
            return m_LogFile;
        }

        // Returns true if the log file is open
        inline bool IsOpen()
        {
            GetFileStream(); // This statement is to check that the LogFile instance has been set up to ensure that m_IsLogFileOpen is initialized.
            return m_IsLogFileOpen;
        }

        LogFile(const LogFile&) = delete;
        LogFile& operator=(const LogFile&) = delete;
        LogFile(LogFile&&) = delete;
        LogFile& operator=(LogFile&&) = delete;
    private:
        LogFile();
        ~LogFile();

        static bool m_IsLogFileOpen;
    };

} // Debug

