#include "Core/Engine.h"
#include "Debug/EngineDebugInfoLogger.h"
#include "Debug/Utilities/Loggers.h"
#include "Debug/MemoryTracking/MemoryTracker.h"

int main()
{
    LOG("\n---   Entering main()    ---     (static and global object initialization above)\n");


    {
        Core::MemoryTracker::Get().Init();
        Astral::Engine engine = Astral::Engine();
        Debug::LogEngineDebugInfo(); // Need to set up window and rendering context before calling this.
        engine.Run();
    }


    LOG("\n---   Exiting main() ---     (static and global object destruction below)\n");
    return 0;
}