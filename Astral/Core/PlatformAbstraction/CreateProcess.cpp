/**
* @file CreateProcess.cpp
* @author Andrew Fagan
* @date 4/13/26
*/

#include "CreateProcess.h"

#ifdef ASTRAL_PLATFORM_MACOS
#include "Platform/Unix/UnixCreateProcess.h"
#endif

#ifdef ASTRAL_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsCreateProcess.h"
#endif


void Astral::CreateChildProcess(const char* filePath, char* const* argv, char* const* envp)
{
#ifdef ASTRAL_PLATFORM_MACOS
    UnixCreateProcess(filePath, argv, envp);
#endif

#ifdef ASTRAL_PLATFORM_WINDOWS
    WindowsCreateProcess(filePath, argv, envp);
#endif
}
