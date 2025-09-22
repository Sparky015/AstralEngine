//
// Created by Andrew Fagan on 1/2/25.
//

#include "Error.h"

#include "ConsoleLogFile.h"
#include "ConsoleColors.h"

#include <cpptrace/cpptrace.hpp>

namespace Astral::Macros {

    void macro_ERROR(std::ostringstream& errorMessage, const char* file, const int lineNumber, const char* func)
    {
        // Stripping the part of the file path prior to the root directory
        std::string filePath(file);
        std::string rootDir(ROOT_DIR);
        size_t pos = filePath.find(rootDir);
        if (pos != std::string::npos)
        {
            filePath.erase(pos, rootDir.length());
        }

    #ifndef TURN_OFF_LOGGING_CONSOLE_TO_FILE
        ConsoleLogFile& logFile = ConsoleLogFile::GetInstance();
        if (logFile.IsOpen())
        {
            std::ostringstream outputStream;

            outputStream << "\n" << cpptrace::generate_trace().current(1).to_string();

            outputStream << "\nERROR called. \n" << filePath
                    << "\nLine: " << lineNumber
                    << "\nFunction: " << func
                    << "\nError Message: " << errorMessage.str() << "\n";
            logFile << outputStream;
            ConsoleLogFile::Shutdown();
        }
        else
        {
            AE_WARN("Attempted write to a log file that is already closed!");
        }
    #endif

        std::cout << "\n";
        cpptrace::generate_trace().current(1).print();

        std::cout << "\n" << SetColor(RED)
        << "ERROR called. \nFile: " << filePath
        << "\nLine: " << lineNumber
        << "\nFunction: " << func
        << "\nError Message: " << errorMessage.str() << SetColor(DEFAULT) << "\n\n";
    }

}