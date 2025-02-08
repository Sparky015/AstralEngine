//
// Created by Andrew Fagan on 11/17/24.
//

#pragma once

#include <fstream>
#include <sstream>

namespace Debug {

    /** @class ConsoleLogFile
     *  @brief Singleton that represents the engine-wide log file that all logging macros write into. */
    class ConsoleLogFile
    {
    public:
        /** Initializes the file stream for logging the macro outputs. This includes the creation of the folder structure that contains the logs. */
        void Init();

        /** Closes the file stream for logging macro outputs. */
        static void Shutdown();

        /** @brief Creates the LogFile instance on first call.
         * @note This set up ensures the log file is set up when static and global objects start trying to log messages. */
        inline static ConsoleLogFile& GetInstance()
        {
            static ConsoleLogFile m_Instance;
            return m_Instance;
        }

        ConsoleLogFile& operator<<(const std::ostringstream& ostream)
        {
            if (m_IsCurrentlyLogging)
            {
                GetFileStream() << ostream.str();
            }
            return *this;
        }

        /// Returns true if the log file is open
        inline bool IsOpen() const { return m_IsLogFileOpen; }
        inline bool IsLogging() const { return m_IsCurrentlyLogging; }
        inline void Enable() { m_IsCurrentlyLogging = true; }
        inline void Disable() { m_IsCurrentlyLogging = false; }

        ConsoleLogFile(const ConsoleLogFile&) = delete;
        ConsoleLogFile& operator=(const ConsoleLogFile&) = delete;
        ConsoleLogFile(ConsoleLogFile&&) = delete;
        ConsoleLogFile& operator=(ConsoleLogFile&&) = delete;
    private:
        ConsoleLogFile();
        ~ConsoleLogFile();

        /// Constructs and initializes the LogFile on first call
        inline std::fstream& GetFileStream() const
        {
            static std::fstream m_LogFile;
            return m_LogFile;
        }

        bool m_IsCurrentlyLogging = true;
        static bool m_IsLogFileOpen;
    };

} // Debug

