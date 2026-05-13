/**
* @file UnixCreateProcess.cpp
* @author Andrew Fagan
* @date 4/13/26
*/

#include "UnixCreateProcess.h"

#include "Utilities/Loggers.h"

#include <spawn.h>
#include <unistd.h>

namespace Astral {

    void UnixCreateProcess(const char* filePath, char* const* argv, char* const* envp)
    {
        pid_t childProcessID;

        posix_spawn_file_actions_t spawnFileActions{};
        posix_spawn_file_actions_init(&spawnFileActions);
        posix_spawn_file_actions_addclose(&spawnFileActions, STDOUT_FILENO);

        posix_spawnattr_t spawnAttributes;
        posix_spawnattr_init(&spawnAttributes);

        int status = posix_spawn(&childProcessID, filePath, &spawnFileActions, &spawnAttributes, argv, envp);

        if (status != 0)
        {
            AE_WARN("Unix process failed to spawn!");
        }

        posix_spawn_file_actions_destroy(&spawnFileActions);
        posix_spawnattr_destroy(&spawnAttributes);
    }

}
