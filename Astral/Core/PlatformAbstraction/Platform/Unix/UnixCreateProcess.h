/**
* @file UnixCreateProcess.h
* @author Andrew Fagan
* @date 4/13/26
*/

#pragma once

namespace Astral {

    void UnixCreateProcess(const char* filePath, char* const* argv, char* const* envp);

}