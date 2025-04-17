#include "Core/Engine.h"
#include "Debug/EngineDebugInfoLogger.h"
#include "Debug/Utilities/Loggers.h"
#include "Core/Memory/Allocators/STLAllocators/STLLinearAllocator.h"
#include "Debug/MemoryTracking/MemoryTracker.h"

#include "nfd.hpp"

int main()
{
    LOG("\n---   Entering main()    ---     (static and global object initialization above)\n");

    NFD_Init();

    Core::MemoryTracker::Get().Init();
    // Core::MemoryTracker::Get().BeginScene("Engine_Lifetime");

    {
        Astral::Engine engine = Astral::Engine();
        Debug::LogEngineDebugInfo(); // Need to set up window and rendering context before calling this.
        engine.Run();
    }

    NFD_Quit();

    LOG("\n---   Exiting main() ---     (static and global object destruction below)\n");
    // Core::MemoryTracker::Get().EndScene();
    return 0;
}
