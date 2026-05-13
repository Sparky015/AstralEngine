/**
* @file CreateProcess.h
* @author Andrew Fagan
* @date 4/13/26
*/

#pragma once

namespace Astral {

    void CreateChildProcess(const char* filePath, char* const* argv, char* const* envp);

}