//
// Created by Andrew Fagan on 1/2/25.
//

#include "Debug/Utils/Public/Error.h"

#include "Debug/LogFiles/ConsoleLogFile.h"
#include "ConsoleColors.h"

namespace Debug::Macros {

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

    #ifdef TURN_ON_LOGGING_CONSOLE_TO_FILE
        ConsoleLogFile& logFile = ConsoleLogFile::GetInstance();
        if (logFile.IsOpen())
        {
            std::ostringstream outputStream;
            outputStream << "\n\nERROR called. \n" << filePath
                    << "\nLine: " << lineNumber
                    << "\nFunction: " << func
                    << "\nError Message: " << errorMessage.str() << "\n";
            logFile << outputStream;
            ConsoleLogFile::Shutdown();
        }
        else
        {
            WARN("Attempted write to a log file that is already closed!");
        }
    #endif

        std::cout << "\n\n" << SetColor(RED)
        << "ERROR called. \nFile: " << filePath
        << "\nLine: " << lineNumber
        << "\nFunction: " << func
        << "\nError Message: " << errorMessage.str() << SetColor(DEFAULT) << "\n";
    }

}